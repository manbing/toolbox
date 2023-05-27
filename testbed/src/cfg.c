#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libubox/uloop.h>
#include <libubox/vlist.h>
#include <libubox/blob.h>
#include <uci.h>

#include "cfg.h"

struct vlist_tree config;
static struct uci_context *uci_ctx = NULL;
static struct uci_package *uci_testbed = NULL;

static void config_update(struct vlist_tree *tree, struct vlist_node *node_new,
			  struct vlist_node *node_old)
{
	struct girl *old = container_of(node_old, struct girl, node);
	struct girl *new = container_of(node_new, struct girl, node);

	if (old && new) {
		printf("[%s:%d] Update name from %s to %s\n", __func__,
		       __LINE__, old->name, new->name);
		printf("[%s:%d] Update age from %d to %d\n", __func__, __LINE__,
		       old->age, new->age);

		const char *tmp = NULL;
		tmp = old->name;
		old->name = new->name;
		new->name = tmp;
		old->age = new->age;

		free((char *)new->name);
		free(new);
	} else if (old) {
		printf("[%s:%d] Remove %s, age %d\n", __func__, __LINE__,
		       old->name, old->age);
		free((char *)old->name);
		free(old);
	} else if (new) {
		printf("[%s:%d] Create %s, age %d\n", __func__, __LINE__,
		       new->name, new->age);
	}
}

static int config_cmp(const void *k, const void *k2, void *ptr)
{
	const char *key = k, *key2 = k2;

	return strcmp(key, key2);
}

static void config_parse_girl(struct uci_section *s)
{
	struct girl *obj = NULL;
	const char *age = NULL;
	const char *name = NULL;
	char *key = NULL;

	obj = calloc_a(sizeof(*obj), &key, strlen(s->e.name) + 1);
	if (!obj)
		return;

	obj->key = strcpy(key, s->e.name);

	name = uci_lookup_option_string(uci_ctx, s, "name");
	if (name)
		obj->name = strdup(name);

	age = uci_lookup_option_string(uci_ctx, s, "age");
	if (age)
		obj->age = strtol(age, NULL, 10);
	else
		obj->age = 0;

	vlist_add(&config, &obj->node, obj->key);
}

static void config_init_girl(void)
{
	struct uci_element *e;

	uci_foreach_element(&uci_testbed->sections, e)
	{
		struct uci_section *s = uci_to_section(e);

		if (!strcmp(s->type, "girl"))
			config_parse_girl(s);
	}
}

static struct uci_package *__init_package(const char *config)
{
	struct uci_package *p = NULL;

	if (!uci_ctx)
		return NULL;

	p = uci_lookup_package(uci_ctx, config);
	if (p)
		uci_unload(uci_ctx, p);

	if (uci_load(uci_ctx, config, &p))
		return NULL;

	return p;
}

static void __attribute__((constructor)) config_preinit(void)
{
	vlist_init(&config, config_cmp, config_update);
	config.keep_old = true;
	config.no_delete = true;
}

void config_init(void)
{
	if (!uci_ctx)
		uci_ctx = uci_alloc_context();

	//uci_set_confdir(ctx, config_path);
	//uci_set_savedir(ctx, "./tmp");

	uci_testbed = __init_package("testbed");

	vlist_update(&config);
	config_init_girl();
	vlist_flush(&config);
}

void config_deinit(void)
{
	if (uci_testbed) {
		uci_save(uci_ctx, uci_testbed);
		uci_commit(uci_ctx, &uci_testbed, false);
		uci_unload(uci_ctx, uci_testbed);
	}

	if (uci_ctx)
		uci_free_context(uci_ctx);
}

/*
static int uci_init_uci_ptr(struct uci_ptr *ptr, const char *package, const char *section, const char *option)
{
        if (!ptr)
                return UCI_ERR_INVAL;
        
        memset(ptr, 0, sizeof(struct uci_ptr));

        ptr->package = package;
        ptr->target = UCI_TYPE_PACKAGE;
        if (!package)
                return UCI_ERR_INVAL;

        ptr->section = section;
        ptr->target = UCI_TYPE_SECTION;
        if (!section) {
                ptr->target = UCI_TYPE_PACKAGE;
                goto out;
        }

        ptr->option = option;
        ptr->target = UCI_TYPE_OPTION;
        if (!section) {
                ptr->target = UCI_TYPE_SECTION;
                goto out;
        }

out:
        return UCI_OK;
}
*/

int uci_shell_delete_option(const char *cfg)
{
	struct uci_ptr ptr;
	char cfg2[128] = { 0 };

	if (!cfg)
		return -1;

	if (!uci_ctx) {
		uci_ctx = uci_alloc_context();
		if (!uci_ctx) {
			printf("uci_alloc_context fail\n");
			return UCI_ERR_INVAL;
		}
	}

	snprintf(cfg2, sizeof(cfg2), "%s", cfg);
	if (uci_lookup_ptr(uci_ctx, &ptr, cfg2, true) != UCI_OK) {
		return -1;
	}

	if (uci_delete(uci_ctx, &ptr) != UCI_OK) {
		printf("uci_delete fail\n");
		return -1;
	}

	if (uci_save(uci_ctx, ptr.p) != UCI_OK) {
		printf("uci_save fail");
	}

	if (uci_commit(uci_ctx, &ptr.p, false) != UCI_OK) {
		printf("uci_commit fail");
	}

	uci_unload(uci_ctx, ptr.p);
	return 0;
}

int uci_shell_set_option(const char *cfg, const char *value)
{
	int ret = 0;
	struct uci_ptr ptr;
	char cfg2[128] = { 0 };

	if (!uci_ctx) {
		uci_ctx = uci_alloc_context();
		if (!uci_ctx) {
			printf("uci_alloc_context fail\n");
			return 0;
		}
	}

	snprintf(cfg2, sizeof(cfg2), "%s", cfg);
	if (uci_lookup_ptr(uci_ctx, &ptr, cfg2, true) != UCI_OK) {
		return -1;
	}

	if (value)
		ptr.value = value;

	uci_set(uci_ctx, &ptr);

	if (ptr.o && ptr.o->v.string)
		ret = strlen(ptr.o->v.string);

	if (uci_save(uci_ctx, ptr.p) != UCI_OK) {
		printf("uci_save fail");
	}

	if (uci_commit(uci_ctx, &ptr.p, false) != UCI_OK) {
		printf("uci_commit fail");
	}

	uci_unload(uci_ctx, ptr.p);
	return ret;
}

int uci_shell_add_section(const char *package, char *section_type)
{
	struct uci_package *p = NULL;
	struct uci_section *s = NULL;

	if (!uci_ctx) {
		uci_ctx = uci_alloc_context();
		if (!uci_ctx) {
			printf("uci_alloc_context fail\n");
			return -1;
		}
	}

	if (uci_load(uci_ctx, package, &p) != UCI_OK) {
		printf("uci_load fail\n");
		return -1;
	}

	if (uci_add_section(uci_ctx, p, section_type, &s) != UCI_OK) {
		printf("uci_add_section fail\n");
		return -1;
	}

	if (uci_save(uci_ctx, p) != UCI_OK) {
		printf("uci_save fail");
	}

	return 0;
}

int uci_shell_get_option(char *dst, size_t size, const char *cfg)
{
	int ret = 0;
	struct uci_ptr ptr;
	char cfg2[128] = { 0 };

	if (!uci_ctx) {
		uci_ctx = uci_alloc_context();
		if (!uci_ctx) {
			printf("uci_alloc_context fail\n");
			return -1;
		}
	}

	snprintf(cfg2, sizeof(cfg2), "%s", cfg);
	if (uci_lookup_ptr(uci_ctx, &ptr, cfg2, true) != UCI_OK) {
		return -1;
	}

	if (!ptr.o)
		return -1;

	if (ptr.o->v.string)
		ret = snprintf(dst, size, "%s", ptr.o->v.string);
	else
		ret = 0;

	uci_unload(uci_ctx, ptr.p);
	return ret;
}

int uci_api_add_list(struct uci_package *p, struct uci_section *s,
		     const char *option_name, const char *val)
{
	struct uci_option *o = uci_lookup_option(uci_ctx, s, option_name);
	struct uci_ptr ptr;

	memset(&ptr, 0, sizeof(struct uci_ptr));
	ptr.p = p;
	ptr.s = s;
	ptr.o = o;
	ptr.option = option_name;
	ptr.value = val;
	return uci_add_list(uci_ctx, &ptr);
}

int uci_api_set(struct uci_package *p, struct uci_section *s,
		const char *option_name, const char *val)
{
	struct uci_option *o = uci_lookup_option(uci_ctx, s, option_name);
	struct uci_ptr ptr;

	memset(&ptr, 0, sizeof(struct uci_ptr));
	ptr.p = p;
	ptr.s = s;
	ptr.o = o;
	ptr.option = option_name;
	ptr.value = val;
	return uci_set(uci_ctx, &ptr);
}

int uci_api_add_section(struct uci_package *p, const char *section_name,
			struct uci_section **new)
{
	int retval = 0;
	struct uci_ptr ptr;

	memset(&ptr, 0, sizeof(struct uci_ptr));
	ptr.p = p;
	retval = uci_add_section(uci_ctx, p, section_name, &ptr.s);
	*new = ptr.s;
	return retval;
}

int uci_api_delete(struct uci_package *p, struct uci_section *s,
		   struct uci_option *o)
{
	struct uci_ptr ptr;

	memset(&ptr, 0, sizeof(struct uci_ptr));
	ptr.p = p;
	ptr.s = s;
	ptr.o = o;
	return uci_delete(uci_ctx, &ptr);
}

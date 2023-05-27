#ifndef _CONF_H_
#define _CONF_H_

#include <uci.h>
#include <libubox/vlist.h>

struct girl {
	struct vlist_node node;
	const char *key;
	const char *name;
	int age;
};

void config_init(void);
int uci_shell_delete_option(const char *cfg);
int uci_shell_set_option(const char *cfg, const char *value);
int uci_shell_add_section(const char *package, char *section_type);
int uci_shell_get_option(char *dst, size_t size, const char *cfg);
int uci_api_add_list(struct uci_package *p, struct uci_section *s,
		     const char *option_name, const char *val);
int uci_api_set(struct uci_package *p, struct uci_section *s,
		const char *option_name, const char *val);
int uci_api_add_section(struct uci_package *p, const char *section_name,
			struct uci_section **new);
int uci_api_delete(struct uci_package *p, struct uci_section *s,
		   struct uci_option *o);

#endif /* !_CONF_H_ */

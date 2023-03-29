#ifndef _CONF_H_
#define _CONF_H_

#include <libubox/vlist.h>

struct girl {
        struct vlist_node node;
		const char *key;
        const char *name;
        int age;
};

void config_init(void);
int cfg_delete_option(const char *cfg);
int cfg_set_option(const char *cfg, const char *value);
int cfg_get_option(char *dst, size_t size, const char *cfg);
int cfg_add_section(const char *package, char *section_type);

#endif /* !_CONF_H_ */

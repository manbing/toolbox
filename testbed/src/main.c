#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libubox/uloop.h>
#include <libubox/vlist.h>

#include "ubus.h"
#include "cfg.h"


static void __test_cfg_api(void)
{
        char tmp[8] = {0};
        cfg_delete_option("network.@interface[0].gw");
        cfg_get_option(tmp, sizeof(tmp), "network.@interface[0].gw");
        printf("[%s:%d] tmp = %s\n", __func__, __LINE__, tmp);
        cfg_set_option("network.@interface[0].gw", "456");
        cfg_get_option(tmp, sizeof(tmp), "network.@interface[0].gw");
        printf("[%s:%d] tmp = %s\n", __func__, __LINE__, tmp);
        cfg_add_section("network", "interface");
}

int main(void)
{
        ubus_init();
        config_init();
        //__test_cfg_api();

        uloop_run();
        ubus_deinit();
        return 0;
}

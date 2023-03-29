#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libubus.h>
#include <libubox/blobmsg_json.h>
#include "cjson/cJSON.h"

static struct ubus_context *ubus_ctx;
static struct blob_buf b;

static void cb(struct ubus_request *req, int type, struct blob_attr *msg)
{
        char *str = NULL;
		cJSON *root = NULL;
		cJSON *ipv4_address = NULL;
		cJSON *ip = NULL;
		cJSON *mask = NULL;
		cJSON *route = NULL;
		cJSON *dns_servers = NULL;
		cJSON *device = NULL;
        
		str = blobmsg_format_json_indent(msg, true, 0);
        //printf("received date:\n %s \n", str);

		root = cJSON_Parse(str);
		if (!root)
			goto out;

		device = cJSON_GetObjectItem(root, "device");
		if (!device)
			goto out2;

		if (device->type == cJSON_String)
			printf("%s\n", device->valuestring);

		ipv4_address = cJSON_GetObjectItem(root, "ipv4-address");
		if (!ipv4_address)
			goto out2;

		if (ipv4_address->type == cJSON_Array) {
			int size = cJSON_GetArraySize(ipv4_address);
			int i = 0;
			cJSON *tmp = NULL, *tmp2 = NULL;

			for (i = 0; i < size; i++) {
				tmp = cJSON_GetArrayItem(ipv4_address, i);
				tmp2 = cJSON_GetObjectItem(tmp, "address");
				if (tmp2->type == cJSON_String)
					printf("IP is %s\n", tmp2->valuestring);
				
				tmp2 = cJSON_GetObjectItem(tmp, "mask");
				if (tmp2->type == cJSON_Number)
					printf("mask is %d\n", tmp2->valueint);
			}
		}

		route = cJSON_GetObjectItem(root, "route");
		if (!route)
			goto out2;
		
		if (route->type == cJSON_Array) {
			int size = cJSON_GetArraySize(route);
			int i = 0;
			cJSON *tmp = NULL, *tmp2 = NULL, *tmp3 = NULL;

			for (i = 0; i < size; i++) {
				tmp = cJSON_GetArrayItem(route, i);
				tmp2 = cJSON_GetObjectItem(tmp, "mask");

				if (!tmp2->valueint) {
					tmp3 = cJSON_GetObjectItem(tmp, "nexthop");
					if (tmp3)
						printf("gateway is %s\n", tmp3->valuestring);
				}
			}
		}
		
		dns_servers = cJSON_GetObjectItem(root, "dns-server");
		if (!dns_servers)
			goto out2;
		
		if (dns_servers->type == cJSON_Array) {
			int size = cJSON_GetArraySize(dns_servers);
			int i = 0;
			cJSON *tmp = NULL;

			printf("DNS is ");
			for (i = 0; i < size; i++) {
				tmp = cJSON_GetArrayItem(dns_servers, i);
				if (tmp->type == cJSON_String) {
					printf("%s ", tmp->valuestring);
				}
			}
			printf("\n");
		}

out2:
		cJSON_Delete(root);
out:
        free(str);
}

int main(void)
{
        unsigned int obj_id = 0;
        int ret = 0;
        const char *path = "network.interface.wan";
        int timeout = 5;

        if (!(ubus_ctx = ubus_connect(NULL))) {
                fprintf(stderr, "connect to ubus fail\n");
                return -1;
        }

        printf("connected id %08x\n", ubus_ctx->local_id);

        ret = ubus_lookup_id(ubus_ctx, path, &obj_id);
        if (ret != UBUS_STATUS_OK) {
                fprintf(stderr, "ubus_lookup_id() fail: %s\n", ubus_strerror(ret));
                return -1;
        }

        blob_buf_init(&b, 0);
        //blobmsg_add_string(&b, "name", "br-lan");

        ret = ubus_invoke(ubus_ctx, obj_id, "status", b.head, cb, NULL, timeout * 1000);
        if (ret != UBUS_STATUS_OK) {
                fprintf(stderr, "ubus_invoke() fail: %s\n", ubus_strerror(ret));
                return -1;
        }

		ubus_free(ubus_ctx);
        return 0;
}

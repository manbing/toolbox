#include <fcntl.h>
#include <libubox/uloop.h>
#include <libubus.h>

#include "ubus.h"
#include "cfg.h"

struct ubus_context *ubus_ctx = NULL;

static int main_object_restart(struct ubus_context *ctx, struct ubus_object *obj,
                	       struct ubus_request_data *req, const char *method,
                               struct blob_attr *msg)
{
        printf("[%s:%d]\n", __func__, __LINE__);
        return UBUS_STATUS_OK;
}


static int main_object_reload(struct ubus_context *ctx, struct ubus_object *obj,
                	       struct ubus_request_data *req, const char *method,
                               struct blob_attr *msg)
{
        printf("[%s:%d]\n", __func__, __LINE__);
        config_init();
        return UBUS_STATUS_OK;
}

static struct ubus_method main_object_methods[] = {
        {.name = "restart", .handler = main_object_restart},
        {.name = "reload", .handler = main_object_reload},
};

static struct ubus_object_type main_object_type =
	UBUS_OBJECT_TYPE("testbed", main_object_methods);

static struct ubus_object main_object = {
        .name = "testbed.girl",
        .type = &main_object_type,
        .methods = main_object_methods,
        .n_methods = ARRAY_SIZE(main_object_methods),
};

static void ubus_add_fd(void)
{
        ubus_add_uloop(ubus_ctx);
        fcntl(ubus_ctx->sock.fd, F_SETFD, fcntl(ubus_ctx->sock.fd, F_GETFD) | FD_CLOEXEC);
}

static void ubus_reconnect_timer(struct uloop_timeout *timeout)
{
        static struct uloop_timeout retry = {
                .cb = ubus_reconnect_timer,
        };
        int t = 2;

        if (ubus_reconnect(ubus_ctx, NULL) != 0) {
                printf("failed to reconnect, trying again in %d seconds\n", t);
                uloop_timeout_set(&retry, t * 1000);
                return;
        }

        printf("reconnected to ubus, new id: %08x\n", ubus_ctx->local_id);
        ubus_add_fd();
}

static void ubus_connection_lost(struct ubus_context *ctx)
{
        ubus_reconnect_timer(NULL);
}


static void handle_signal(int signo)
{
        ubus_deinit();
}

static void setup_signals(void)
{
        struct sigaction s;

        memset(&s, 0, sizeof(s));
        s.sa_handler = handle_signal;
        s.sa_flags = 0;
        sigaction(SIGINT, &s, NULL);
        sigaction(SIGTERM, &s, NULL);
        sigaction(SIGUSR1, &s, NULL);
        sigaction(SIGUSR2, &s, NULL);

        s.sa_handler = SIG_IGN;
        sigaction(SIGPIPE, &s, NULL);
}

static void __add_object(struct ubus_object *obj)
{
        int retval = 0;

        retval = ubus_add_object(ubus_ctx, obj);

        if (retval)
                fprintf(stderr, "Failed to publish object '%s': %s\n", obj->name, ubus_strerror(retval));
}

static void add_objects(void)
{
        __add_object(&main_object);
}

bool ubus_init(void)
{
        setup_signals();
        uloop_init();
        ubus_ctx = ubus_connect(NULL);

        if (!ubus_ctx)
                return false;
        
        printf("connected as %08x\n", ubus_ctx->local_id);
        ubus_ctx->connection_lost = ubus_connection_lost;
        ubus_add_fd();
        add_objects();

        return true;
}

void ubus_deinit(void)
{
        uloop_end();
        ubus_free(ubus_ctx);
}

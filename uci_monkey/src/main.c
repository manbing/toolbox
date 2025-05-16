#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#include <uci.h>
#include <libubox/uloop.h>

#define UCI_PACKAGE "uci_test"
#define SECS_IN_DAY (24 * 60 * 60)

static char name[32] = {0};
void monkey_read_func(struct uloop_timeout *timeout);
void monkey_write_func(struct uloop_timeout *timeout);
void monkey_write_func2(struct uloop_timeout *timeout);

static void displayClock(pid_t pid, char *func, int line)
{
	clockid_t clock = CLOCK_MONOTONIC;
	char *name = "CLOCK_MONOTONIC"; 
	struct timespec ts;

	if (clock_gettime(clock, &ts) == -1) {
		perror("clock_gettime");
		exit(EXIT_FAILURE);
	}

	printf("[%d:%s:%d] %10ld.%03ld\n", pid, func, line,
			(long) ts.tv_sec, ts.tv_nsec / 1000000);
}

struct uloop_timeout monkey_read_timer = {
	.cb = monkey_read_func,
};

struct uloop_timeout monkey_write_timer = {
	.cb = monkey_write_func,
};

struct uloop_timeout monkey_write_timer2 = {
	.cb = monkey_write_func2,
};

void monkey_read_func(struct uloop_timeout *timeout)
{
	const char *value = NULL;
	struct uci_context *ctx = NULL;
	struct uci_package *p = NULL;
	struct uci_section *s = NULL;
	//struct uci_element *e = NULL;

	ctx = uci_alloc_context();
	if (ctx == NULL)
		return;

	uci_load(ctx, UCI_PACKAGE, &p);
	if (!p) {
		printf("uci_load() error\n");
		goto uci_out;
	}
	
	s = uci_lookup_section(ctx, p, "test");
	if (s == NULL) {
		printf("uci_lookup_section() error\n");
		goto uci_out;
	}

	value = uci_lookup_option_string(ctx, s, "init01");
	value = uci_lookup_option_string(ctx, s, "init02");
	value = uci_lookup_option_string(ctx, s, "master01");
	value = uci_lookup_option_string(ctx, s, "master02");

uci_out:
	uci_free_context(ctx);

	uloop_timeout_set(&monkey_read_timer, 500);
}

void monkey_write_func(struct uloop_timeout *timeout)
{
	static int test_write_times = 0;
	char v[32] = {0};

	test_write_times++;
	snprintf(v, sizeof(v), "%d", test_write_times);

	struct uci_context *ctx = NULL;
	struct uci_package *p = NULL;
	struct uci_section *s = NULL;
	struct uci_option *o = NULL;
	//struct uci_element *e = NULL;
	struct uci_ptr ptr = {0};

	ctx = uci_alloc_context();
	if (ctx == NULL)
		return;

	uci_load(ctx, UCI_PACKAGE, &p);
	if (!p) {
		printf("uci_load() error\n");
		goto uci_out;
	}

	s = uci_lookup_section(ctx, p, "test");
	if (s == NULL) {
		printf("uci_lookup_section() error\n");
		goto uci_out;
	}

	memset(&ptr, 0, sizeof(ptr));
	o = uci_lookup_option(ctx, s, "write_times");
	ptr.p = p;
	ptr.s = s;
	ptr.o = o;
	ptr.option = "write_times";
	ptr.value = v;
	uci_set(ctx, &ptr);

	uci_save(ctx, p);
	uci_commit(ctx, &p, false);
uci_out:
	uci_free_context(ctx);

	uloop_timeout_set(&monkey_write_timer, 500);
}

void monkey_write_func2(struct uloop_timeout *timeout)
{
	static int test_write_times = 0;
	char v[32] = {0};

	test_write_times++;
	snprintf(v, sizeof(v), "%d", test_write_times);

	struct uci_context *ctx = NULL;
	struct uci_package *p = NULL;
	struct uci_section *s = NULL;
	struct uci_option *o = NULL;
	struct uci_ptr ptr = {0};

	ctx = uci_alloc_context();
	if (ctx == NULL)
		return;

	uci_load(ctx, UCI_PACKAGE, &p);
	if (!p) {
		printf("uci_load() error\n");
		goto uci_out;
	}

	s = uci_lookup_section(ctx, p, name);
	if (s == NULL) {
		printf("uci_lookup_section() error\n");
		goto uci_out;
	}

	memset(&ptr, 0, sizeof(ptr));
	o = uci_lookup_option(ctx, s, "write_times");
	ptr.p = p;
	ptr.s = s;
	ptr.o = o;
	ptr.option = "write_times";
	ptr.value = v;
	uci_set(ctx, &ptr);

	uci_save(ctx, p);
	uci_commit(ctx, &p, false);
uci_out:
	uci_free_context(ctx);

	uloop_timeout_set(&monkey_write_timer2, 1000);
}

static void create_monkey(char *name)
{
	struct uci_context *ctx = NULL;
	struct uci_package *p = NULL;
	struct uci_section *s = NULL;
	struct uci_ptr ptr = {0};

	ctx = uci_alloc_context();
	if (ctx == NULL)
		return;

	uci_load(ctx, UCI_PACKAGE, &p);
	if (!p) {
		printf("uci_load() error\n");
		goto uci_out;
	}

	// Add section
	uci_add_section(ctx, p, name, &ptr.s);
	ptr.p = p;
	ptr.value = name;
	uci_rename(ctx, &ptr);
	uci_save(ctx, p);
	uci_commit(ctx, &p, false);
uci_out:;
	uci_free_context(ctx);

	// Create timer
	uloop_init();
	uloop_timeout_set(&monkey_write_timer, 500);
	uloop_timeout_set(&monkey_write_timer2, 1000);
	uloop_timeout_set(&monkey_read_timer, 800);
	uloop_run();
}

static void monkey_eren(void)
{
	struct uci_context *ctx = NULL;
	struct uci_package *p = NULL;
	struct uci_section *s = NULL;
	struct uci_option *o = NULL;
	//struct uci_element *e = NULL;
	struct uci_ptr ptr = {0};

	ctx = uci_alloc_context();
	if (ctx == NULL)
		return;

	uci_load(ctx, UCI_PACKAGE, &p);
	if (!p) {
		printf("uci_load() error\n");
		goto uci_out;
	}

	s = uci_lookup_section(ctx, p, "test");
	if (s == NULL) {
		printf("uci_lookup_section() error\n");
		goto uci_out;
	}

	memset(&ptr, 0, sizeof(ptr));
	o = uci_lookup_option(ctx, s, "master01");
	ptr.p = p;
	ptr.s = s;
	ptr.o = o;
	ptr.option = "master01";
	ptr.value = "master01";
	uci_set(ctx, &ptr);

	memset(&ptr, 0, sizeof(ptr));
	o = uci_lookup_option(ctx, s, "master02");
	ptr.p = p;
	ptr.s = s;
	ptr.o = o;
	ptr.option = "master02";
	ptr.value = "master02";
	uci_set(ctx, &ptr);

	uci_save(ctx, p);
	uci_commit(ctx, &p, false);
	uci_free_context(ctx);

	// Step 1 end
	sleep(30);
	// Step 2
	ctx = NULL;
	p = NULL;
	s = NULL;
	o = NULL;

	ctx = uci_alloc_context();
	if (ctx == NULL)
		return;

	uci_load(ctx, UCI_PACKAGE, &p);
	if (!p) {
		printf("uci_load() error\n");
		goto uci_out;
	}

	s = uci_lookup_section(ctx, p, "test");
	if (s == NULL) {
		printf("uci_lookup_section() error\n");
		goto uci_out;
	}
	
	memset(&ptr, 0, sizeof(ptr));
	o = uci_lookup_option(ctx, s, "target");
	ptr.p = p;
	ptr.s = s;
	ptr.o = o;
	ptr.option = "target";
	ptr.value = "master";
	uci_set(ctx, &ptr);

	uci_save(ctx, p);
	uci_commit(ctx, &p, false);
uci_out:
	uci_free_context(ctx);
	return;
}

FILE *uci_open_stream(const char *filename, bool write)
{
    struct stat statbuf;
    FILE *file = NULL;
    int fd, ret;
    int flags = (write ? O_RDWR : O_RDONLY);
    mode_t mode = 0x600;
    char *name = NULL;
    char *filename2 = NULL;

    fd = open(filename, flags, mode);
    if (fd < 0)
		return NULL;

    ret = flock(fd, (write ? LOCK_EX : LOCK_SH));
    if ((ret < 0) && (errno != ENOSYS)) {
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	int opt = 0;

	optind = 1;
	while ((opt=getopt(argc, argv, "ms:ab")) != -1) {
		switch (opt) {
			case 'm':
				monkey_eren();
				break;
			case 's':
				snprintf(name, sizeof(name),"%s", optarg);
				create_monkey(name);
				break;
			case 'a':
				uci_open_stream("/etc/config/uci_test", true);
				displayClock(getpid(), __func__, __LINE__);
				pause();
				break;
			case 'b':
				uci_open_stream("/etc/config/uci_test", false);
				displayClock(getpid(), __func__, __LINE__);
				pause();
				break;
			default:
				break;
		}
	}

	return 0;
}

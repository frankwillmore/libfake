#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static char *proccmd(void);

static struct passwd fake_pwd_struct =
{
	.pw_name = "fake_name",
	.pw_passwd = "x",
	.pw_uid = -1,
	.pw_gid = -1,
	.pw_gecos = "fake gecos",
	.pw_dir = "/fake_home/",
	.pw_shell = "/fake_shell"
};

struct passwd *
getpwuid(uid_t uid)
{
	char *fake_name = getenv("LOGNAME");
	if(fake_name)
		fake_pwd_struct.pw_name = fake_name;
	char *gid = getenv("GID");
	if(gid)
		fake_pwd_struct.pw_gid = atoi(gid);
	else
		fake_pwd_struct.pw_gid = getgid();
	char *gecos = getenv("GECOS");
	if(gecos)
		fake_pwd_struct.pw_gecos = gecos;
	char *home = getenv("HOME");
	if(home)
		fake_pwd_struct.pw_dir = home;
	char *shell = getenv("SHELL");
	if(shell)
		fake_pwd_struct.pw_shell = shell;
	fake_pwd_struct.pw_uid = uid;
	if(getenv("VERBOSE_LIBFAKE"))
		fprintf(stderr, "fake getpwuid: called from\n"
			"%s\n"
			"providing ->\n"
			"{\n"
			"	.pw_name = \"%s\",\n"
			"	.pw_passwd = \"%s\",\n"
			"	.pw_gid = \"%d\",\n"
			"	.pw_uid = \"%d\",\n"
			"	.pw_gecos = \"%s\",\n"
			"	.pw_dir = \"%s\",\n"
			"	.pw_shell = \"%s\",\n"
			"}\n",
			proccmd(),
			fake_pwd_struct.pw_name,
			fake_pwd_struct.pw_passwd,
			fake_pwd_struct.pw_gid,
			fake_pwd_struct.pw_uid,
			fake_pwd_struct.pw_gecos,
			fake_pwd_struct.pw_dir,
			fake_pwd_struct.pw_shell);
	return &fake_pwd_struct;
}

static char *
proccmd()
{
	static char cmd[1024];
	const size_t N = sizeof cmd / sizeof cmd[0];
	size_t p = snprintf(cmd, N, "[%d] ", getpid());
	if(p >= N)
		return cmd;
	FILE *f = fopen("/proc/self/cmdline", "r");
	if(!fgets(cmd+p, N-p, f))
		snprintf(cmd+p, N-p, "error in reading /proc/self/cmdline");
	fclose(f);
	return cmd;
}

#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>

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
	char *gid = getenv("GID");
	char *gecos = getenv("GECOS");
	char *home = getenv("HOME");
	char *shell = getenv("SHELL");
	if(fake_name)
		fake_pwd_struct.pw_name = fake_name;
	if(gid)
		fake_pwd_struct.pw_gid = atoi(gid);
	if(gecos)
		fake_pwd_struct.pw_gecos = gecos;
	if(home)
		fake_pwd_struct.pw_dir = home;
	if(shell)
		fake_pwd_struct.pw_shell = shell;
	fake_pwd_struct.pw_uid = uid;
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
	const size_t N = 1024;
	static char cmd[N];
	size_t p = snprintf(cmd, N, "[%d] ", getpid());
	if(p >= N)
		return cmd;
	FILE *f = fopen("/proc/self/cmdline", "r");
	if(!fgets(cmd+p, N-p, f))
		snprintf(cmd+p, N-p, "error in reading /proc/self/cmdline");
	fclose(f);
	return cmd;
}

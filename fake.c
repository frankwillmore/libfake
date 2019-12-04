#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

static char *proccmd(char *buf, size_t bufsize);

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

void
verbose(char *restrict name, struct passwd *pwd)
{
	char buffer[1024];
	if(getenv("VERBOSE_LIBFAKE"))
		fprintf(stderr, "fake %s: called from\n"
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
			name,
			proccmd(buffer, sizeof buffer / sizeof buffer[0]),
			pwd->pw_name,
			pwd->pw_passwd,
			pwd->pw_gid,
			pwd->pw_uid,
			pwd->pw_gecos,
			pwd->pw_dir,
			pwd->pw_shell);
}

static void
getpwuid_impl(uid_t uid, struct passwd *pwd)
{
	char *fake_name = getenv("LOGNAME");
	if(fake_name)
		pwd->pw_name = fake_name;
	else
		pwd->pw_name = fake_pwd_struct.pw_name;
	char *gid = getenv("GID");
	if(gid)
		pwd->pw_gid = atoi(gid);
	else
		pwd->pw_gid = getgid();
	char *gecos = getenv("GECOS");
	if(gecos)
		pwd->pw_gecos = gecos;
	else
		pwd->pw_gecos = fake_pwd_struct.pw_gecos;
	char *home = getenv("HOME");
	if(home)
		pwd->pw_dir = home;
	else
		pwd->pw_dir = fake_pwd_struct.pw_dir;
	char *shell = getenv("SHELL");
	if(shell)
		pwd->pw_shell = shell;
	else
		pwd->pw_shell = fake_pwd_struct.pw_shell;
	pwd->pw_uid = uid;
}

struct passwd *
getpwuid(uid_t uid)
{
	getpwuid_impl(uid, &fake_pwd_struct);
	verbose("getpwuid", &fake_pwd_struct);
	return &fake_pwd_struct;
}

int
getpwuid_r(uid_t uid, struct passwd *pwd, char *buffer,
	size_t bufsize, struct passwd **result)
{
	getpwuid_impl(uid, pwd);
	verbose("getpwuid_r", pwd);
	int s = snprintf(buffer, bufsize, "%s%c%s%c%s%c%s%c%s",
		pwd->pw_name, 0,
		pwd->pw_passwd, 0,
		pwd->pw_gecos, 0,
		pwd->pw_dir, 0,
		pwd->pw_shell);
	if(s < 0)
		return EIO;
	if((size_t)s >= bufsize)
		return ERANGE;
	pwd->pw_name = buffer; buffer += strlen(pwd->pw_name) + 1;
	pwd->pw_passwd = buffer; buffer += strlen(pwd->pw_passwd) + 1;
	pwd->pw_gecos = buffer; buffer += strlen(pwd->pw_gecos) + 1;
	pwd->pw_dir = buffer; buffer += strlen(pwd->pw_dir) + 1;
	pwd->pw_shell = buffer;
	*result = pwd;
	return 0;
}

static char *
proccmd(char *cmd, size_t N)
{
	size_t p = snprintf(cmd, N, "[%d] ", getpid());
	if(p >= N)
		return cmd;
	FILE *f = fopen("/proc/self/cmdline", "r");
	if(!fgets(cmd+p, N-p, f))
		snprintf(cmd+p, N-p, "error in reading /proc/self/cmdline");
	fclose(f);
	return cmd;
}

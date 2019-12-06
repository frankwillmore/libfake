#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <grp.h>
#include <assert.h>

#define MAX_MEMBERS_PER_GROUP 16384;
#define FAKE_MEMS_BUFFER_SIZE 65536;

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

/* A set of pointers into the names in the fake_mems_buffer */
static char* fake_mems[MAX_MEMBERS_PER_GROUP];

/* The values are obtained from the environment and should be treated as immutable, so we need to make a copy here. */
static char* fake_mems_buffer[FAKE_MEMS_BUFFER_SIZE];

static struct group fake_group_struct =
{
	.gr_name = "fake_name",
	.gr_passwd = "x",
	.gr_gid = -1,
	.gr_mem = fake_mems
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
//FTW: add bits for group 
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

/* FTW: Search the member string of CSV, 
	grab the addresses where each name starts, 
	and then flip the comma to a NULL so as to terminate each string */
static void extract_members_and_stamp_terminators(char* members[], char* buffer)
{
	/* Handle case of empty list */
	if ((*buffer) == '\0')
	{
		members[0] = NULL; 
		return;
	}

	members[0] = buffer;

	/* Walk the buffer, capture members, and flip commas to terminators */
	for 
	(
		char* p_char = buffer+1, int n_members=1;
		*p_char != '\0'; 
		p_char++
	) 
	if (*p_char == ',') 
	{  
		members[n_members++] = p_char;
		n_members++;
		assert(n_members < MAX_MEMBERS_PER_GROUP);
		*p_char = '\0'; 
	}
}

/* 	Populate fields of grp, except for member list */
static void
getgrgid_impl(gid_t gid, struct group *grp)
{
	char *group_name = getenv("GROUP_NAME");
	if(group_name)
		grp->gr_name = group_name;
	else
		grp->gr_name = fake_group_struct.gr_name;

	grp->gr_passwd = NULL; /* not a required field */

	grp->gr_gid = gid;

//	char* member;
//	char* rest = getenv("GROUP_MEMBERS");
//	while ((member = strtok_r(rest, ",", &rest))) printf("Got group member: %s\n", member); 
}

struct group *
getgrgid(gid_t gid)
{
	/* First copy group list from environment to our buffer */
	strncpy(getenv("GROUP_MEMBERS"), fake_mems_buffer, FAKE_MEMS_BUFFER_SIZE);
	/* If there are more members than room in the buffer, they're lost. */

	/* Then slice it into strings, grabbing pointers as we go */
	extract_members_and_stamp_terminators(fake_group_struct.gr_mem, fake_mems_buffer);

	getgrgid_impl(gid, &fake_group_struct);
//	verbose("getgrgid", &fake_group_struct);
	return &fake_group_struct;
}

int
getgrgid_r(gid_t gid, struct group *grp, char *buffer,
	size_t bufsize, struct group **result)
{
	getgrgid_impl(gid, grp);
//	verbose("getpwuid_r", pwd);

	/*	Get the group member list from environment. 
		Later, print to buffer and run the harvester against that buffer. 
	*/
	char* csv_members = getenv("GROUP_MEMBERS");

	int s = snprintf(buffer, bufsize, "%s%c%s%c%s%c%s",
		grp->gr_name, 0,
		grp->gr_passwd, 0,
		grp->gr_gid, 0,
		csv_members
		);
	if(s < 0)
		return EIO;
	if((size_t)s >= bufsize)
		return ERANGE;

// process the csv inside the buffer and grab the tokens
	grp->gr_name = buffer; buffer += strlen(pwd->pw_name) + 1;
	grp->gr_passwd = buffer; buffer += strlen(pwd->pw_passwd) + 1;
	grp->gr_gid = buffer; buffer += strlen(pwd->pw_dir) + 1;
	extract_members_and_stamp_terminators(fake_group_struct.gr_mem, buffer);
	
	*result = grp;
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

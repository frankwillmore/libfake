#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <grp.h>
//#include <unistd.h>

//#include <malloc.h>
//#include <errno.h>

static char* mems[256];

static struct group fake_group_struct =
{
        .gr_name = "fake_name",
        .gr_passwd = "*",
        .gr_gid = -1,
        .gr_mem = mems
};

int main()
{
    gid_t gid = getegid();
    printf("Got egid = %ld\n", (long)gid);

    printf("Running getgrgid() for gid %ld:\n", (long int)gid);

    struct group* getgrgid_result = getgrgid(gid);

    if (getgrgid_result != NULL) printf("Success for getgrgid(). Result = ->%lu<-\n", (unsigned long)getgrgid_result);
    
    return 0; 
}

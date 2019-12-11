#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <grp.h>
#include <unistd.h>
#include <malloc.h>

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

    printf("Attempting to run getgrgid_r() for gid %ld:\n", (long int)gid);

    size_t buffer_size = 65536;
    char* buf = (char*) malloc(buffer_size); 
    
    struct group* result;
    int retval = getgrgid_r(gid, &fake_group_struct, buf, buffer_size, &result);

    if (retval == 0) printf("Success for getgrgid_r().\n");

    /* dump the actual data */

    printf("got group name: %s\n", result->gr_name);
    printf("got group passwd: %s\n", result->gr_passwd);
    printf("got group gid: %ld\n", (long)result->gr_gid);
    printf("got group members: \n");

    char** p_members = result->gr_mem;
    while ((*p_members) != NULL) printf("\t%s\n", *p_members++);



    free(buf);
    return retval;
}


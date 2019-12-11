#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <grp.h>
#include <unistd.h>

int main()
{
    gid_t gid = getegid();
    printf("Got egid = %ld\n", (long)gid);

    printf("Running getgrgid() for gid %ld:\n", (long int)gid);

    struct group* getgrgid_result = getgrgid(gid);

    if (getgrgid_result != NULL)
    {
         printf("Success return code for getgrgid().\n");
    }
    
    printf("got group passwd: %s\n", getgrgid_result->gr_passwd);
    printf("got group gid: %ld\n", (long)getgrgid_result->gr_gid);
    printf("got group members: \n");
    char** p_members = getgrgid_result->gr_mem;
printf("FTW: %ld, %ld, %ld\n", p_members, *p_members, **p_members);
    while ((*p_members) != NULL) printf("\t%s\n", *p_members++);

    return 0;
}

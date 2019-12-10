#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <grp.h>
#include <unistd.h>

#include <malloc.h>
#include <errno.h>

//static char* mems[] = {"users", "boozers", "losers"};
static char* mems[256];

static struct group fake_group_struct =
{
        .gr_name = "fake_name",
        .gr_passwd = "*",
        .gr_gid = -1,
        .gr_mem = mems
};

//int main(int argc, char** argv)
int main()
{
    gid_t gid = getegid();
    printf("Got egid = %ld\n", (long)gid);
    //gid_t gid = (gid_t)1014;

    printf("Attempting to run getgrgid_r() for gid %ld:\n", (long int)gid);

    size_t buffer_size = 65536;
    char* buf = (char*) malloc(buffer_size); 
    
    struct group* result;
    int retval = getgrgid_r(gid, &fake_group_struct, buf, buffer_size, &result);

    if (retval == 0) printf("Success for getgrgid_r(). Result = ->%lu<-\n", (unsigned long)result);
    free(buf);

    sleep(1);

    printf("Running getgrgid() for gid %ld:\n", (long int)gid);

    struct group* getgrgid_result = getgrgid(gid);

    if (getgrgid_result != NULL) printf("Success for getgrgid(). Result = ->%lu<-\n", (unsigned long)getgrgid_result);
    
//    printf("Information retrieved: \n\n");

//    printf("fake_group_struct.gr_name: %s\n", fake_group_struct.gr_name);
//    printf("fake_group_struct.gr_passwd: %s\n", fake_group_struct.gr_passwd);
//    printf("fake_group_struct.gr_gid: %u\n", (unsigned)fake_group_struct.gr_gid);

//    printf("Group members:\n\n");

 //   for (int member_number = 0; fake_group_struct.gr_mem[member_number] != NULL; member_number++)
 //       printf("fake_group_struct.gr_mem[%d]: %s\n", member_number, fake_group_struct.gr_mem[member_number]);


    return 0; 
}

//    printf("result = %ld\n");
//struct group *getgrgid(gid_t gid);
//    char* token;
//    char* rest = argv[1];
    // Keep printing tokens while one of the 
    // delimiters present in str[]. 
//    while ((token = strtok_r(rest, ":", &rest))) 
//        printf("%s\n", token); 

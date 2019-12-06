#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <grp.h>

//static char* mems[] = {"users", "boozers", "losers"};
static char* mems[256];

static struct group fake_group_struct =
{
        .gr_name = "fake_name",
        .gr_passwd = NULL,
        .gr_gid = -1,
        .gr_mem = mems
};

int main(int argc, char** argv)
{
    //gid_t gid = getegid();
    gid_t gid = (gid_t)1014;
    printf("Got egid = %ld\n", gid);

    char buf[1024]; 
    
    struct group** result;
    int retval = getgrgid_r(gid, &fake_group_struct, buf, (size_t)1024, result);

    printf("retval = %d\n");
    printf("result = %ld\n");

//struct group *getgrgid(gid_t gid);

//    char* token;
//    char* rest = argv[1];
  
    // Keep printing tokens while one of the 
    // delimiters present in str[]. 

//    while ((token = strtok_r(rest, ":", &rest))) 
//        printf("%s\n", token); 

    printf("%s\n", fake_group_struct.gr_name);
    printf("%ld\n", fake_group_struct.gr_mem);
    printf("%s\n", fake_group_struct.gr_mem[0]);
//    printf("%s\n", fake_group_struct.gr_mem[1]);
//    printf("%s\n", fake_group_struct.gr_mem[2]);

    return 0; 
}

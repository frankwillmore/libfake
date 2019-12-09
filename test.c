#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <grp.h>

#include <errno.h>

//static char* mems[] = {"users", "boozers", "losers"};
static char* mems[256];

static struct group fake_group_struct =
{
        .gr_name = "fake_name",
        .gr_passwd = NULL,
        .gr_gid = -1,
        .gr_mem = mems
};

//int main(int argc, char** argv)
int main()
{
    //gid_t gid = getegid();
    //printf("Got egid = %ld\n", gid);
    gid_t gid = (gid_t)1014;
    printf("Using gid = %ld\n", gid);

    size_t buffer_size = 65536;
    char *buf =malloc(buffer_size); 
    
    struct group* result;
    int retval = getgrgid_r(gid, &fake_group_struct, buf, buffer_size, &result);

    printf("getgrgid_r retval = %d\n", retval);

    printf("fake_group_struct.gr_name: %s\n", fake_group_struct.gr_name);
    printf("fake_group_struct.gr_mem: %ld\n", fake_group_struct.gr_mem);
    printf("fake_group_struct.gr_mem[0]: %s\n", fake_group_struct.gr_mem[0]);
    printf("fake_group_struct.gr_mem[1]: %s\n", fake_group_struct.gr_mem[1]);
    printf("fake_group_struct.gr_mem[2]: %s\n", fake_group_struct.gr_mem[2]);

    free(buf);

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

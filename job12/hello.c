#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

//用于打印信息
void log_printf(char *fmt, ...)
{
    int silent = 0;
    if (silent)
        return;

    va_list ap;
    va_start(ap, fmt);

    write(1, "\033[32m", 5);
    vfprintf(stderr, fmt, ap);
    write(1, "\033[m", 3);

    va_end(ap);
}

char *root_path = "/";

char *hello_name = "hello";
char *hello_path = "/hello";
char *hello_content = "Hello!\n";

char *world_name = "world";
char *world_path = "/world";
char *world_content = "World!\n";

char *parent_name = "parent";
char *parent_path = "/parent";

char *john_name = "john";
char *john_path= "/parent/john";
char *john_content = "I am john!\n";

char *mike_name = "mike";
char *mike_path = "/parent/mike";
char *mike_content = "I am mike!\n";


//用于获取文件属性
int hello_getattr(const char *path, struct stat *p)
{
    log_printf("getattr %s\n", path);
    memset(p, 0, sizeof(struct stat));

    if (strcmp(path, root_path) == 0) {
        p->st_mode = S_IFDIR | 0755;
        p->st_nlink = 2;
        return 0;
    } 
    
    if (strcmp(path, hello_path) == 0) {
        p->st_mode = S_IFREG | 0444;
        p->st_nlink = 1;
        p->st_size = strlen(hello_content);
        return 0;
    } 

    if (strcmp(path, world_path) == 0) {
        p->st_mode = S_IFREG | 0444;
        p->st_nlink = 1;
        p->st_size = strlen(world_content);
        return 0;
    } 

    if (strcmp(path, parent_path) == 0) {
        p->st_mode = S_IFDIR | 0755;
        p->st_nlink = 2;
        return 0;
    } 
    if (strcmp(path, john_path) == 0) {
        p->st_mode = S_IFREG | 0444;
        p->st_nlink = 1;
        p->st_size = strlen(john_content);
        return 0;
    } 

    if (strcmp(path, mike_path) == 0) {
        p->st_mode = S_IFREG | 0444;
        p->st_nlink = 1;
        p->st_size = strlen(mike_content);
        return 0;
    } 

    return -ENOENT;
}

//填充目录项
int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                  off_t offset, struct fuse_file_info *fi)
{
    log_printf("readdir %s\n", path);

    if (strcmp(path, root_path) == 0) {
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);
        filler(buf, hello_name, NULL, 0);
        filler(buf, world_name, NULL, 0);
        filler(buf,parent_name,NULL,0);
        return 0;
    }

    if(strcmp(path,parent_path)==0){
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);
        filler(buf, john_name, NULL, 0);
        filler(buf, mike_name, NULL, 0);
        return 0;
    }

    return -ENOENT;
}

//打开文件
int hello_open(const char *path, struct fuse_file_info *fi)
{
    log_printf("open %s\n", path);

    if ((fi->flags & 3) != O_RDONLY)
        return -EACCES;

    if (strcmp(path, hello_path) == 0)
        return 0;

    if (strcmp(path, world_path) == 0)
        return 0;
    
    if(strcmp(path,john_path)==0)
        return 0;

    if(strcmp(path,mike_path)==0)
        return 0;
    return -ENOENT;
}

//比较
int min(int a, int b)
{
    return a < b ? a : b;
}

//读字符串
int read_string(char *string, off_t offset, char *buf, size_t size)
{
    size_t end = strlen(string);
    if (offset > end)
        return 0;

    size = min(end - offset, size);
    memcpy(buf, string + offset, size);
    return size;
}

int hello_read(const char *path, char *buf, size_t size, off_t offset,
               struct fuse_file_info *fi)
{
    log_printf("read %s %ld at %ld\n", path, size, offset);

    if (strcmp(path, hello_path) == 0)
        return read_string(hello_content, offset, buf, size);

    if (strcmp(path, world_path) == 0)
        return read_string(world_content, offset, buf, size);

    if(strcmp(path,john_path)==0){
        return read_string(john_content, offset, buf, size);
    }
    if(strcmp(path,mike_path)==0)
        return read_string(mike_content, offset, buf, size);

    return -ENOENT;
}

//创建回调函数
struct fuse_operations hello_ops = {
    .getattr    = hello_getattr,
    .readdir    = hello_readdir,
    .open       = hello_open,
    .read       = hello_read,
};

//调用fuse_main并传递参数
int fuse_start(char *program, char *mount_point)
{
    //参数列表
    char *argv[] = {
        program, "-s", "-f", "-o", "auto_unmount", 
        mount_point, NULL
    };

    //计算参数个数
    int argc = 0;
    while (argv[argc])
        argc++;

    //调用函数并传递回调函数
    return fuse_main(argc, argv, &hello_ops, NULL);
}

//主函数
int main(int argc, char *argv[])
{
    char *program = argv[0];
    char *mount_point = argv[1];
    //传递参数，调用fuse_start
    fuse_start(program, mount_point);
}

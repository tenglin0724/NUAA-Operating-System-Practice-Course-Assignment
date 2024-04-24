#ifndef _CMD_H
#define _CMD_H

//命令结构体
typedef struct {
    //命令名字
    char *name;
    //函数指针，指向实现该命令的函数
    void (*handler)(int argc, char *argv[]);
} cmd_t;

extern void usage();
extern void do_cmd(int argc, char *argv[]);

#endif

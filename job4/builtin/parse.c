#include<assert.h>
#include<stdio.h>
#include<string.h>
#include "parse.h"

void parse_cmd(char*line,struct cmd*cmd){
	cmd->argc=0;
	//使用strtok函数，将line拆分处成命令
	char*tok;
	const char delim[]=" ";
	tok=strtok(line,delim);
	//循环复制
	while(tok!=NULL){
		cmd->argv[cmd->argc++]=tok;
		tok=strtok(NULL,delim);
	}
	cmd->argv[cmd->argc]=NULL;
}

void dump_cmd(struct cmd *cmd)
{
    printf("argc = %d\n", cmd->argc);
    int i;
    for (i = 0; i < cmd->argc; i++) {
        printf("argv[%d] = (%s)\n", i, cmd->argv[i]);
    }
}
	
	

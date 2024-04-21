#include<assert.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parse.h"
#include<math.h>
char *getSub(char*source,int start,int len){
	int sl=strlen(source);
	char*ans=malloc(sl);
	int cnt=0;
	for(int i=start;i<start+len;i++){
		ans[cnt++]=source[i];
	}
	ans[cnt]='\0';
	return ans;
}

void parse_cmd(char*line,struct cmd*cmd){
	cmd->argc=0;
	cmd->input=NULL;
	cmd->output=NULL;
	//使用strtok函数，将line拆分处成命令
	char*tok;
	const char delim[]=" ";
	tok=strtok(line,delim);
	//循环复制
	while(tok!=NULL){
		if(tok[0]=='<'){
			int sl=strlen(tok);
			cmd->input=getSub(tok,1,sl-1);
			tok=strtok(NULL,delim);
		}else if(tok[0]=='>'){
			int sl=strlen(tok);
			cmd->output=getSub(tok,1,sl-1);
			tok=strtok(NULL,delim);
		}else{
			cmd->argv[cmd->argc++]=tok;
			tok=strtok(NULL,delim);
		}
	}
	cmd->argv[cmd->argc]=NULL;
}

void dump_cmp(struct cmd *cmd)
{
    printf("argc = %d\n", cmd->argc);
    int i;
    for (i = 0; i < cmd->argc; i++) {
        printf("argv[%d] = (%s)\n", i, cmd->argv[i]);
    }
    char *input = cmd->input ? cmd->input : "NULL";
    printf("input = (%s)\n", input);

    char *output = cmd->output ? cmd->output : "NULL";
    printf("output = (%s)\n", output);
}
	
	

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "cmd.h"

void exec_cmd(struct cmd *cmd)
{
    /* 完成此函数 */
    int error=execvp(cmd->argv[0],cmd->argv);
	if(error<0){
		perror("exec");
    		exit(0);
	}
}

int builtin_cmd(struct cmd*cmd){
	//实现cd命
	if(cmd->argc!=0&&strcmp(cmd->argv[0],"cd")==0){
		int ans=chdir(cmd->argv[1]);
		if(ans==-1){
			perror("cd error");
		}
		return 1;
	}
	else if(cmd->argc!=0&&strcmp(cmd->argv[0],"pwd")==0){
	//实现pwd命令
		char buf[1024];
		if(getcwd(buf,sizeof(buf))!=NULL){
			printf("%s\n",buf);		
		}else{
			perror("pwd error");
		}
		return 1;
	}
	else if(cmd->argc!=0&&strcmp(cmd->argv[0],"exit")==0){
	//实现exit命令
		pid_t pid=getpid();
		exit(pid);
		return 1;
	}
	return 0;
}

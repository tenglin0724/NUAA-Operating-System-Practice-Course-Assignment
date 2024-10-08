#include<assert.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include"cmd.h"
#include"parse.h"

//定义函数，用于读取一行内容
void read_line(char*line,int size){
	char *result=fgets(line,size,stdin);
	if(result==NULL){
		//读取失败，终止程序
		exit(0);
	}
	//取出最后的换行符
	int len=strlen(line);
	assert(line[len-1]=='\n');
	line[len-1]=0;
}
int main(int argc,char *argv[]){
	//循环
	while(1){
		char line[100];
		//将字符长度为2的字符串输出到屏幕设备
		write(1,"> ",2);
		read_line(line,sizeof(line));

		//定义结构体，用于将line转换成指令
		struct cmd cmd;
		parse_cmd(line,&cmd);
		
		//判断指令是否是内置的指令
		if(builtin_cmd(&cmd)){
			continue;
		}		

		//定义子进程
		pid_t pid =fork();
		if(pid==0){
			exec_cmd(&cmd);
		}
		//父进程等待子进程结束
		wait(NULL);
	}
	return 0;
}

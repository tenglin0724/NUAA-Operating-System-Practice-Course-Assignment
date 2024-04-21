#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
//手写切片函数
int getSpilt(char*source,char*target[],char aim){
	const int MAXS=256,len=strlen(source);
        int count=0,id=0;
        for(int i=0;i<len;i++){
                if(source[i]==' '){
                        int ml=i-count;
                        target[id]=malloc(MAXS);
                        strncpy(target[id],source+count,ml);
                        target[id][ml]='\0';
                        count=i+1;
                        id++;
                }
        }
        int ml=len-count;
        target[id]=malloc(MAXS);
        strncpy(target[id],source+count,ml);
        target[id][ml]='\0';
	return id+1;
}

void mysys(char*command){
	//将command按照空格分割成多个单词
	char *words[256];
	int num=getSpilt(command,words,' ');
	words[num]=NULL;
	//使用fork创建进程
	pid_t pid;
	pid=fork();
	if(pid==0){
		//子进程
		int error=execvp(words[0],(char * const *)words);
		if(error<0){
			perror("execlp");
		}
	}else{
		//父进程
		wait(NULL);
	}
}

int main(){
	//不要修改main函数
	puts("---");
	mysys("echo HELLO WORLD");
	puts("---");
	mysys("ls /");
	puts("---");
	return 0;
}

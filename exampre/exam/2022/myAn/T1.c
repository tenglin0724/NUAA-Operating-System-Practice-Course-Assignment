#include<stdlib.h>
int fd[2];

void child(){
	dup2(fd[0],0);
	close(fd[1]);
	char buf;
	while(read(0,buf,1)){
		if(buf=='z'){
			exit(0);
		}
		printf("%d: %c",getpid(),toupper(buf));
	}
}
void parent(){
	dup2(fd[1],1);
	close(fd[0]);

}

int main(){
	pipe(fd);	 
	int pid1=fork();
	if(pid1==0){
		child();
	}else{
		int pid2=fork();
		if(pid2==0){
			child();
		}else{
			
			puts("END");	
		}
	}
	return 0;
}

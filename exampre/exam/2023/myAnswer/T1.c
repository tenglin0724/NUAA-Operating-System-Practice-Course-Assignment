#include <stdio.h>
#include <unistd.h> 
#include<ctype.h>
#include <sys/wait.h>

int main(){
	int p[2],q[2];
	pipe(p);
	pipe(q);
	int pid=fork();
	char buf[5];
	if(pid==0){
		dup2(p[0],0);
		dup2(q[1],1);
		close(p[0]);
		close(p[1]);
		close(q[0]);
		close(q[1]);
		read(0,buf,4);
		for(int i=0;i<4;i++){
			buf[i]=toupper(buf[i]);
		}
		write(1,buf,4);	
	}else{
	dup2(p[1],1);
	dup2(q[0],0);
	close(p[0]);
	close(p[1]);
	close(q[1]);
	close(q[0]);
	write(1,"abc\n",4);
	wait(0);
	read(0,buf,4);
	write(2,buf,4);
	}
	return 0;
}

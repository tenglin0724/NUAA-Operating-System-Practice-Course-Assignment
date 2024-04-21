#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc,char *argv[]){
	//判别指令输入格式是否正确
	if(argc!=3){
		printf("<用法>：%s <源文件名> <目标文件>\n",argv[0]);
		return 0;
	}else{
		//正确后，打开文件
		int fd_source=open(argv[1],O_RDWR);
		if(fd_source<0){
			printf("<文件打开失败>:%s",argv[1]);
			return 0;
		}
		int fd_target=open(argv[2],O_RDWR|O_CREAT,0666);
		if(fd_target<0){
			printf("<文件打开失败>:%s",argv[2]);
			return 0;
		}

		//定义字符缓冲区
		const int bufSize=128;
		char *String=malloc(bufSize);
		//循环读取文件
		while(read(fd_source,String,bufSize)>0){
			//写入到目标文件中
			int yes=write(fd_target,String,bufSize);
			if(yes==-1){
				//提示写入错误
				printf("<文件写入错误>");
			}	
			String=malloc(bufSize);
		}
		//完成后，关闭文件
		close(fd_source);
		close(fd_target);
		return 0;
	}
}

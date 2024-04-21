#include<stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main(int argc,char*argv[]){
	//判断输入参数是否符合格式
	if(argc!=2){
		printf("用法： %s <文件名>\n", argv[0]);
		return 0;
	}
	//定义字节缓冲区
	const int bufSize=1024;
	char*String=malloc(bufSize);
	//打开文件
	int fd=open(argv[1],O_RDWR);
	if(fd<0){
		printf("文件打开失败！");
		return 0;
	}else{
		//使用read函数打开文件
		while(read(fd,String,bufSize)>0){
			printf("%s",String);
			String=malloc(bufSize);
		}
		//关闭文件
		close(fd);
		return 0;
	}
}

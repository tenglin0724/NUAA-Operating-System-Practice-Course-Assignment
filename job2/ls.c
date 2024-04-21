#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
//根据输入的权限数字，生成对应的字符串
void mode_to_str(int mode,char*mode_str,int mode_start){
	if(mode&1){
		mode_str[mode_start+2]='x';
	}
	if((mode>>1)&1){
		mode_str[mode_start+1]='w';
	}
	if((mode>>2)&1){
		mode_str[mode_start]='r';
	}
}
//查看文件权限
char* stat_file(char*filePath){
	//定义变量
	struct stat file_stat;
	char*mode_str=malloc(9);
	//赋给mode_str初始值'-'
	for(int i=0;i<9;i++){
		mode_str[i]='-';
	}
	//获取权限
	if(stat(filePath,&file_stat)==-1){
		perror("<获取文件权限失败>");
	}else{
		int file_mode=file_stat.st_mode&0777;
		int i=6;
		while(file_mode){
			int wei=file_mode&07;
			mode_to_str(wei,mode_str,i);
			file_mode>>=3;
			i-=3;
		}
	}
	return mode_str;
}

//遍历所有文件
void list_dir(char *path,int is_l)
{
	//定义变量
    	DIR *dir = opendir(path);
    	struct dirent *entry;
    	//循环获取每一个文件节点
	while (entry = readdir(dir)) {
		//如果是'.'，跳过
        	if (strcmp(entry->d_name, ".") == 0){ 
            		continue;
		}
		//如果是'..',跳过
		else if (strcmp(entry->d_name, "..") == 0) {
            		continue;
		}
       		//如果是目录，打印名字
		else if (entry->d_type == DT_DIR){ 
			char *temp=malloc(256);
			strcpy(temp,path);
			strcat(temp,"/");
			strcat(temp,entry->d_name);
			if(is_l){
				char *mode_str=stat_file(temp);
				printf("%s %s\n",mode_str,entry->d_name);
			}
			else{
				printf("%s\n",entry->d_name);
			}
		}
		//如果是文件，打印名字；-l模式下还要打印文件权限
		else {
                       char *temp=malloc(256);
                       strcpy(temp,path);
		       strcat(temp,"/");
                       strcat(temp,entry->d_name);
		       if(is_l){
		       	char*mode_str=stat_file(temp);	       
           		printf("%s %s\n",mode_str,entry->d_name);
		       }else{
		      	printf("%s\n",entry->d_name);
		       }
		}
    	}
	//关闭目录变量
    	closedir(dir);
}

//主函数
int main(int argc,char*argv[]){
	//判断输入的命令是否符合格式
	if(argc>=3){
		printf("<用法>\n<列出当前目录下所有文件和目录的名字>：%s\n<列出当前目录下所有文件和目录的名字和权限>：%s -l\n<列出‘/’目录下所有文件和目录>：%s /\n",argv[0],argv[0],argv[0]);
	}else{
		if(argc==2&&strcmp(argv[1],"-l")!=0&&strcmp(argv[1],"/")!=0){
			printf("<用法>\n<列出当前目录下所有文件和目录的名字>：%s\n <列出当前目录下所有文件和目录的名字和权限>：%s -l\n<列出‘/’目录下所有文件和目录>：%s /\n",argv[0],argv[0],argv[0]);
		}else if(argc==2){
			//表示为-l或者/参数
			if(strcmp(argv[1],"-l")==0){
				list_dir(".",1);
			}else{
				list_dir("/",0);
			}
		}else{
			//表示基础功能
			list_dir(".",0);
		}
	}
	return 0;
}

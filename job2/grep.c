#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
//对文件内容进行查找
void search_file(char*path,char*neddle,int is_r){
	//(1)打开文件
	FILE *fd=fopen(path,"r");
        if(fd==NULL){
		printf("打开文件失败！\n");
		exit(0);
	}
       	char *line=malloc(256);
        //(2)按行读取
        while(fgets(line,256,fd)){
		int size=strlen(line);
		if(line[size-1]=='\n'){
			line[size-1]='\0';
		}
		//(3)进行匹配
              if(strstr(line,neddle)!=NULL){
		      if(is_r==1){
                     		printf("%s:%s\n",path,line);
		      }else{
		      		printf("%s\n",line);
		      }
              }
	      line=malloc(256);
         }
        //(4)关闭文件
        fclose(fd);

}

//递归寻找所有文件
void search_dir(char *path,char *neddle,int is_r)
{
	//定义变量
    	DIR *dir = opendir(path);
   	struct dirent *entry;
    	//循环
	while (entry = readdir(dir)) {
        	if (strcmp(entry->d_name, ".") == 0){	
           	//跳过
			continue;
		}
       		else if (strcmp(entry->d_name, "..") == 0){
		//跳过
			continue;
		}else if (entry->d_type == DT_DIR){	
            	//遇到目录，看参数是否有-r。若有，递归寻找；没有跳过。
			if(!is_r){
				continue;
			}
			//得到新path
			char *temp=malloc(256);
			strcpy(temp,path);
			int slen=strlen(path);
			if(path[slen-1]!='/'){
				strcat(temp,"/");
			}
			strcat(temp,entry->d_name);
			search_dir(temp,neddle,is_r);
		}else if (entry->d_type == DT_REG){ 
		//遇到文件
			//得到文件的全路径
			char *temp=malloc(256);
			strcpy(temp,path);
			int slen=strlen(path);
			if(path[slen-1]!='/'){
				strcat(temp,"/");
			}	
			strcat(temp,entry->d_name);
			//对文件进行查找
			search_file(temp,neddle,is_r);
		}
   	 }
	//关闭dir
    	closedir(dir);
}
int main(int argc,char*argv[]){
	//指令格式检查
	if(argc==3){
		//基础用法
		search_file(argv[2],argv[1],0);		
	}else if(argc==4&&strcmp(argv[1],"-r")==0){
		//表示可以查找目录
		search_dir(argv[3],argv[2],1);
	}else{
		printf("<用法>\n<查找文件中的匹配行>：%s <被查找字符串> <文件名>\n<查找目录下所有文件中的匹配行>：%s -r <被查找字符串> <目录名>\n",argv[0],argv[0]);
	}
	return 0;
}

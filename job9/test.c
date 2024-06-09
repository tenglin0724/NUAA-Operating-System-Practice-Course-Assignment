#include "std.h"
#include "tcp.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
void read_path(int fd,char *path)
{
//      char buff[10000];
    FILE *fr = fdopen(fd, "r");

    char line[10000];
    fgets(line,sizeof(line), fr);
        printf("%s\n",line);
        int cnt=1;
    char *token;
        token=strtok(line," ");
        while(token!=NULL)
        {
                if(cnt==2)
                {
                        strcpy(path,token);
                        break;
                }
                cnt++;
                token=strtok(NULL," ");

        }
        path[strlen(path)]='\0';
}
void read_pipe(int fd,char*content)
{
        int count=read(fd,content,10000);
        content[count]=0;

}
void read_file(char *content,char *path)
{
        printf("%s",path);
        if(strstr(path,"app")==NULL)//静态处理文件
        {
                int file=open(path,O_RDONLY);
                if(file<0)
                {
                        printf("error\n");
                }
                int count=read(file,content,10000);
                content[count]=0;
        }
        else //动态处理文件
        {
        char *ppath;
                char *fname;
                char *fpath=NULL;
                char *show_fname,*envv_temp,*envv;
                ppath=(char*)malloc(sizeof(char)*(strlen(path)+1));
                strcpy(ppath,path);
                fname=strtok(ppath,"/");
                fname=strtok(NULL,"/");
                int flag=0;
                if(strstr(fname,"?")==NULL)
                {
                        flag=1;
                        fpath=(char*)malloc(sizeof(char)*(strlen(fname)+1));
                        sprintf(fpath,"./%s",fname);
                }
                else
                {
                //      char *show_fname,*envv_temp,*envv;
                        show_fname=(char*)malloc(sizeof(char)*(strlen(fname)+1));
                        envv=(char*)malloc(sizeof(char)*(strlen(fname)+100));
                        envv_temp=(char*)malloc(sizeof(char)*(strlen(fname)+10));
                        show_fname=strtok(fname,"?");
                        envv_temp=strtok(NULL,"?");
                        sprintf(fpath,"./%s",show_fname);
                        sprintf(envv,"QUERY_STRING=%s",envv_temp);
                        printf("%sdd",fpath);
                        printf("%s---",envv);
                        flag =2;

                }

//              char *fpath;
//              fpath=(char*)malloc(sizeof(char)*(strlen(fname)+1));
//              sprintf(fpath,"./%s",fname);
                int fd[2];
                pipe(fd);
                pid_t pid=fork();
                if(pid==0)
                {
                        dup2(fd[1],1);
                        close(fd[0]);
                        close(fd[1]);
                        chdir("app");
                        if(flag==1)
                        execlp(fpath,fname,NULL);
                        else if(flag ==2)
                        {
                                char *argv[]={fpath,NULL};
                                char *env[]={envv,NULL};
                                execve(fpath,argv,env);
                        }
                }
                wait(NULL);
                close(fd[1]);
                read_pipe(fd[0],content);
                //读管道
                close(fd[0]);
        }
}
void read_dir(char*content,char * path)
{//只需要显示当前目录下的文件和目录，递归的话，网页点击+重新执行该函数
        DIR *dir = opendir(path);
    struct dirent *entry;
        int len=0;
        sprintf(content+len,"<h1>directory listing for %s</h1><hr><ul>\n",path);
        len+=40+strlen(path);
    while (entry = readdir(dir)) {
        if (strcmp(entry->d_name, ".") == 0)
            continue;

                else if (strcmp(entry->d_name, "..") == 0)
            continue;

                else
                {
                        char *child=(char*)malloc(sizeof(char)*(strlen(path)+200));
                        sprintf(child,"%s/%s",path,entry->d_name);
                        sprintf(content+len,"<li><a href=\"/%s\">%s</a></li>\n",child,entry->d_name);
                        len+=(26+strlen(child)+strlen(entry->d_name));
                }

    }
sprintf(content+len,"</ul><hr>\n");
    closedir(dir);
}

char *get_file_ch_last2(char *filename,char ch) {
    char *dot = strchr(filename, ch);
    if (!dot || dot == filename) return NULL;
    return dot + 1;
}
void http_handler(int fd)
{
    FILE *fw = fdopen(fd, "w");
    char content[10000];
        char path[10000];
        read_path(fd,path);
        printf("--------%s\n",path);
        if(strcmp(path,"/")==0)
        {
                read_file(content,"index.html");
        }
        else{
                puts("ii");
                printf("======%s\n",path);
                char* ppath=get_file_ch_last2(path,'/');
                printf("%s====",ppath);
                if(strstr(path,"?")!=NULL)
                {
                        printf("99");
                        printf("%s===",ppath);
                        read_file(content,ppath);
                        memset(ppath,0,sizeof(ppath));
                        memset(path,0,sizeof(path));
                }
                else
                {
                        char p[20000];
                        sprintf(p,"./%s",ppath);
                        struct stat st;
                //判断路径是文件还是目录
                        if(lstat(p,&st)==-1)
                        {
                                memset(ppath,0,sizeof(ppath));
                                memset(content,0,sizeof(content));
                                perror("stat");
                                return ;
                        }
                        if(S_ISREG(st.st_mode))
                        read_file(content,ppath) ;
                        else if(S_ISDIR(st.st_mode))
                        read_dir(content,ppath);
                        else
                        puts("未知文件错误！");
                        memset(ppath,0,sizeof(ppath));
                        memset(path,0,sizeof(path));
                }
        //              memset(content,0,sizeof(content));
        }
    int content_length = strlen(content);

    fprintf(fw, "HTTP/1.0 200 OK\r\n");
    fprintf(fw, "Server: tiny httpd\r\n");
    fprintf(fw, "Content-type: text/html\r\n");
    fprintf(fw, "Content-length: %d\r\n", content_length);
    fprintf(fw, "Connection: close\r\n");
    fprintf(fw, "\r\n");
    fwrite(content, content_length, 1, fw);
    fflush(fw);
        memset(path,0,sizeof(path));
        memset(content,0,sizeof(content));
}

int main(int argc, char *argv[])
{
    char *ip_address = "127.0.0.1";
    int port = 8080;


        chdir("www");
    int server_fd = tcp_listen(ip_address, port);
    while (1) {
        int client_fd = tcp_accept(server_fd);
                if(client_fd<0)
                        fatal("accept");
    //       pid_t pid=fork();
   //          if(pid==0)
  //           {                       close(server_fd);
                        http_handler(client_fd);
                        close(client_fd);
  //           }
 ///            close(client_fd);
  }
//      wait(NULL);
//      close(server_fd);
    return 0;
}
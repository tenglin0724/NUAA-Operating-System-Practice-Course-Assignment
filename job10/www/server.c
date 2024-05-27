#include "std.h"
#include "tcp.h"

void http_read_req(int fd)
{
    FILE *fr = fdopen(fd, "r");

    char line[100];
    fgets(line, 100, fr);
    printf("%s", line);
}

//读取html文件
char *get_file_content(char*path){
    FILE *file = fopen(path, "rb");
    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
    
        char *buffer = (char*)malloc(fileSize * sizeof(char));
        if (buffer != NULL) {
            fread(buffer, sizeof(char), fileSize, file);
        }
        fclose(file);
        return buffer;
    }else{
        return NULL;
    }
}

//获取请求路径
char*get_path(char*buff,int length){
    //从buff中获取要截取的字符串起始地址和长度
    int start=0,leng=0;
    for(int i=0;i<length;i++){
        if(buff[i]=='/'){
            start=i;
            break;
        }
    }
    for(int i=start;i<length;i++){
        if(buff[i]==' '){
            break;
        }else{
            leng++;
        }
    }
    //根据以上获取的信息，截取字符串
    char*ans=(char*)malloc(sizeof(char)*(leng));
    for(int i=start;i-start<leng;i++){
        ans[i-start]=buff[i];
    }
    ans[leng]='\0';
    return ans;
}


//进行http处理
void http_handler(int fd,char*html_content)
{
    FILE *fw = fdopen(fd, "w");

    char *content = html_content;
    int content_length = strlen(content);                                           //显示的html的内容
    
    fprintf(fw, "HTTP/1.0 200 OK\r\n");
    fprintf(fw, "Server: tiny httpd\r\n");
    fprintf(fw, "Content-type: text/html\r\n");
    fprintf(fw, "Content-length: %d\r\n", content_length);
    fprintf(fw, "Connection: close\r\n");
    fprintf(fw, "\r\n");
    fwrite(content, content_length, 1, fw);                                        //输出到客户端
    fflush(fw);
}

//处理文件
void http_reg(int fd,char*path){
    http_handler(fd,get_file_content(path));
}

//遍历文件或文件夹
void http_dir(int fd,char *path)
{
    char*content=(char*)malloc(sizeof(char)*10000);                 //定义要显示的html内容
    int start=0;
    sprintf(content+start,"<h1>Directory listing for %s/</h1><hr><ul>",path);
    start+=(strlen(path)+40);

    DIR *dir = opendir(path);
    struct dirent *entry;
    while (entry = readdir(dir)) {
        if (strcmp(entry->d_name, ".") == 0){
            continue;
        }else if (strcmp(entry->d_name, "..") == 0){ 
            continue;
        }else {

            char*son_path=(char*)malloc(sizeof(char)*(strlen(path)+100));
            sprintf(son_path,"%s/%s",path,entry->d_name);                       //获取子文件的目录

            sprintf(content+start,"<li><a href=\"%s\">%s</a></li>",son_path,entry->d_name);
            start+=(24+strlen(entry->d_name)+strlen(son_path));
        }
    }
    sprintf(content+start,"</ul><hr>");
    http_handler(fd,content);                                                   //发送给浏览器渲染
    closedir(dir);
}


//处理客户端的请求
void path_handler(int fd)
{
    //从客户端读取请求
    char buff[128];
    int count = read(fd, buff, sizeof(buff));
    buff[count] = 0;


    //识别请求路径
    char*file_path=get_path(buff,count);
    printf("server path: %s  %ld\n",file_path,strlen(file_path));

    //根据路径判别
    if(strcmp(file_path,"/")==0){
        http_reg(fd,"./index.html");                                    //index页面
    }else if(strlen(file_path)!=0){                                     //其他

        char*path=(char*)malloc(sizeof(char)*(strlen(file_path)+1));
        sprintf(path,".%s",file_path);

        struct stat st;
        if(lstat(path, &st)==-1){                                  //获取文件状态
            fatal("文件路径错误！");
        }                                         
        if (S_ISDIR(st.st_mode)) {                                      //是目录
            http_dir(fd,path);
        }else if(S_ISREG(st.st_mode)){                                  //是文件
            http_reg(fd,path);
        }else{
            fatal("文件错误！");
        }
    }else{

    }
}

//服务端要做的处理
void *path_entry(void *arg)
{
    int fd = (long) arg;
    path_handler(fd);
    close(fd);
    return NULL;
}

//监听
void run_server(char *ip_addr, int port)
{

    int server_fd = tcp_listen(ip_addr, port);
    while (1) {
        int client_fd = tcp_accept(server_fd);
         if (client_fd < 0)
            fatal("accept");
        printf("accept client\n");
        pthread_t echo_thread;
        pthread_create(&echo_thread, NULL, path_entry, (void *)(long)client_fd);
    }
    close(server_fd);
}

//主函数
int main(int argc, char *argv[])
{
    char *ip_addr = "0.0.0.0";
    int port = 1234;
    run_server(ip_addr, port);
    return 0;
}
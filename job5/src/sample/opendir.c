#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
//打印目录项的完整路径
void list_dir(char *path)
{
    DIR *dir = opendir(path);
    struct dirent *entry;
    while (entry = readdir(dir)) {
        if (strcmp(entry->d_name, ".") == 0)
            continue;

        if (strcmp(entry->d_name, "..") == 0)
            continue;

        char sub_path[256];
        if (strcmp(path, ".") == 0)
            strcpy(sub_path, entry->d_name);
        else
            sprintf(sub_path, "%s/%s", path, entry->d_name);
        printf("%s\n", sub_path);
    }
    closedir(dir);
}

int main(int argc, char *argv[])
{
    char *path = argv[1];
    list_dir(path);
    return 0;
}

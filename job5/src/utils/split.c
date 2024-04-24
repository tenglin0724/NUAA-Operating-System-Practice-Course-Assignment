#include <string.h>

/* input string will be destroyed */
//返回分割后的字符数组word_table长度
int split_string(char *string, char *delim, char *word_table[])
{
    char *word;
    int word_count = 0;
    //传入目标分割字符串和分割符之后，返回分割字符串数组指针
    word = strtok(string, delim);
    while (word) {
        word_table[word_count++] = word;
        //得到下一个字符串
        word = strtok(NULL, delim);
    }
    return word_count;
}

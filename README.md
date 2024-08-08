# NUAA操作系统实践课程作业简介
## Job1
+ 提交的目录结构
  ```
  nuaaos
  └── job1
      ├── echo.c
      ├── hello.c
      └── parent
          └── child
              ├── a
              ├── b
              └── c
  ```
+ 作业内容
  - 创建如上所示的目录和文件
  - 实现hello.c
      ```shell
      $ cc -o hello hello.c
      $ ./hello
      hello
      ```
  - 实现echo.c
    ```
    $ cc -o echo echo.c
    $ ./echo hello
    hello
    $ ./echo hello world
    hello world
    ```

## Job2
+ 提交的目录结构
  ```
  nuaaos
  └── job2
      ├── cat.c
      ├── cp.c
      ├── grep.c
      ├── ls.c
      ├── test.py
      └── test.sh
  ```
+ 作业内容
  - 创建如上所示的目录和文件
  - 实现test.sh
    ```shell
    $ chmod +x test.sh
    $ ./test.sh
    hello
    world
    ```
  - 实现test.py
    ```shell
    $ chmod +x test.py
    $ ./test.py
    hello
    world
    ```
  - 实现cat.c
    ```shell
    $ cc -o cat cat.c
    $ ./cat cat.c
    #include <stdio.h>
    #include <unistd.h>
    
    int main()
    {
        return 0;
    }
    ```
  - 实现cp.c
    ```shell
    $ cc -o cp cp.c
    $ ./cp cp.c cp.bak
    $ cat cp.bak
    #include <stdio.h>
    #include <unistd.h>
    
    int main()
    {
        return 0;
    }
    ```
  - 实现ls.c
    ```shell
    $ cc -o ls ls.c
    $ touch file0 file1 file2
    $ ./ls
    file0
    file1
    file2
    ```
  - 实现grep.c
    ```shell
    $ cc -o grep grep.c
    $ ./grep main grep.c
    int main(int argc, char *argv[])
    $ grep -r main nuaaos
    nuaaos/job1/hello.c:int main()
    nuaaos/job1/echo.c:int main()
    nuaaos/job2/cat.c:int main()
    nuaaos/job2/ls.c:int main()
    nuaaos/job2/cp.c:int main()
    nuaaos/job2/grep.c:int main()
    ```
## Job3
+ 提交的目录结构
  ```
  nuaaos
  └── job3
      ├── mysys.c
      └── project
          ├── Makefile
          ├── main.c
          ├── max.c
          ├── max.h
          ├── min.c
          └── min.h
  ```
+ 作业内容
  - 创建如上所示的目录和文件
  - 编写Makefile
    ```shell
    $ wget https://www.nuaalab.cn/project.tgz
    $ tar zxvf project.tgz
    project/
    project/min.h
    project/min.c
    project/max.h
    project/main.c
    project/max.c
    $ cd project
    $ make
    cc -c min.c
    cc -c max.c
    cc -c main.c
    cc -o app main.o min.o max.o
    $ ./app
    min = 1
    max = 2
    ```
  - 实现mysys.c

## Job4&5
+ 提交的目录结构
  ```
  + nuaaos
  + job4
    - basis
      * Makefile
      * main.c
      * ...
    - builtin
      * Makefile
      * main.c
      * ...
    - io
      * Makefile
      * main.c
      * ...
    - pipe
      * Makefile
      * main.c
      * ...
  ```
+ 作业内容
  - 创建如上所示的目录和文件
  - 实现[讲义](https://www.nuaalab.cn/tmp/sh/)第1小结内容
  - 实现[讲义](https://www.nuaalab.cn/tmp/sh/)第2小结内容
  - 实现[讲义](https://www.nuaalab.cn/tmp/sh/)第3小结内容
  - 实现[讲义](https://www.nuaalab.cn/tmp/sh/)第4小结内容
## Job6
+ 提交的目录结构
  ```
  nuaaos
  └── job6
      └── src
          ├── blob.c
          ├── blob.h
          ├── cmd.c
          ├── cmd.h
          ├── commit.c
          ├── commit.h
          ├── config.c
          ├── config.h
          ├── head.c
          ├── head.h
          ├── index.c
          ├── index.h
          ├── main.c
          ├── Makefile
          ├── oid.c
          ├── oid.h
          ├── tree.c
          ├── tree.h
          └── utils
              ├── dir.c
              ├── dir.h
              ├── file.c
              ├── file.h
              ├── list.c
              ├── list.h
              ├── split.c
              ├── split.h
              ├── std.c
              └── std.h
  ```
+ 作业内容
  - 创建如上所示的目录和文件
  - 实现git的简单功能
    ```shell
    $ cd /home/guest/nuaaos/job6/src
    $ make
    $ pwd
    /home/guest/nuaaos/job6/src
    $ PATH=/home/guest/nuaaos/job6/src:$PATH
    $ dg
    DeltaGit Usage
      dg init
      dg cat-file -t oid      打印类型
      dg cat-file -s oid      打印大小
      dg cat-file -p oid      打印内容
      dg add file             把文件加入到索引区
      dg add dir              把目录下的文件加入到索引区
      dg rm file              从索引区中删除文件
      dg rm dir               从索引区中删除目录下的文件
    
      dg commit -m msg
      dg checkout oid
      dg log
    $ mkdir prj
    $ cd prj
    $ dg init
    Initialized empty Git repository
    $ cat main.c
    int main()
    {
    }
    $ dg add main.c
    add main.c
    $ dg commit -m v0
    commit 556c8e1e1db54f4036bbb4f5224d6beec7aefaf0 v0
    $ cat util.c
    int max(int a, int b)
    {
        return a > b ? a : b;
    }
    $ dg add util.c
    add util.c
    $ dg commit -m v1
    commit a6f85a19dceebe59435878b3c606376b2b4566d1 v1
    $ dg log
    commit a6f85a19dceebe59435878b3c606376b2b4566d1
    time   Sun May 12 10:35:40 2024
    msg    v1
    
    commit 556c8e1e1db54f4036bbb4f5224d6beec7aefaf0
    time   Sun May 12 10:35:12 2024
    msg    v0
    $ dg checkout 556c
    unlnk util.c
    unlnk main.c
    creat main.c
    $ ls
    main.c
    $ cat main.c
    int main()
    {
    }
    $ dg checkout 61d3
    unlnk main.c
    creat main.c
    creat util.c
    $ cat main.c
    int main()
    {
    }
    $ cat util.c
    int max(int a, int b)
    {
        return a > b ? a : b;
    }
    ```
## Job7
+ 提交的目录结构
  ```
  nuaaos
  └── job7
      ├── pi1.c
      ├── pi2.c
      └── sort.c
  ```
+ 作业内容
  - 创建如上所示的目录和文件
  - 实现pi1.c
    * 莱布尼兹级数公式: 1 - 1/3 + 1/5 - 1/7 + 1/9 - ... = PI/4
    * 主线程创建 1 个辅助线程
    * 主线程计算级数的前半部分
    * 辅助线程计算级数的后半部分
    * 主线程等待辅助线程运行結束后,将前半部分和后半部分相加
  - 实现pi2.c
    * 与上一题类似，但本题更加通用化，能适应 N 个核心
    * 主线程创建 N 个辅助线程
    * 每个辅助线程计算一部分任务，并将结果返回
    * 主线程等待 N 个辅助线程运行结束，将所有辅助线程的结果累加
    * 本题要求 1: 使用线程参数，消除程序中的代码重复
    * 本题要求 2: 不能使用全局变量存储线程返回值
  - 实现sort.c
    * 主线程创建两个辅助线程
    * 辅助线程1使用选择排序算法对数组的前半部分排序
    * 辅助线程2使用选择排序算法对数组的后半部分排序
    * 主线程等待辅助线程运行結束后, 使用归并排序算法归并子线程的计算结果
    * 本题要求 1: 使用线程参数，消除程序中的代码重复
    * 本题要求 2: 不能使用全局变量存储线程返回值
## Job8
+ 提交的目录结构
  ```
  nuaaos
  └── job8
      ├── pc1.c
      ├── pc2.c
      └── pgrep.c
  ```
+ 作业内容
  - 创建如上所示的目录和文件
  - 实现pc1.c
    * 系统中有3个线程：生产者、计算者、消费者
    * 系统中有2个容量为4的缓冲区：buffer1、buffer2
    * 生产者生产'a'、'b'、'c'、‘d'、'e'、'f'、'g'、'h'八个字符，放入到buffer1
    * 计算者从buffer1取出字符，将小写字符转换为大写字符，放入到buffer2
    * 消费者从buffer2取出字符，将其打印到屏幕上
  - 实现pc2.c
    * 功能和前面的实验相同，使用信号量解决
  - 实现grep.c
    * 实现[并行的 grep ](https://www.nuaalab.cn/tmp/pgrep)功能
## Job9&10
+ 提交的目录结构
  ```
  nuaaos
  └── job9
      ├── 源文件 1
      ├── 源文件 2
      ├── 源文件 3
      ├── ...
      ├── www
      └── Makefile
  ```
+ 作业内容
  - 创建如上所示的目录和文件
  - 实现首页
    ```shell
    $ cd job9
    $ make
    $ ls httpd
    httpd
    $ ./httpd &
    $ wget http://localhost:8080/index.html
    --2024-06-02 10:16:58--  http://localhost:8080/index.html
    正在解析主机 localhost (localhost)... 127.0.0.1
    正在连接 localhost (localhost)|127.0.0.1|:8080... 已连接。
    已发出 HTTP 请求，正在等待回应... 200 OK
    长度： 未指定 [text/html]
    正在保存至: “index.html”
    
    index.html             [ <=>            ]     579  --.-KB/s    用时 0s
    
    2024-06-02 10:16:58 (8.94 MB/s) - “index.html” 已保存 [579]
    $ ls index.html
    index.html
    $ diff index.html www/index.html
    $ 
    ```
  - 实现目录
    ```shell
    $ wget http://localhost:8080/dir
    guest@linux:~/nuaalab/courses/os/job9$ wget http://localhost:8080/dir
    --2024-06-02 10:25:10--  http://localhost:8080/dir
    Resolving localhost (localhost)... 127.0.0.1
    Connecting to localhost (localhost)|127.0.0.1|:8080... connected.
    HTTP request sent, awaiting response... 200 OK
    Length: unspecified [text/html]
    Saving to: ‘dir’
    
    dir                   [ <=>          ]     226  --.-KB/s    in 0s
    
    2024-06-02 10:25:10 (1.69 MB/s) - ‘dir’ saved [226]
    $ ls dir
    dir
    $ cat dir
    <html>
    <head><title>Index of /dir</title></head>
    <body>
    <h1>Index of /dir</h1>
    <hr>
    <a href="/dir/hello.html">hello.html</a><br>
    <a href="/dir/world.html">world.html</a><br>
    <hr>
    </body>
    </html>
    $ 
    ```
  - 实现动态页面
    ```shell
    $ wget http://localhost:8080/app/now.sh
    --2024-06-02 10:36:12--  http://localhost:8080/app/now.sh
    Resolving localhost (localhost)... 127.0.0.1
    Connecting to localhost (localhost)|127.0.0.1|:8080... connected.
    HTTP request sent, awaiting response... 200 OK
    Length: unspecified [text/html]
    Saving to: ‘now.sh’
    
    now.sh                [ <=>          ]      60  --.-KB/s    in 0s
    
    2024-06-02 10:36:12 (761 KB/s) - ‘now.sh’ saved [60]
    $ ls now.sh
    now.sh
    $ cat now.sh
    <h1>Current time</h1>
    <b>
    Sun Jun  2 10:36:12 CST 2024
    </b>
    $ 
    ```
## Job11
+ 提交的目录结构
  ```
  nuaaos
  └── job11
      ├── Makefile
      ├── pc.c
      ├── pc.py
      ├── pfc.c
      ├── pfc.py
      └── lib 
          ├── stdio.c
          ├── cpu.h
          ├── cpu.S
          ├── coro.h
          └── coro.c
  ```
+ 作业内容
  - 创建如上所示的目录和文件
  - 利用协作式多任务完成生产者消费者
    ```output
        CONSUME
    PRODUCE
    produce 0
        consume 0
    produce 1
        consume 1
    produce 2
        consume 2
    produce 3
        consume 3
    produce 4
        consume 4
    produce 5
        consume 5
    produce 6
        consume 6
    produce 7
        consume 7
    ```
  - 利用协作式多任务完成生产者计算者消费者
    ```output
            CONSUME
        FILTER
    PRODUCE
    produce 0
        filter 0 -> 0
            consume 0
    produce 1
        filter 1 -> 10
            consume 10
    produce 2
        filter 2 -> 20
            consume 20
    produce 3
        filter 3 -> 30
            consume 30
    produce 4
        filter 4 -> 40
            consume 40
    produce 5
        filter 5 -> 50
            consume 50
    produce 6
        filter 6 -> 60
            consume 60
    produce 7
        filter 7 -> 70
            consume 70
    END
    ```
## Job12
+ 提交的目录结构
  ```
  nuaaos
  └── job12
      ├── hello.c
      └── Makefile
  ```
+ 作业内容
  - 创建如上所示的目录和文件
  - 基于 FUSE 实现文件系统的读取
    ```shell
    $ cd job12
    $ make
    $ ls
    hello hello.c Makefile
    $ mkdir mnt
    $ ./hello mnt &
    $ cat mnt/hello
    getattr /hello
    open /hello
    read /hello 4096 at 0
    Hello!
    $ cat mnt/parent/mike
    getattr /parent
    getattr /parent/mike
    open /parent/mike
    read /parent/mike 4096 at 0
    I am mike!
    $
    ```
## Job13
+ 提交的目录结构
  ```
  nuaaos
  └── job13
      ├── main.c
      ├── ...
      ├── ...
      ├── ...
      └── Makefile
  ```
+ 作业内容
  - 创建如上所示的目录和文件
  - 基于 FUSE 实现文件系统的读取2
    ```shell
    $ make
    $ ls uxfs
    uxfs
    $ ./mkfs.uxfs disk.bin
    $ ./uxfs -log disk.bin mnt &
    $ cp -rf city/* mnt
    $ fg
    $ ./uxfs -log disk.bin mnt
    ^C
    $ ls mnt
    $ ./uxfs -log disk.bin mnt &
    $ ls mnt
    opendir /
    readdir /
    getattr /hello.c
    getattr /china
    getattr /world.c
    releasedir /
    china  hello.c  world.c
    ```


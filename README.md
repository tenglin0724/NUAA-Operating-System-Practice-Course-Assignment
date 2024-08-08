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

## 作业二

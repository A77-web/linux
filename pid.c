#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
        pid_t pid=fork();//创建进程
        if(pid>0)//父进程
        {
                printf("leihoua~\n");
        }else if(pid==0)//子进程
        {
                printf("你好~\n");
                sleep(5);
                exit(0);//退出进程 0-退出返回值
        }
        while(1)
        {
                printf("hello world!\n");
                sleep(1);
        }
        return 0;
}

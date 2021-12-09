 1 #include<stdio.h>
  2 #include<unistd.h>
  3 #include<signal.h>
  4 #include<stdlib.h>
  5 void sigcb(int signo)
  6 {
  7   printf("recv signum:%d\n",signo);
  8 }
  9 int main()
 10 {
 11   signal(2,sigcb);
 12   signal(40,sigcb);
 13   sigset_t set;//定义信号集合
 14   sigemptyset(&set);//将新定义的不干净的set集合清理干净
 15   sigaddset(&set,2);
 16   sigaddset(&set,40);//将2号和40 号信号添加到要操作的set信号集中；                                                                                   
 17   sigprocmask(SIG_BLOCK,&set,NULL);//将set中包含的信号阻塞，收到set中的信号先阻塞
 18   printf("按下回车继续运行\n");
 19   getchar();//等待输入回车后在运行到后边看后边的效果
 20   sigprocmask(SIG_UNBLOCK,&set,NULL);//解除阻塞 观察信号处理
 21   return 0;
 22 
 23 }
~

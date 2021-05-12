#include <iostream>
#include <unistd.h>
#include <string>
#include <pthread.h>
#include <signal.h>
#include "judge_info.hpp"
using namespace std;
namespace htto_judger
{
    string compile(const string &compiler, const JudgeInfo &judge_info);
    void children_run(const JudgeInfo &info);
}
int main(int argc, char **argv)
{
    // ./a.out 10 10 cpp ../test/1.in ../test/1.out ../test/b.cpp 111 ../test/
    auto info=htto_judger::get_judge_info(argc, argv);

    pid_t child_pid=fork();
    if(child_pid<0)
    {
        cerr<<"create thread error\n";
        exit(1);
    }
    // children process
    if(child_pid==0)
    {
        htto_judger::children_run(info);
    }
    // parent process
    else {
        cout<<"create parent process\n";
        rusage cost;
        int stat;
        wait4(child_pid,&stat,WSTOPPED,&cost);
        printf("memory usgae:%d Bytes\n",cost.ru_maxrss);
        printf("cpu usage:%d s\n",(int)cost.ru_utime.tv_sec);
    }
    return 0;
}
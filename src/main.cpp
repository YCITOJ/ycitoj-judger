#include <iostream>
#include <unistd.h>
#include <string>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <getopt.h>
#include <sys/resource.h>
#include "judge_info.hpp"
using namespace std;

namespace htto_judger
{
    string compile(const string &compiler, const JudgeInfo &judge_info);
    void children_run(const JudgeInfo &info);
    ResultTag get_state(int stat,const rusage &usage,const htto_judger::JudgeInfo & info)
    {
        int usage_time=usage.ru_utime.tv_sec*1000 + usage.ru_utime.tv_usec/1000;
        if(usage_time > info.time_limit)
            return ResultTag::TLE;

        if(info.memory_limit*1024*1024 < usage.ru_maxrss)
            return ResultTag::MLE;
        
        if (WIFEXITED(stat) ) 
        {
            int exit_code=WEXITSTATUS(stat);
            if(!exit_code)
                return ResultTag::NORMAL;
            if(exit_code==ResultTag::CE)
                return (ResultTag)exit_code;
            return ResultTag::UKE;
        }

        if(WIFSIGNALED(stat))
        {
            return ResultTag::RE;
        }
        return ResultTag::UKE;
    }
    
    struct __KillerParam{
        int child_id;
        JudgeInfo *info;
    };
    void* killer(void *param){
        __KillerParam kp=*(__KillerParam*)param;
        int sleep_time=kp.info->time_limit/1000+2;
        sleep(sleep_time);
        kill(kp.child_id,SIGKILL);
        printf("killed by killer\n");
        return nullptr;
    }
} 



int main(int argc, char **argv)
{
    // ./a.out 10 10 cpp ../test/1.in ../test/1.out ../test/main.cpp 111 ../test/
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
        pthread_t moniter_thread;
        htto_judger::__KillerParam param;
        param.child_id=child_pid;
        param.info = &info;
        int t = pthread_create(&moniter_thread, NULL, htto_judger::killer, (void *)&param);
        if (t != 0) {
            exit(-1);
        }
        rusage cost;
        int stat;
        wait4(child_pid,&stat,WSTOPPED,&cost);
        pthread_cancel(moniter_thread);
        int usage_time=cost.ru_utime.tv_sec*1000 + cost.ru_utime.tv_usec/1000;

        printf("memory usgae:%d Bytes\n",cost.ru_maxrss);
        printf("cpu usage:%d ms\n",usage_time);
        printf("children stat:%d\n",stat);
        int type=htto_judger::get_state(stat,cost,info);
        printf("Type:%d\n",htto_judger::get_state(stat,cost,info));
        exit(type);
    }
    return 0;
}
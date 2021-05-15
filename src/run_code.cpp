#include <iostream>
#include <map>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include "judge_info.hpp"
using namespace std;
namespace htto_judger
{
    string replace_string(string input, const vector<std::string> &param)
    {
        string ret;
        int p_cnt = 0;
        for (int i = 0; i < input.size(); i++)
        {
            if (input[i] == '?')
                ret += param.at(p_cnt++);
            else
                ret.push_back(input[i]);
        }
        return ret;
    }

    map<std::string, std::string> compiler_arg{
        {"cpp", "g++ ? -o ? -std=c++17 "},
    };
    map<std::string, std::string> interpretor_arg{
        {"merdog", "/usr/bin/merdog"},
        {"python", "/usr/bin/python3"},
        {"node", "/usr/bin/node"},
    };
    string compile(const string &compiler, const JudgeInfo &judge_info)
    {
        if (compiler == "cpp")
        {
            string ret = judge_info.gen_path + judge_info.submission_id;
            // if executable file exists, delete it.
            if (access(ret.c_str(), F_OK) == 0)
            {
                if(unlink(ret.c_str()))
                    cerr<<"delete old executable file "+ ret+" failed.\n";
            }
            string ins = replace_string(compiler_arg[compiler], {judge_info.source_path, ret});
            system(ins.c_str());
            return ret;
        }
        return "";
    }
    void redirect(const JudgeInfo &info)
    {
        FILE *input = fopen(info.input_path.c_str(), "r");
        if (!input)
        {
            cerr << "get input file failed!";
            exit(1);
        }
        dup2(fileno(input), fileno(stdin));
        FILE *output = fopen(info.output_path.c_str(), "w");
        if (!output)
        {
            cerr << "write output file faield";
            exit(1);
        }
        dup2(fileno(output), fileno(stdout));
    }
    void set_limit(const JudgeInfo &info)
    {
        // issue : macOS ARM || WSL1 can not restrict memory usage.
        rlimit lim;
        lim.rlim_cur = lim.rlim_max = info.memory_limit * 1024 * 1500;
        // limit memory
        if (setrlimit(RLIMIT_AS, &lim) != 0)
        {
            cerr << "limit memory failed\n";
            exit(1);
        }
        // limit time
        lim.rlim_cur = lim.rlim_max = info.time_limit / 1000 + 1;
        if (setrlimit(RLIMIT_CPU, &lim) != 0)
        {
            cerr << "limit cpu falied";
            exit(1);
        }
    }

    void children_run(const JudgeInfo &info)
    {
        redirect(info);
        set_limit(info);
        if (compiler_arg.count(info.lang))
        {
            string exe_path = info.gen_path + info.submission_id;
            execve(exe_path.c_str(), NULL, NULL);
            cerr << "compiler error\n";
        }
        else if (interpretor_arg.count(info.lang))
        {
            string interpretor = interpretor_arg[info.lang];
            char *argv[] = {
                "interpreter",
                (char *)info.source_path.c_str(),
                NULL};
            execve(interpretor.c_str(), argv, NULL);
            cerr << "interpreter error\n";
        }
        else
        {
            cerr << "invalid compiler or interpreter";
        }
        exit(ResultTag::CE);
    }

}
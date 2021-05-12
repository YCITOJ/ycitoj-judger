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
        {"merdog", "merdog ?"}};
    string compile(const string &compiler, const JudgeInfo &judge_info)
    {
        if (compiler_arg.count(compiler))
        {
            if (compiler == "cpp")
            {
                string ret = judge_info.gen_path + judge_info.submission_id;

                string ins = replace_string(compiler_arg[compiler], {judge_info.source_path, ret});
                cout <<"Ins:"<< ins << endl;
                system(ins.c_str());
                return ret;
            }
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
        std::string output_path = info.gen_path + info.submission_id + ".txt";
        FILE *output = fopen(output_path.c_str(), "w");
        if (!output)
        {
            cerr << "write output file faield";
            exit(1);
        }
        dup2(fileno(output), fileno(stdout));
    }
    void set_limit(const JudgeInfo &info)
    {
        rlimit lim;
        lim.rlim_cur = info.memory_limit * 1024 * 1024;
        lim.rlim_max = lim.rlim_cur;
        // limit memory
        if (setrlimit(RLIMIT_AS, &lim) != 0)
        {
            cerr << "limit memory failed\n";
            exit(1);
        }
    }

    void children_run(const JudgeInfo &info)
    {
        string exe_path = compile(info.lang, info);
        char *envp[] = {"PATH=/bin", 0};
        set_limit(info);
        redirect(info);
        execve(exe_path.c_str(), NULL, envp);
        cerr << "compiler error";
        exit(0);
    }
}
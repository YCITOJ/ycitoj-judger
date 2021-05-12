#include "judge_info.hpp"
#include <cstring>
#include <iostream>
#define _mdebug(x) std::cout << #x << " : " << x << std::endl
namespace htto_judger
{

    void JudgeInfo::debug()
    {
        _mdebug(memory_limit);
        _mdebug(time_limit);
        _mdebug(lang);
        _mdebug(input_path);
        _mdebug(output_path);
        _mdebug(source_path);
        _mdebug(gen_path);
        _mdebug(submission_id);
    }

    int parse_int(const std::string &str)
    {
        int res = 0;

        for (auto ch : str)
        {
            res *= 10;
            res += ch - '0';
        }
        return res;
    }

    JudgeInfo get_judge_info(int pcnt, char **param)
    {
        JudgeInfo ret;
        ret.memory_limit=-1;
        // one is program name
        if (pcnt != 9)
        {
            printf("Param Error need 9 bug get %d ",pcnt);
            return ret;
        }
        ret.memory_limit = parse_int(std::string(param[1]));
        ret.time_limit = parse_int(std::string(param[2]));
        ret.lang = param[3];
        ret.input_path = param[4];
        ret.output_path = param[5];
        ret.source_path = param[6];
        ret.submission_id = param[7];
        ret.gen_path=param[8];
        return ret;
    }
}
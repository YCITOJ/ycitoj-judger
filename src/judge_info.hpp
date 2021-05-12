#pragma once
#include <string>
#include <vector>

namespace htto_judger
{
    // judge result Tag
    enum ResultTag
    {
        NORMAL,
        TLE,
        MLE,
        RE,
        UKE,
        CE,
    };
    struct JudgeInfo
    {
        // KB
        int memory_limit;
        // ms
        int time_limit;
        std::string lang;
        std::string input_path;
        std::string output_path;
        std::string source_path;
        std::string submission_id;
        std::string gen_path;
        void debug();
    };
    struct JudgeResult
    {
        ResultTag stat;
        int time_usage;
        int limit_usgae;
    };
    /*
        parameter order 
        1. memory_limit  
        2. time_limit  
        3. lang
        4. input_path
        5. output_path
        6. source_path
        7. submission_id
        8. gen_path
        @pcnt: parameter number
        @param: param content
    */
    JudgeInfo get_judge_info(int pcnt, char **param);

}
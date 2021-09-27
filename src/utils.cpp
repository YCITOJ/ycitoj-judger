
#include "utils.hpp"
#include <fstream>
namespace utils
{
    JSON &config()
    {
        static std::string content;
        if (content.empty())
        {
            std::ifstream ifs("./config.json");
            if (!ifs.good())
            {
                std::cout << "Open config failed\n";
                exit(1);
            }
            std::string line;
            while (getline(ifs, line))
            {
                content += line;
            }
        }
        static JSON ret(content);
        return ret;
    }
}
#include <iostream>
#include <list>
#include <string>
#include <sstream>

int main()
{
    std::list<std::string> history;
    std::string line;

    while (std::getline(std::cin, line))
    {
        if (line == "END")
            break;

        if (line.rfind("ADD ", 0) == 0)
        {                                      // 以 "ADD " 开头
            history.push_back(line.substr(4)); // 从下表4开始截到末尾，s 可以包含空格
        }
        else if (line == "UNDO")
        {
            if (!history.empty())
                history.pop_back();
        }
        else if (line == "SHOW")
        {
            for (std::list<std::string>::const_iterator it = history.begin();
                 it != history.end(); it++)
            {
                std::cout << *it << "\n";
            }
        }
    }
    return 0;
}

#include "Index.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include <locale>
#include <stdexcept>
#include <algorithm> 

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << std::endl;
        return 1;
    }

    std::cout << "INDEX" << "\n=====\n" << std::endl;

    Index index;

    for (int i = 1; i < argc; ++i)
    {
        std::ifstream file(argv[i]);
        
        if (!file.is_open())
        {
            std::cerr << "Error opening file: " << argv[i] << std::endl;
            continue;
        }

        std::string line;
        int line_number = 1;

        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string word;

            while (iss >> word)
            {
                for (char& c : word)
                {
                    if (!word.empty())
                    {
                        while (!word.empty() && std::ispunct(word.front()))
                        {
                            word.erase(0, 1);
                        }

                        while (!word.empty() && std::ispunct(word.back()))
                        {
                            word.pop_back();
                        }
                    }
                }

                std::transform(word.begin(), word.end(), word.begin(), ::tolower);

                index.add_word(word, argv[i], line_number);
            }

            ++line_number;
        }
    }

    std::cout << index;

    return 0;
}

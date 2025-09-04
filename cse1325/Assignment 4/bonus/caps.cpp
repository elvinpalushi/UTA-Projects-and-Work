#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
    std::vector<std::string> caps;
    std::vector<std::string>* no_caps = new std::vector<std::string>;

    for (int i = 1; i < argc; i++)
    {
        if (std::isupper(argv[i][0]))
        {
            caps.push_back(argv[i]);
        }
        else
        {
            no_caps->push_back(argv[i]);
        }
    }

    std::cout << "\n" << "Capitalized:" << std::endl;
    for (auto word : caps) {
        std::cout << word << std::endl;
    }

    std::cout << "\n" << "Lower Case:" << std::endl;
    for (auto word : *no_caps) {
        std::cout << word << std::endl;
    }

    delete no_caps;

    return 0;
}
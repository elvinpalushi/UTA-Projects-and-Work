#include "Index.h"

void Index::add_word(Word word, std::string filename, int line)
{
    Locations& locations = index[word];

    if (locations.find(Location(filename, line)) == locations.end())
    {
        locations.insert(Location(filename, line));
    }
}

std::ostream& operator<<(std::ostream& ost, const Index& index)
{
    for (auto& [ word, locations ] : index.index)
    {
        Location::next_word();

        ost << word << ": ";

        auto it = locations.begin();
        while(it != locations.end())
        {
            ost << *it;
            if (std::next(it) != locations.end()) 
            {
                ost << ", ";
            }
            ++it;
        }
        ost << '\n';
    }
    return ost;
}
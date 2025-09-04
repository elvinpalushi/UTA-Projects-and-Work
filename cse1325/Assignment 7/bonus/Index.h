#ifndef __INDEX_H
#define __INDEX_H

#include <iostream>
#include <map>
#include <set>
#include "Location.h"

class Index
{
    public:
        typedef std::string Word;
        typedef std::set<Location> Locations;

        void add_word(Word word, std::string filename, int line);
        friend std::ostream& operator<<(std::ostream& ost, const Index& index);
    private:
        std::map<Word, Locations> index = {};
};

#endif
#include "Location.h"

std::string Location::last_filename;

Location::Location(std::string fn, int l) : filename(fn), line(l) {}

int Location::compare(const Location& location) const
{
    if(filename<location.filename) return -1;
    if(filename>location.filename) return  1;
    if(line<location.line) return -1;
    if(line>location.line) return  1;
    return 0;   
}

void Location::next_word()
{
    last_filename = "";
}

std::ostream& operator<<(std::ostream& ost, const Location& location)
{
    if(location.filename != Location::last_filename)
    {
        ost << location.filename << " line ";
        Location::last_filename = location.filename;
    }

    ost << location.line;
        
    return ost;
}
#include "Location.h"

Location::Location(std::string fn, int l) : filename(fn), line(l) {}

int Location::compare(const Location& location) const
{
    if(filename<location.filename) return -1;
    if(filename>location.filename) return  1;
    if(line<location.line) return -1;
    if(line>location.line) return  1;
    return 0;   
}

std::ostream& operator<<(std::ostream& ost, const Location& location)
{
    ost << location.filename << " line " << location.line;
        
    return ost;
}
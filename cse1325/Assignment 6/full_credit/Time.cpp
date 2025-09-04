#include "Time.h"
#include <iostream>
#include <exception>
#include <algorithm>

Time::Time(int h, int m, int s) : hour(h), minute(m), second(s)
{
    rationalize();
}

Time Time::operator+(const Time& time)
{
    int totalSeconds = hour * 3600 + minute * 60 + second + time.hour * 3600 + time.minute * 60 + time.second;
    
    int hours = totalSeconds / 3600;
    int remainingSeconds = totalSeconds % 3600;
    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;

    return Time(hours, minutes, seconds);
}

Time& Time::operator++()
{
    ++second;
    rationalize();
    return *this;
}

Time Time::operator++(int)
{
    Time time{*this};
    ++*this;
    return time;
}

void Time::rationalize()
{
    int totalSeconds = hour * 3600 + minute * 60 + second;
    totalSeconds = (totalSeconds + 86400) % 86400;

    hour = (totalSeconds / 3600) % 24;
    minute = (totalSeconds / 60) % 60;
    second = totalSeconds % 60;
}

int Time::compare(const Time& time)
{
    if(hour<time.hour) return -1;
    if(hour>time.hour) return  1;
    if(minute<time.minute) return -1;
    if(minute>time.minute) return  1;
    if(second<time.second) return -1;
    if(second>time.second) return  1;
    return 0;   
}

std::ostream& operator<<(std::ostream& ost, const Time& time)
{
    ost << std::setfill('0') << std::setw(2) << time.hour << ":"
        << std::setw(2) << time.minute << ":" << std::setw(2) << time.second;
        
    return ost;
}

std::istream& operator>>(std::istream& ist, Time& time)
{
    char discard;
    ist >> time.hour >> discard >> time.minute >> discard >> time.second;

    if (discard != ':')
    {
        ist.setstate(std::ios::failbit);
    }
    else
    {
        time.rationalize();
    }

    return ist;
}
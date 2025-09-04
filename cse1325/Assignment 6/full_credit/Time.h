#ifndef __TIME_H
#define __TIME_H

#include <iostream>
#include <iomanip>

class Time
{
    public:
        Time(int hour = 0, int minute = 0, int second = 0);

        Time operator+(const Time& time);
        Time& operator++();
        Time operator++(int);

        inline bool operator==(const Time& time) {return (compare(time) == 0);}
        inline bool operator!=(const Time& time) {return (compare(time) != 0);}
        inline bool operator< (const Time& time) {return (compare(time) <  0);}
        inline bool operator<=(const Time& time) {return (compare(time) <= 0);}
        inline bool operator> (const Time& time) {return (compare(time) >  0);}
        inline bool operator>=(const Time& time) {return (compare(time) >= 0);} 

        friend std::ostream& operator<<(std::ostream& ost, const Time& time);
        friend std::istream& operator>>(std::istream& ist, Time& time);
    private:
        void rationalize();
        int compare(const Time& time);   

        int hour;
        int minute;
        int second;
};

#endif
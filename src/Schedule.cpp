#include <Schedule.hpp>
#include <iostream>

void Schedule::insert_day(Day d)
{
    Schedule::days.push_back(d);
}

void Schedule::delete_day(int index)
{
    Schedule::days.erase(days.begin() + index);
}

void Schedule::add_to_day(int index, TaskTime t)
{
    Schedule::days[index].insert_next(t);
}

void Schedule::delete_task_from(int day, int task)
{
    Schedule::days[day].delete_at(task);
}

Day Schedule::get_day(int index)
{
    return Schedule::days[index];
}

struct time Schedule::get_total_time()
{
    struct time total_time;
    struct time day_time;

    total_time.hour = 0;
    total_time.minute = 0;

    for(unsigned i = 0; i < Schedule::days.size(); ++i)
    {
        day_time = days[i].get_total_time();
        total_time.hour += day_time.hour;
        total_time.minute += day_time.minute;
    }

    std::cout << total_time.hour << ' ' << total_time.minute << '\n';

    return align_time(total_time);
}

std::vector<char> Schedule::serialize()
{
    std::vector<char> serializer;
    std::vector<char> day_serializer;
    
    for(unsigned i = 0; i < Schedule::days.size(); ++i)
    {
        day_serializer = Schedule::days[i].serialize();
        std::copy(day_serializer.begin(), day_serializer.end(), 
        std::back_inserter(serializer));
    }
    
    return serializer;
}

std::string Schedule::to_string()
{
    std::string s;
    for(unsigned long i = 0; i < days.size(); ++i) 
    {
        s += Schedule::days[i].to_string();
        s += "\n";
    }
    return s;
}

std::ostream& operator<<(std::ostream& os, Schedule s)
{
    return os << s.to_string();
}
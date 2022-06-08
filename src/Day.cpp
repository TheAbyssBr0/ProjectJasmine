#include <Day.hpp>
#include <iostream>


Day::Day(std::string n)
{
    Day::day_title = n;
}

int Day::get_max_len()
{
    int max_len; 
    
    max_len = 0;
    for (unsigned long i = 0; i < task_list.size(); ++i)
    {
        max_len = (task_list[i].get_len() > max_len) ? 
        task_list[i].get_len() : max_len;
    }
    return max_len;
}

void Day::insert_next(TaskTime t) 
{
    task_list.push_back(t);
}

/*
void Day::insert_at(TaskTime t, int index)
{
    task_list.insert(task_list.begin() + index, t);
}
*/

void Day::delete_at(int index)
{
    task_list.erase(task_list.begin() + index);
}

std::string Day::get_title()
{
    return Day::day_title;
}

struct time Day::get_total_time()
{
    struct time total_time;
    struct time time_spent_day;

    total_time.hour = 0;
    total_time.minute = 0;

    for(unsigned i = 0; i < Day::task_list.size(); ++i)
    {
        time_spent_day = Day::task_list[i].time_spent();
        total_time.hour += time_spent_day.hour;
        total_time.minute += time_spent_day.minute;
    }

    return align_time(total_time);    
}

std::vector<char> Day::serialize()
{
    // serialized as ^ day title time_task1, time_task2 ...
    std::vector<char> serializer;
    std::vector<char> day_serializer;

    serializer.push_back('^');
    std::copy(Day::day_title.begin(), day_title.end(), 
    std::back_inserter(serializer));

    for(unsigned i = 0; i < Day::task_list.size(); ++i)
    {
        day_serializer = Day::task_list[i].serialize();
        std::copy(day_serializer.begin(), day_serializer.end(), 
        std::back_inserter(serializer));
    }

    return serializer;
}

std::string Day::to_string()
{
    std::string return_string;
    return_string = Day::day_title + "\n";
    for (unsigned long i = 0; i < task_list.size(); ++i)
    {
        return_string += 
        task_list[i].to_string(get_max_len() + SPACES_AFTER_MAX);
    }

    return return_string;
}

std::ostream& operator<<(std::ostream& os, Day c)
{
    return os << c.to_string();
}
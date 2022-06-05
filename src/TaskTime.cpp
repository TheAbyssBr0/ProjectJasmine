#include <string>
#include <sstream>
#include <iomanip>
#include <TaskTime.hpp>

#define PADDING std::setw(2) << std::setfill('0') 

TaskTime::TaskTime
(std::string t,
unsigned char start_h,
unsigned char start_min,
unsigned char end_h,
unsigned char end_min)
{
    task_title = t;
    task_time[0] = start_h;
    task_time[1] = start_min;
    task_time[2] = end_h;
    task_time[3] = end_min;
}

int TaskTime::get_len()
{
    return TaskTime::task_title.length();
}

std::vector<char> TaskTime::serialize()
{
    // Timetasks serialize with &, t1, t2, t3, t4, title
    std::vector<char> serializer;
    int num_bytes;

    serializer.push_back('&');

    num_bytes = 4;          // 4 bytes per TaskTime
    for(int i = 0; i < num_bytes; ++i)
        serializer.push_back(TaskTime::task_time[i]);

    std::copy(TaskTime::task_title.begin(), 
        TaskTime::task_title.end(), 
        std::back_inserter(serializer));
    
    return serializer;
}

std::string TaskTime::to_string(int space_len)
{
    std::stringstream string_stream;
    string_stream << 
    task_title << std::string(space_len - TaskTime::get_len(), ' ') << 
    PADDING << +task_time[0] <<
    ":" <<
    PADDING << +task_time[1] <<
    " - " <<
    PADDING << +task_time[2] <<
    ":" <<
    PADDING << +task_time[3] <<
    "\n";

    return string_string.str();
}

short operator+(TaskTime t1, TaskTime t2)
{
    unsigned char hours_t1, hours_t2, hours_total,
    minutes_t1, minutes_t2, minutes_total;
    short time_total;

    hours_t1 = t1::task_time[2] - t1::task_time[0] - 1;
    minutes_t1 = t1::task_time[3] + 60 - t1::task_time[1];
    hours_t2 = t2::task_time[2] - t2::task_time[0] - 1;
    minutes_t2 = t2::task_time[3] + 60 - t2::task_time[1];

    hours_total = hours_t1 + hours_t2;
    minutes_total = minutes_t1 + minutes_t2;

    while(minutes_total >= 60)
    {
        minutes_total -= 60;
        ++hours_total;
    }

    time_total = (((short)hours_total) << 8) | (short)minutes_total;
    return time_total;
}
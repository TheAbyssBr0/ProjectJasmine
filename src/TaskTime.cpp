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

    return string_stream.str();
}
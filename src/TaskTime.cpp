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
    task = t;
    time[0] = start_h;
    time[1] = start_min;
    time[2] = end_h;
    time[3] = end_min;
    task_len = t.length();
}

std::vector<char> TaskTime::serialize()
{
    // Timetasks serialize with &, title, `, t1, t2, t3, t4
    std::vector<char> serializer;
    serializer.push_back('&');
    std::copy(TaskTime::task.begin(), TaskTime::task.end(), std::back_inserter(serializer));
    serializer.push_back('`');
    
    int number_of_times = 4;
    for(int i = 0; i < number_of_times; ++i)
        serializer.push_back(TaskTime::time[i]);

    return serializer;
}

std::string TaskTime::to_string(int space_len)
{
    std::stringstream string_string;
    string_string << 
    task << std::string(space_len - task_len, ' ') << 
    PADDING << +time[0] <<
    ":" <<
    PADDING << +time[1] <<
    " - " <<
    PADDING << +time[2] <<
    ":" <<
    PADDING << +time[3] <<
    "\n";

    return string_string.str();
}
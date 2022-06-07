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
    
    task_start.hour = start_h;
    task_start.minute = start_min;

    task_end.hour = end_h;
    task_end.minute = end_min;
}

int TaskTime::get_len()
{
    return TaskTime::task_title.length();
}

struct time TaskTime::time_spent()
{
    struct time time_spent;

    time_spent.hour = TaskTime::task_end.hour
     - TaskTime::task_start.hour - 1;   // one hour is taken and added
                                        // to minutes
    time_spent.minute = TaskTime::task_end.minute 
    + 60 - TaskTime::task_start.minute; // 60 here is the added minute

    while(time_spent.minute >= 60)
    {
        ++time_spent.hour;
        time_spent.minute -= 60;
    }

    return time_spent;
}

std::vector<char> TaskTime::serialize()
{
    // Timetasks serialize with &, t1, t2, t3, t4, title
    std::vector<char> serializer;
    int num_bytes;

    serializer.push_back('&');

    serializer.push_back(TaskTime::task_start.hour);
    serializer.push_back(TaskTime::task_start.minute);
    serializer.push_back(TaskTime::task_end.hour);
    serializer.push_back(TaskTime::task_end.minute);

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
    PADDING << +TaskTime::task_start.hour <<
    ":" <<
    PADDING << +TaskTime::task_start.minute <<
    " - " <<
    PADDING << +TaskTime::task_end.hour <<
    ":" <<
    PADDING << +TaskTime::task_end.minute <<
    "\n";

    return string_stream.str();
}
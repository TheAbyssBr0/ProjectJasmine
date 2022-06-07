#include <Time.hpp>
#include <ctime>
#include <sstream>
#include <iomanip>

#define PADDING std::setw(2) << std::setfill('0') 

std::string name_helper(bool last_week)
{
    time_t now_raw;
    int days_since_target_monday;
    struct tm * time_diff;
    std::stringstream return_str;

    time(&now_raw);
    time_diff = localtime(&now_raw);
    days_since_target_monday = (time_diff->tm_wday - 1 + 7 * last_week);

    time_diff->tm_mday -= days_since_target_monday;
    mktime(time_diff);

    return_str << time_diff->tm_year - 100 <<
    PADDING << +(time_diff->tm_mon + 1) <<
    PADDING << +(time_diff->tm_mday) <<
    ".bin";
    
    return return_str.str();
}

std::string get_week_name()
{
    return name_helper(false);
}

std::string get_last_week_name()
{
    return name_helper(true);
}
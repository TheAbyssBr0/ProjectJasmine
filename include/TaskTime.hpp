#include <string>
#include <vector>

/** 
 * Creates an object that has a task title and the duration
 * in form of two 24-hour times
 */
class TaskTime
{
    private:
        std::string task_title;
        unsigned char task_time[4];

    public:
        TaskTime
        (std::string t,
        unsigned char start_h,
        unsigned char start_min,
        unsigned char end_h,
        unsigned char end_m);

        int get_len();
        std::vector<char> serialize();
        std::string to_string(int space_len);
        short operator+(TaskTime t1, TaskTime t2);
};

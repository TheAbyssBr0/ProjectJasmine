#include <string>
#include <vector>

/** 
 * Creates an object that has a task title and the duration
 * in form of two 24-hour times
 */
class TaskTime
{
    private:
        std::string task;
        unsigned char time[4];
        int task_len;

    public:
        int get_len();

        TaskTime
        (std::string t,
        unsigned char start_h,
        unsigned char start_min,
        unsigned char end_h,
        unsigned char end_m);

        std::vector<char> serialize();
        std::string to_string(int space_len);
};

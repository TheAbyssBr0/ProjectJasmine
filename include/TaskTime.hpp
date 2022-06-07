#include <string>
#include <vector>

/** 
 * Hour and minute together in one struct
 */
struct time
{
    unsigned char hour;
    unsigned char minute;
};

/** 
 * Creates an object that has a task title and the duration
 * in form of two 24-hour times
 */
class TaskTime
{
    private:
        std::string task_title;
        struct time task_start;
        struct time task_end;

    public:
        TaskTime
        (std::string t,
        unsigned char start_h,
        unsigned char start_min,
        unsigned char end_h,
        unsigned char end_m);

        int get_len();
        struct time time_spent();
        std::vector<char> serialize();
        std::string to_string(int space_len);
};

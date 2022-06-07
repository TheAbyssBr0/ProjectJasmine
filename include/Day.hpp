#include <vector>
#include <TaskTime.hpp>
#define SPACES_AFTER_MAX 5

/**
 * Creates an object that represents a day (list of task-times) 
 */
class Day
{
    private:
        std::vector<TaskTime> task_list;
        int get_max_len();
        std::string day_title;

    public:
        Day(std::string n);
        void insert_next(TaskTime t);
        // void insert_at(TaskTime t, int index);
        void delete_at(int index);
        std::string get_title();
        struct time get_total_time();
        std::vector<char> serialize();
        std::string to_string();
        friend std::ostream& operator<<(std::ostream& os, Day c);
};
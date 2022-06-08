#include <Day.hpp>

class Schedule
{
    private:
        std::vector<Day> days;
       
    public:
        void insert_day(Day d);
        void add_to_day(int index, TaskTime t);
        void delete_day(int index);
        void delete_task_from(int day, int task);
        Day get_day(int index);
        struct time get_total_time();
        std::vector<char> serialize();
        std::string to_string();
        friend std::ostream& operator<<(std::ostream& os, Schedule s);
};
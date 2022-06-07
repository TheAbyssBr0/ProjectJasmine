#include <Schedule.hpp>
#include <Check.hpp>

class Journal : public Schedule
{
    private:
        std::string title;
        std::vector<Check> check_list;

    public:
        Journal(std::string title);
        void insert_check(std::string c_string);
        void insert_checks(std::vector<Check> check_vector);
        void change_check(int index, check_status c);
        std::vector<Check> get_incomplete();
        void save_as(std::string file_n);
        std::string to_string();
        friend std::ostream& operator<<(std::ostream& os, Journal j);
};
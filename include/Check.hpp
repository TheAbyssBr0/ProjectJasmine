#include <string>
#include <vector>

enum check_status {unchecked = 0, checked = 1, partial = 2};

class Check
{
    private:
        std::string check_title;
        check_status check_stat; 
    
    public:
        Check(std::string c_string);
        void change_status_to(check_status c);
        check_status get_check_status();
        std::string get_title();
        std::vector<char> serialize();
        std::string to_string();
};
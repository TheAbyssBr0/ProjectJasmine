#include <Check.hpp>
#include <string>

Check::Check(std::string c_string)
{
    Check::check_title = c_string;
    Check::check_stat = unchecked;
}

void Check::change_status_to(check_status c)
{
    Check::check_stat = c;
}

check_status Check::get_check_status()
{
    return check_stat;
}

std::string Check::get_title()
{
    return Check::check_title;
}

std::vector<char> Check::serialize()
{
    std::vector<char> serializer;

    serializer.push_back('%');
    serializer.push_back(Check::check_stat);
    std::copy(Check::check_title.begin(), Check::check_title.end(), std::back_inserter(serializer));
    
    return serializer;
}

std::string Check::to_string()
{
    std::string check_strings[3] = {"[ ]", "[X]", "[\\]"};
    return check_strings[check_stat] + " " + Check::check_title + "\n";
}
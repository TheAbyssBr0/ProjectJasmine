#include <Journal.hpp>
#include <fstream>

Journal::Journal(std::string t)
{
    Journal::title = t;
}

void Journal::insert_check(std::string c_string)
{
    check_list.push_back(Check(c_string));
}

void Journal::insert_checks(std::vector<Check> check_vector)
{
    Journal::check_list.insert(check_list.end(), check_vector.begin(), check_vector.end());
}

void Journal::change_check(int index, check_status c)
{
    check_list[index].change_status_to(c);
}

std::vector<Check> Journal::get_incomplete()
{
    std::vector<Check> partial_or_unchecked;
    for(unsigned long i = 0; i < Journal::check_list.size(); ++i)
    {
        if (check_list[i].get_check_status() != checked)
        {
            partial_or_unchecked.push_back(check_list[i]);
        }
    }

    return partial_or_unchecked;
}

void Journal::save_as(std::string file_n)
{
    /**  
     * X1 checklist title  | %
     * X2 Day title        | ^
     * X3 time task title  | &
     */
    std::vector<char> serializer;
    std::vector<char> check_serializer;
    std::vector<char> schedule_serializer;

    for(unsigned i = 0; i < Journal::check_list.size(); ++i)
    {
        check_serializer = Journal::check_list[i].serialize();
        std::copy(check_serializer.begin(), check_serializer.end(), 
        std::back_inserter(serializer));
    }

    schedule_serializer = Schedule::serialize();
    std::copy(schedule_serializer.begin(), schedule_serializer.end(),
    std::back_inserter(serializer));

    std::ofstream os;
    os.open(file_n, std::ios::binary);
    os.write(reinterpret_cast<char const*>(serializer.data()), serializer.size());
    os.close();
}

std::string Journal::to_string()
{
    std::string s;
    s += Journal::title + "\n" + "\nTODO:\n";

    for(unsigned long i = 0; i < Journal::check_list.size(); ++i)
    {
        s += std::to_string(i + 1) + ". " + check_list[i].to_string();
    }

    s += "\n";
    s += Schedule::to_string();

    return s;
}

std::ostream& operator<<(std::ostream& os, Journal j)
{
    return os << j.to_string();
}
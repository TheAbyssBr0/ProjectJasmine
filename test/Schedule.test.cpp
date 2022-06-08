#include <Schedule.hpp>
#include <gtest/gtest.h>
#include <iostream>

TEST(ScheduleTest, allMethods)
{
    Day d1("Monday");
    TaskTime t1("task 01", 13, 00, 14, 50);
    TaskTime t2("task 2", 14, 55, 15, 30);
    d1.insert_next(t1);
    d1.insert_next(t2);

    Day d2("Tuesday");
    d2.insert_next(t2);

    Day d3("not Wednesday");
    d3.insert_next(t1);

    // add all days to schedule s
    Schedule s;
    s.insert_day(d1);
    s.insert_day(d2);
    s.insert_day(d3);
    // delete not Wednesday
    s.delete_day(2);
    // delete task from Tuesday
    s.delete_task_from(1, 0);
    // add different task to Tuesday
    s.add_to_day(1, t1);
    Day d_new_tuesday = s.get_day(1);

    EXPECT_STREQ(d_new_tuesday.to_string().data(), 
    "Tuesday\ntask 01     13:00 - 14:50\n");

    EXPECT_STREQ(s.to_string().data(), 
    "Monday\ntask 01     13:00 - 14:50\ntask 2      14:55 - 15:30\n\nTuesday\ntask 01     13:00 - 14:50\n\n");

    char c[51] = {'^', 'M', 'o', 'n', 'd', 'a', 'y', 
    '&', 13, 0, 14, 50, 't', 'a', 's', 'k', ' ', '0', '1', 
    '&', 14, 55, 15, 30, 't', 'a', 's', 'k', ' ', '2',
    '^', 'T', 'u', 'e', 's', 'd', 'a', 'y',
    '&', 13, 0, 14, 50, 't', 'a', 's', 'k', ' ', '0', '1'};
    std::vector<char> v = s.serialize();
    v.push_back(0);

    EXPECT_STREQ(v.data(), c);

    struct time tt;
    tt = s.get_total_time();
    EXPECT_EQ(tt.hour, 4);
    EXPECT_EQ(tt.minute, 15);
}

int main(int argc, char* argv[])
{
    Day d1("Monday");
    TaskTime t1("task 01", 13, 00, 14, 50);
    TaskTime t2("task 2", 14, 55, 15, 30);
    d1.insert_next(t1);
    d1.insert_next(t2);

    Day d2("Tuesday");
    d2.insert_next(t2);

    Day d3("not Wednesday");
    d3.insert_next(t1);

    // add all days to schedule s
    Schedule s;
    s.insert_day(d1);
    s.insert_day(d2);
    s.insert_day(d3);
    // delete not Wednesday
    s.delete_day(2);
    // delete task from Tuesday
    s.delete_task_from(1, 0);
    // add different task to Tuesday
    s.add_to_day(1, t1);

    struct time tt = s.get_total_time();

    std::cout << tt.hour << ' ' << tt.minute << '\n';

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
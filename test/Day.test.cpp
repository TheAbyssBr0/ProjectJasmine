#include <Day.hpp>
#include <gtest/gtest.h>

TEST(DayTest, allMethods)
{
    std::string title = "Monday";
    Day d(title);
    EXPECT_STREQ(d.get_title().data(), title.data());
    TaskTime t1("task 01", 13, 00, 14, 50);
    TaskTime t2("task 2", 14, 55, 15, 30);
    d.insert_next(t1);
    d.insert_next(t2);
    struct time tt = d.get_total_time();
    EXPECT_EQ((int)tt.hour, 2);
    EXPECT_EQ((int)tt.minute, 25);
    d.delete_at(0);
    tt = d.get_total_time();
    EXPECT_EQ((int)tt.hour, 0);
    EXPECT_EQ((int)tt.minute, 35);
    d.insert_next(t1);
    EXPECT_STREQ(d.to_string().data(), 
    "Monday\ntask 2      14:55 - 15:30\ntask 01     13:00 - 14:50\n");

    char c[31] = {'^', 'M', 'o', 'n', 'd', 'a', 'y', '&', 14, 55, 15, 30, 
    't', 'a', 's', 'k', ' ', '2', '&', 13, 00, 14, 50, 't', 'a', 's', 'k',
    ' ', '0', '1'};
    std::vector<char> v = d.serialize();
    v.push_back('\0');
    EXPECT_STREQ(v.data(), c);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
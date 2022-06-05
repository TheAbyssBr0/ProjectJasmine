// #include <TaskTime.hpp>
#include <gtest/gtest.h>

TEST(TaskTimeTest, BasicAssertions)
{
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 5, 35);
}
/*
int main()
{
    TaskTime _test_task_time("t", 4, 20, 5, 30);
    TaskTime _test_task_time2("t", 0, 0, 1, 2);
    TaskTime _test_task_time3("tt", 14, 0, 15, 2);
    TaskTime _test_task_time4("ttt", 14, 10, 15, 20);
    std::cout << _test_task_time.to_string(4);
    std::cout << _test_task_time2.to_string(4);
    std::cout << _test_task_time3.to_string(4);
    std::cout << _test_task_time4.to_string(4);
    
    std::vector<char> c = _test_task_time.serialize();
    for(int i = 0; i < c.size(); ++i)
        std::cout << c[i];
    std::cout << '\n'; 
}
*/
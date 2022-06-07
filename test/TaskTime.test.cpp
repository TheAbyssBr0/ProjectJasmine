#include <TaskTime.hpp>
#include <gtest/gtest.h>
#include <iostream>

struct TaskTimeTest: testing::Test
{
    TaskTime* t1;
    TaskTime* t2;
    TaskTime* t3;

    TaskTimeTest()
    {
        t1 = new TaskTime("task", 14, 30, 15, 45);
        t2 = new TaskTime("task 2 longer title", 9, 30, 10, 5);
        t3 = new TaskTime("t", 2, 5, 3, 4);
    }

    virtual ~TaskTimeTest()
    {
        delete t1;
        delete t2;
        delete t3;
    }
};

TEST_F(TaskTimeTest, getLenTests)
{
    EXPECT_EQ(t1->get_len(), 4);
    EXPECT_EQ(t2->get_len(), 19);
    EXPECT_EQ(t3->get_len(), 1);
}

TEST_F(TaskTimeTest, timeSpentTests)
{
    EXPECT_EQ((int)t1->time_spent().hour, 1);
    EXPECT_EQ((int)t1->time_spent().minute, 15);
    EXPECT_EQ((int)t2->time_spent().hour, 0);
    EXPECT_EQ((int)t2->time_spent().minute, 35);
    EXPECT_EQ((int)t3->time_spent().hour, 0);
    EXPECT_EQ((int)t3->time_spent().minute, 59);
}

TEST_F(TaskTimeTest, toStringTests)
{
    EXPECT_STREQ(t1->to_string(10).data(), "task      14:30 - 15:45\n");
    EXPECT_STREQ(t2->to_string(20).data(), "task 2 longer title 09:30 - 10:05\n");
    EXPECT_STREQ(t3->to_string(10).data(), "t         02:05 - 03:04\n");
}

TEST_F(TaskTimeTest, serializeTests)
{
    char t1_str[10] = {'&', 14, 30, 15, 45, 't', 'a', 's', 'k'};
    std::vector<char> t1_vec = t1->serialize();
    t1_vec.push_back('\0');
    EXPECT_STREQ(t1_vec.data(), t1_str);

    char t2_str[25] = {'&', 9, 30, 10, 5, 't', 'a', 's', 'k', ' ', '2', ' ', 
    'l', 'o', 'n', 'g', 'e', 'r', ' ', 't', 'i', 't', 'l', 'e'};
    std::vector<char> t2_vec = t2->serialize();
    t2_vec.push_back('\0');
    EXPECT_STREQ(t2_vec.data(), t2_str);

    char t3_str[7] = {'&', 2, 5, 3, 4, 't'};
    std::vector<char> t3_vec = t3->serialize();
    t3_vec.push_back('\0');
    EXPECT_STREQ(t3_vec.data(), t3_str);}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include <Check.hpp>
#include <gtest/gtest.h>

TEST(CheckTest, allCheckTests)
{
    Check c("Check");
    EXPECT_STREQ(c.to_string().data(), "[ ] Check\n");
    EXPECT_EQ(c.get_check_status(), 0);
    EXPECT_STREQ(c.get_title().data(), "Check");
    c.change_status_to(partial);
    EXPECT_EQ(c.get_check_status(), 2);
    EXPECT_STREQ(c.to_string().data(), "[\\] Check\n");
    c.change_status_to(checked);
    EXPECT_EQ(c.get_check_status(), 1);
    EXPECT_STREQ(c.to_string().data(), "[X] Check\n");
    char c_s[10] = {'%', 1, 'C', 'h', 'e', 'c', 'k'};
    std::vector<char> v = c.serialize();
    v.push_back(0);
    EXPECT_STREQ(v.data(), c_s);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

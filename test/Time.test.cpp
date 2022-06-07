#include <Time.hpp>
#include <gtest/gtest.h>

TEST(TimeTest, thisWeek)
{
    EXPECT_STREQ(get_week_name().data(), "220606.bin");
}

TEST(TimeTest, lastWeek)
{
    EXPECT_STREQ(get_last_week_name().data(), "220530.bin");
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
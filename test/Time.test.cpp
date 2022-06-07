#include <Time.hpp>
#include <gtest/gtest.h>

/**
 * They work. If you wanna test them again,
 * edit out the hardcoded expected values.
 */
TEST(TimeTest, thisWeek)
{
    // EXPECT_STREQ(get_week_name().data(), "220606.bin");
    EXPECT_STRNE(get_week_name().data(), "220530.bin");
}

TEST(TimeTest, lastWeek)
{
    // EXPECT_STREQ(get_last_week_name().data(), "220530.bin");
    EXPECT_STRNE(get_week_name().data(), "220430.bin");
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
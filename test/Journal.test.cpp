#include <Journal.hpp>
#include <gtest/gtest.h>

TEST(JournalTest, allMethods)
{
    Journal j("Week of Jun 6");
    j.insert_check("Check 1");
    std::vector<Check> checks;
    Check c2("Check 2");
    Check c3("Check 3");
    checks.push_back(c2);
    checks.push_back(c3);
    j.insert_checks(checks);

    j.change_check(0, checked);
    j.change_check(1, partial);
    checks.erase(checks.begin());

    EXPECT_EQ("[\\] Check 2\n", j.get_incomplete()[0].to_string());
    EXPECT_EQ("[ ] Check 3\n", j.get_incomplete()[1].to_string());

    Day d1("Monday");
    TaskTime t1("task 01", 13, 00, 14, 50);
    TaskTime t2("task 2", 14, 55, 15, 30);
    d1.insert_next(t1);
    d1.insert_next(t2);

    Day d2("Tuesday");
    d2.insert_next(t1);

    j.insert_day(d1);
    j.insert_day(d2);

    EXPECT_STREQ(j.to_string().data(), 
    "Week of Jun 6\n\nTODO:\n1. [X] Check 1\n2. [\\] Check 2\n3. [ ] Check 3\n\nMonday\ntask 01     13:00 - 14:50\ntask 2      14:55 - 15:30\n\nTuesday\ntask 01     13:00 - 14:50\n\n");
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
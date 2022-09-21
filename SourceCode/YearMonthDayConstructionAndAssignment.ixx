export module YearMonthDayConstructionAndAssignment;

import <chrono>;

import <iostream>;
using std::cout, std::endl;

void date_construction();
void date_assignment();


export void ymd_construction_assignment()
{
    date_construction();
    date_assignment();
}

void date_construction()
{
    cout << endl << "*** date_construction() ***" << endl;
    std::chrono::year_month_day ymd{ std::chrono::year{2002},
        std::chrono::month{11}, std::chrono::day{14} };

    // year, month, and day are also classes in C++20 std::chrono

    std::chrono::year_month_day ymd_alt{ std::chrono::year(2002),
        std::chrono::November, std::chrono::day(14) };

    cout << "ymd = " << ymd << endl;
    cout << "ymd_alt = " << ymd_alt << endl << endl;
}

void date_assignment()
{
    cout << endl << "*** date_assignment() ***" << endl;
    auto ymd = std::chrono::year{ 2002 } / std::chrono::month{ 11 } / std::chrono::day{ 14 };
    auto ymd_mth_type = std::chrono::year{ 2002 } / 11 / 14;
    auto mdy = std::chrono::November / 14 / 2002;

    cout << "ymd = " << ymd << endl;
    cout << "ymd_mth_type = " << ymd_mth_type << endl;
    cout << "mdy = " << mdy << endl << endl;
}




export module StateChecksAndEom;

import <chrono>;
import <array>;
import <iostream>;
using std::cout, std::endl;

void date_validity();
void check_if_leap();
unsigned last_day_of_month_std_chrono(const std::chrono::year_month_day& ymd);    // std::chrono built-in functionality
export unsigned last_day_of_the_month(const std::chrono::year_month_day& ymd);     // Using algorithms (Slide 24)

export void state_checks()
{
    using namespace std::chrono;

    date_validity();
    check_if_leap();

    year_month_day ymd{ year{2002}, month{11}, day{14} };
    unsigned eom_day = last_day_of_month_std_chrono(ymd);
    cout << "Last day of month using std::chrono functionality: " << eom_day << endl;
    eom_day = last_day_of_the_month(ymd);
    cout << "Last day of month using algos on std::chrono website: " << eom_day << endl << endl;
}

void date_validity()
{
    cout << endl << "*** date_validity() ***" << endl;

    using namespace std::chrono;

    year_month_day ymd{ year{2002}, month{11}, day{14} };
    bool torf = ymd.ok();			// true

    cout << std::boolalpha;
    cout << ymd << ", " << torf << endl;

    year_month_day ymd_invalid{ year{2018}, month{2}, day{31} };
    torf = ymd_invalid.ok();		// false

    cout << ymd_invalid << ", " << torf << endl;

    year_month_day ymd_completely_bogus{ year{-2004}, month{19}, day{58} };
    torf = ymd_completely_bogus.ok();	// false

    cout << ymd_completely_bogus << ", " << torf << endl << endl;

}

void check_if_leap()
{
    cout << endl << "*** check_if_leap() ***" << endl;

    using namespace std::chrono;

    year_month_day ymd_leap{ year{2016}, month{10}, day{26} };
    bool torf = ymd_leap.year().is_leap();	// true

    cout << std::boolalpha;
    cout << ymd_leap << " is leap? " << torf << endl;

    year_month_day ymd_not_leap{ year{1973}, month{10}, day{26} };
    cout << ymd_not_leap << " is leap? " << torf << endl << endl;
}

unsigned last_day_of_month_std_chrono(const std::chrono::year_month_day& ymd)
{
    // See https://stackoverflow.com/questions/59418514/using-c20-chrono-how-to-compute-various-facts-about-a-date
    // Fact 5: Number of days in month (section)

    cout << endl << "*** last_day_of_month_last(.) ***" << endl;

    using namespace std::chrono;

    year_month_day_last 
        eom{ year{ ymd.year() } / month{ ymd.month() } / std::chrono::last };

    auto last_day = static_cast<unsigned>(eom.day());

    return last_day;
}

unsigned last_day_of_the_month(const std::chrono::year_month_day& ymd)
{
    cout << endl << "*** last_day_of_the_month() ***" << endl;

    using namespace std::chrono;

    unsigned m = static_cast<unsigned>(ymd.month());
    std::array<unsigned, 12> normal_end_dates{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    return (m != 2 || !ymd.year().is_leap() ? normal_end_dates[m - 1] : 29);
}
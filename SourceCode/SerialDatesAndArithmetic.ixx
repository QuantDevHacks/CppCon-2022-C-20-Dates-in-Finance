export module SerialDatesAndArithmetic;

import <chrono>;
//import <array>;
import <iostream>;
using std::cout, std::endl;

import StateChecksAndEom;

void serial_dates();
void date_difference();
void year_month_day_accessors();
void add_months_and_years();

void add_months_algo(std::chrono::year_month_day& ymd, unsigned mths);

export void serial_dates_and_arithmetic()
{
	serial_dates();
	date_difference();
	year_month_day_accessors();
	add_months_and_years();
}


void serial_dates()
{
	cout << endl << "*** serial_dates() ***" << endl;
	auto ymd = std::chrono::year{ 2002 } / std::chrono::month{ 11 } / std::chrono::day{ 14 };
	auto ymd_before_epoch = std::chrono::year{ 1931 } / std::chrono::month{ 4 } / std::chrono::day{ 30 };

	int days_since_epoch_count =
		std::chrono::sys_days(ymd).time_since_epoch().count();

	cout << ymd << " serial date: " << days_since_epoch_count << endl;

	days_since_epoch_count =
		std::chrono::sys_days(ymd_before_epoch).time_since_epoch().count();

	cout << ymd_before_epoch << " serial date: " << days_since_epoch_count << endl << endl;

}

void date_difference()
{
	cout << endl << "*** date_difference() ***" << endl;

	using namespace std::chrono;	// Will be assumed going forward

	year_month_day ymd{ year{2002}, month{11}, day{14} };
	year_month_day ymd_later{year{2003}, month{5}, day{14}};

	int diff = (sys_days(ymd_later) - sys_days(ymd)).count();

	cout << "Number of days between " << ymd << " and " << ymd_later
		<< " is " << diff << endl << endl;
}

void year_month_day_accessors()
{
	cout << endl << "*** year_month_day_accessors() ***" << endl;

	using namespace std::chrono;

	year_month_day ymd{ year{2002}, month{11}, day{14} };
	year_month_day ymd_later{year{2003}, month{5}, day{14}};

	auto ymd_year = ymd.year();		// std::chrono::year object
	auto ymd_month = ymd.month();	// std::chrono::year object
	auto ymd_day = ymd.day();		// std::chrono::year object

	// year can be cast to an int; month and day to unsigned:
	auto year_val = static_cast<int>(ymd_year);
	auto month_val = static_cast<unsigned>(ymd_month);
	auto day_val = static_cast<unsigned>(ymd_day);

	// Casting gets messy, especially for months and days:
	int year_diff = static_cast<int>(ymd_later.year()) - static_cast<int>(ymd.year());

	int month_diff = static_cast<int>(static_cast<unsigned>(ymd_later.month())) 
		- static_cast<int>(static_cast<unsigned>(ymd.month()));

	int day_diff = static_cast<int>(static_cast<unsigned>(ymd_later.day())) 
		- static_cast<int>(static_cast<unsigned>(ymd.day()));

	cout << "Year diff = " << year_diff << ", type = " << typeid(year_diff).name() << ", ";
	cout << "Month diff = " << month_diff << ", type = " << typeid(month_diff).name() << ", ";
	cout << "Day diff = " << day_diff << ", type = " << typeid(day_diff).name() << endl << endl;
}

void add_months_and_years()
{
	cout << endl << "*** add_months_and_years() ***" << endl;

	using namespace std::chrono;

	year_month_day ymd{ year{2002}, month{11}, day{14} };
	cout << "Original day: " << ymd << endl;
	ymd += std::chrono::months(1);	// Result: 2002-12-14
	cout << "Add 1 month: " << ymd << endl;
	ymd += std::chrono::months(18);	// Result: 2004-06-14
	cout << "Add 18 months: " << ymd << endl;
	ymd += std::chrono::years(2);	// Result: 2006-06-14
	cout << "Add 2 years: " << ymd << endl << endl;

	cout << "Add months => result in invalid dates: " << endl;
	// 2015-01-31
	year_month_day ymd_eom_1{ year{2015}, month{1}, day{31} };

	// 2014-08-31
	year_month_day ymd_eom_2{ year{2014}, month{8}, day{31} };
	auto ymd_eom_2_copy = ymd_eom_2;

	cout << "Original dates: " << ymd_eom_1 << ", " << ymd_eom_2 << endl;

	ymd_eom_1 += months{ 1 };  // 2015-02-31 is not a valid date
	cout << "Add 1 month to ymd_eom_1: " << ymd_eom_1 << endl;
	ymd_eom_2 += months{ 1 };  // 2014-09-31 is not a valid date 
	cout << "Add 1 month to ymd_eom_2: " << ymd_eom_2 << endl << endl;

	cout << "Adjust 1st date using std::chrono::year_month_day_last: " << endl;
	year_month_day_last
		eom{ year{ ymd_eom_1.year() } / month{ ymd_eom_1.month() } / std::chrono::last };
	ymd_eom_1 = eom;
	cout << "Adjusted 1st date: " << ymd_eom_1 << endl << endl;

	cout << "Add one month to 2nd date using add_months_algo(.): " << endl;
	add_months_algo(ymd_eom_2_copy, 1);
	cout << "Adjusted 2nd date: " << ymd_eom_2_copy << endl << endl;

}

void add_months_algo(std::chrono::year_month_day& ymd, unsigned mths)
{
	using namespace std::chrono;

	ymd += months(mths);    // Naively attempt the addition

	if (!ymd.ok())
	{
		ymd = ymd.year() / ymd.month() / day{ last_day_of_the_month(ymd) };
	}
}


//unsigned last_day_of_the_month(const std::chrono::year_month_day& ymd)
//{
//	unsigned m = static_cast<unsigned>(ymd.month());
//	std::array<unsigned, 12> normal_end_dates{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
//	return (m != 2 || !ymd.year().is_leap() ? normal_end_dates[m - 1] : 29);
//}

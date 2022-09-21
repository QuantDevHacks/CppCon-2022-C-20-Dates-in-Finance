/*
	Copyright 2022 Daniel Hanson (BSD-3)

	Redistribution and use in source and binary forms, 
	with or without modification, are permitted provided that 
	the following conditions are met:

	1. Redistributions of source code must retain the above 
	   copyright notice, this list of conditions and the following 
	   disclaimer.

	2. Redistributions in binary form must reproduce the above 
	   copyright notice, this list of conditions and the following 
	   disclaimer in the documentation and/or other materials 
	   provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its 
	   contributors  may be used to endorse or promote products 
	   derived from this software without specific prior written 
	   permission.

	 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND 
	 CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
	 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
	 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
	 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
	 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
	 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
	 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
	 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
	 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
	 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
	 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 	 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
	 OF SUCH DAMAGE.

*/

export module ChronoDate;
import <chrono>;
namespace date = std::chrono;
using Date = std::chrono::year_month_day;

import <compare>;
import <exception>;
import <array>;

export class ChronoDate
{
public:
	ChronoDate(int year, unsigned month, unsigned day);
	ChronoDate(const Date& ymd);
	ChronoDate();

	// Accessors:
	int year() const;
	unsigned month() const;
	unsigned day() const;
	int serial_date() const;
	date::year_month_day ymd() const;	

	// Modifying member functions:
	ChronoDate& add_years(int rhs_years);
	ChronoDate& add_months(int rhs_months);
	ChronoDate& add_days(int rhs_days);		
	ChronoDate& weekend_roll();

	// Operators
	unsigned operator - (const ChronoDate& rhs) const;
	bool operator == (const ChronoDate& rhs) const;
	std::strong_ordering operator <=> (const ChronoDate& rhs) const;

	// Check state:
	bool end_of_month() const;
	unsigned days_in_month() const;
	bool leap_year() const;

	// friend operator so that we can output date details with cout
	friend std::ostream& operator << (std::ostream& os, const ChronoDate& rhs);

private:
	date::year_month_day date_;
	int serial_date_;

	void reset_serial_date_();
};

ChronoDate::ChronoDate(int year, unsigned month, unsigned day) : 
	date_{ date::year{year} / date::month{month} / date::day{day} }
{
	if(!date_.ok())		// std::chrono member function to check if valid date
	{
		std::exception e("ChronoDate constructor: Invalid date.");
		throw e;		
	}

	reset_serial_date_();
}

ChronoDate::ChronoDate(const Date& ymd) : date_{ymd}
{
	if (!date_.ok())		// std::chrono member function to check if valid date
	{
		std::exception e("ChronoDate constructor: Invalid year_month_day input.");
		throw e;
	}

	reset_serial_date_();
}

ChronoDate::ChronoDate():date_{date::year(1970), date::month{1}, date::day{1} }, 
	serial_date_{0} { }

// chrono::year can be cast to int
// See https://en.cppreference.com/w/cpp/chrono/year/operator_int
int ChronoDate::year() const
{
	return static_cast<int>(date_.year());
}

// chrono::month can be cast to unsigned
// Note this is different from chrono::year (int)
// See https://en.cppreference.com/w/cpp/chrono/month/operator_unsigned
unsigned ChronoDate::month() const
{
	return static_cast<unsigned>(date_.month());
}

// chrono::day can be cast to unsigned
// Note this is different from chrono::year (int)
// See https://en.cppreference.com/w/cpp/chrono/day/operator_unsigned
unsigned ChronoDate::day() const
{
	return static_cast<unsigned>(date_.day());
}

int ChronoDate::serial_date() const
{
//	return date::sys_days(date_).time_since_epoch().count();
	return serial_date_;
}

std::chrono::year_month_day ChronoDate::ymd() const
{
	return date_;
}

ChronoDate& ChronoDate::add_years(int rhs_years)
{
	// Proceed naively: 
	date_ += date::years(rhs_years);

	// Only possible error case is if month is February
	// and the result is day = 29 in a non-leap year.

	if (!date_.ok())
	{
		date_ = date_.year() / date_.month() / 28;
	}

	reset_serial_date_();

	return *this;
}

ChronoDate& ChronoDate::add_months(int rhs_months)
{	
	date_ += date::months(rhs_months);    // Naively attempt the addition

	// If the date is invalid, it is because the
	// result is an invalid end-of-month:
	if (!date_.ok())
	{
		date_ = date_.year() / date_.month() / date::day{ days_in_month() };
	}

	reset_serial_date_();

	return *this;
}

ChronoDate& ChronoDate::add_days(int rhs_days)
{
	// Note that adding days is handled differently, per Howard Hinnant's Stack Overflow comments.
	// See https://stackoverflow.com/questions/62734974/how-do-i-add-a-number-of-days-to-a-date-in-c20-chrono

	date_ = date::sys_days(date_) + date::days(rhs_days);

	if (!date_.ok())
	{
		std::exception e("ChronoDate::ChronoDate::addDays(.): resulting date out of range.");
		throw e;
	}

	reset_serial_date_();

	return *this;
}

// ROLL OVER WEEKEND 
ChronoDate& ChronoDate::weekend_roll()
{
	date::weekday wd{ date::sys_days(date_) };
	date::month orig_mth{ date_.month() };
		
	unsigned wdn{ wd.iso_encoding() }; // Mon =  1, ..., Sat = 6, Sun = 7
	if (wdn > 5) date_  = date::sys_days(date_) + date::days(8-wdn);

	// If eom, roll back.
	// Note: Need to handle January as well.
	if (orig_mth < date_.month()|| (orig_mth == date::December && date_.month() == date::January))
	{
		date_ = date::sys_days(date_) - date::days(3);
	}

	reset_serial_date_();	// must be a valid date so no need to check

	return *this;
}

unsigned ChronoDate::operator - (const ChronoDate& rhs) const
{
	// By using the serial dates, we can avoid
	// these function calls:
	// return (date::sys_days(date_).time_since_epoch()
	//	- date::sys_days(rhs.date_).time_since_epoch()).count();

	return this->serial_date_ - rhs.serial_date_;
}

bool ChronoDate::operator == (const ChronoDate& rhs) const
{
	return date_ == rhs.date_;
}

std::strong_ordering ChronoDate::operator <=> (const ChronoDate& rhs) const
{
	return date_ <=> rhs.date_;
}


bool ChronoDate::end_of_month() const
{
	return date_ == date_.year() / date_.month() / date::last;
}


unsigned ChronoDate::days_in_month() const
{
	// The code below is an adaption of the algorithm on the 
	// std::chrono GitHub site:  https://howardhinnant.github.io/date_algorithms.html#last_day_of_month

	unsigned m = static_cast<unsigned>(date_.month());
	std::array<unsigned, 12> normal_end_dates{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	return (m != 2 || !date_.year().is_leap() ? normal_end_dates[m - 1] : 29);
}

bool ChronoDate::leap_year() const
{
	return date_.year().is_leap();
}

void ChronoDate::reset_serial_date_()		
{
	serial_date_ = date::sys_days(date_).time_since_epoch().count();
}

// This is a 'friend' of the ChronoDate class; also
// requires separate export keyword even though the 
// class declaration is export(ed).
export std::ostream& operator << (std::ostream& os, const ChronoDate& rhs)
{
	os << rhs.ymd();
	return os;
}

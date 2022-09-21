# CppCon 2022:  C++20-Dates in Finance

This repository contains files related to the CppCon 2022 talk _Using_ `std::chrono` _Calendar Dates for Finance_, presented by Daniel Hanson.

The original slide deck is provided, along with a corrected version.  There were two errors.  First, on slide 19, the original stated that taking the difference between `std::chrono::year`, `std::chrono::month`, or `std::chrono::day` objects would result in integer type returns.  This is incorrect.  These objects need to be cast to `int` types first, with `month` and `day` types first cast to `unsigned` types, as shown in the corrected version.

Second, on slide 37, contract dates will always be proper business dates, so the `weekend_roll()` function will only apply to regular transaction (payment) dates.

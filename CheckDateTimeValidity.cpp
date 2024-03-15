#include "CheckDateTimeValidity.h"

#include <array>
#include <stdexcept>
#include <string>
#include <string_view>

using namespace std::literals;

enum class CheckResult {
    OK,
    SMALL,
    BIG
};

CheckResult CheckValidity(int value, int min, int max) {
    if (value < min) {
        return CheckResult::SMALL;
    } else if (value > max) {
        return CheckResult::BIG;
    } else {
        return CheckResult::OK;
    }
}

std::pair<CheckResult, std::string_view> CheckValidity(const DateTime& dt) {
    CheckResult result = CheckValidity(dt.year, 1, 9999);
    if (result != CheckResult::OK) {
        return {result, "year"sv};
    }

    result = CheckValidity(dt.month, 1, 12);
    if (result != CheckResult::OK) {
        return {result, "month"sv};
    }

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const std::array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    result = CheckValidity(dt.day, 1, month_lengths[dt.month - 1]);
    if (result != CheckResult::OK) {
        return {result, "day"sv};
    }

    result = CheckValidity(dt.hour, 0, 23);
    if (result != CheckResult::OK) {
        return {result, "hour"sv};
    }

    result = CheckValidity(dt.minute, 0, 59);
    if (result != CheckResult::OK) {
        return {result, "minute"sv};
    }

    result = CheckValidity(dt.second, 0, 59);
    if (result != CheckResult::OK) {
        return {result, "second"sv};
    }

    return {CheckResult::OK, "all"sv};
}

std::string DateTimeErrorMessage(CheckResult check_result, std::string_view name) {
    if (check_result == CheckResult::SMALL) {
        return std::string(name) + " is too small"s;
    } else if (check_result == CheckResult::BIG) {
        return std::string(name) + " is too big"s;
    } else {
        return ""s;
    }
}

void CheckDateTimeValidity(const DateTime& dt) {
    std::pair<CheckResult, std::string_view> result = CheckValidity(dt);
    if (result.first != CheckResult::OK) {
        throw std::domain_error(DateTimeErrorMessage(result.first, result.second));
    }
}
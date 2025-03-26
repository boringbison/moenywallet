#include "validate.h"
#include <regex>

bool isValidPassword(const std::string &password)
{
    return password.length() >= 6;
}

bool isValidPhone(const std::string &phone)
{
    return std::regex_match(phone, std::regex("^\\d{9,11}$"));
}

bool isValidEmail(const std::string &email)
{
    return std::regex_match(email, std::regex("^[\\w.-]+@[\\w.-]+\\.\\w+$"));
}

bool isValidBirthday(const std::string &birthday)
{
    return std::regex_match(birthday, std::regex("^(\\d{2}/\\d{2}/\\d{4}|\\d{4}-\\d{2}-\\d{2})$"));
}

bool isValidFullName(const std::string &name)
{
    if (name.empty())
        return false;

    // Regex: chỉ cho phép chữ cái (có dấu hoặc không), dấu cách
    std::regex pattern("^[A-Za-zÀ-ỹà-ỹ\\s']+$");

    return std::regex_match(name, pattern); // bạn có thể kiểm tra không chứa số nếu muốn
}

bool isValidBalance(const std::optional<double> &balance)
{
    return !balance.has_value() || balance.value() >= 0;
}

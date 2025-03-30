#include "utils.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

std::string getCurrentDateTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm *timeinfo = std::localtime(&now_time);

    std::ostringstream oss;
    oss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

bool isEmpty(const UserUpdateData &data)
{
    return data.password.empty() &&
           data.fullName.empty() &&
           data.phone.empty() &&
           data.address.empty() &&
           data.email.empty() &&
           data.birthday.empty() &&
           !data.balance.has_value();
}



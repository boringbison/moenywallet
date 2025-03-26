#ifndef MENU_H
#define MENU_H

#include <string>
#include <optional>

void handleMenu();
std::pair<std::string, std::string> getLoginInput();

struct UserUpdateData
{
    std::string password;
    std::string fullName;
    std::string phone;
    std::string address;
    std::string email;
    std::string birthday;
    std::optional<double> balance;
};

UserUpdateData inputUserData();

#endif

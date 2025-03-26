#ifndef VALIDATE_H
#define VALIDATE_H

#include <string>
#include <optional>

bool isValidPassword(const std::string &password);
bool isValidPhone(const std::string &phone);
bool isValidEmail(const std::string &email);
bool isValidBirthday(const std::string &birthday);
bool isValidFullName(const std::string &name);
bool isValidBalance(const std::optional<double> &balance);

#endif

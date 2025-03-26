#ifndef LOGIN_H
#define LOGIN_H
#include "user.h"
#include <optional>
#include <string>

std::optional<User> loginUser(const std::string &username, const std::string &password);

#endif

#ifndef UPDATE_USER_H
#define UPDATE_USER_H

#include <string>

// adminMode = true thì không cần nhập OTP
void updateUserInfo(const std::string &username, bool adminMode = false);
#endif

#ifndef USER_H
#define USER_H

#include <string>

struct User {
    std::string username;
    std::string password;
    std::string role;
    int balance;
    int user_id;
    bool is_admin;
};

#endif


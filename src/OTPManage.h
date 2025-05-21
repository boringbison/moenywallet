#ifndef MENU_H
#define MENU_H

#include <string>
#include <ctime>

class OTPManager {
private:
    std::string currentOTP;
    time_t otpGeneratedTime;
    int otpLength;
    int expireSeconds;

public:
    OTPManager(int length = 6, int expire = 30);

    std::string generateOTP();
    bool checkOTP(const std::string& inputOTP);
};

#endif

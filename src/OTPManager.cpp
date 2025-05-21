#include "otp.h"
#include <cstdlib>
#include <ctime>

OTPManager::OTPManager(int length, int expire) {
    otpLength = length;
    expireSeconds = expire;
    currentOTP = "";
    otpGeneratedTime = 0;
}

std::string OTPManager::generateOTP() {
    std::string otp;
    srand(time(0));

    for (int i = 0; i < otpLength; ++i) {
        int digit = rand() % 10;
        otp += std::to_string(digit);
    }

    currentOTP = otp;
    otpGeneratedTime = time(0);

    return otp;
}

bool OTPManager::checkOTP(const std::string& inputOTP) {
    time_t now = time(0);
    double secondsPassed = difftime(now, otpGeneratedTime);

    if (secondsPassed > expireSeconds) {
        return false;
    }

    return inputOTP == currentOTP;
}

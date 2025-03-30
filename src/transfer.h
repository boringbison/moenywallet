#ifndef TRANSFER_H
#define TRANSFER_H

#include <iostream>
#include <string>
#include "user.h"  // Chỉ cần include thay vì khai báo lại
#include "OpenXLSX.hpp"

using namespace std;
using namespace OpenXLSX;

// Tìm user trong sheet
User findUser(XLWorksheet& sheet, const std::string& username);

// Ghi log giao dịch
void logTransaction(XLWorksheet& logSheet, int userId, const std::string& action, 
                    int oldValue, int newValue, int senderId, int recipientId);

// Thực hiện giao dịch
void transferPoints(const std::string& filePathUser, const std::string& filePathLog);

#endif

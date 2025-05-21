#include <iostream>
#include <OpenXLSX.hpp>
#include <string>
#include "user.h"
#include "hash_utils.h"
#include "validate.h"
#include "utils.h"
#include "transfer.h"

using namespace OpenXLSX;
using namespace std;

bool isUsernameTaken(XLWorksheet &sheet, const string &username)
{
    int row = 2;
    while (sheet.cell(row, 2).value().type() != XLValueType::Empty)
    {
        auto cell = sheet.cell(row, 2);
        std::string existingUsername;

        if (cell.value().type() == XLValueType::String)
        {
            existingUsername = cell.value().get<std::string>();
        }
        else if (cell.value().type() == XLValueType::Integer)
        {
            existingUsername = std::to_string(cell.value().get<int>());
        }
        else
        {
            existingUsername = "";
        }

        if (existingUsername == username)
        {
            return true;
        }
        row++;
    }
    return false;
}

void addUser(const string &filePath)
{
    XLDocument doc;
    doc.open(filePath);
    auto sheet = doc.workbook().worksheet("Sheet1");

    // Tìm dòng trống đầu tiên
    int newRow = 2;
    while (sheet.cell(newRow, 1).value().type() != XLValueType::Empty)
    {
        ++newRow;
    }

    // Tự động tạo ID
    int newID = 1;
    if (newRow > 2)
    {
        auto lastIDCell = sheet.cell(newRow - 1, 1);
        if (lastIDCell.value().type() == XLValueType::Integer)
        {
            newID = lastIDCell.value().get<int>() + 1;
        }
    }

    // Nhập dữ liệu người dùng
    string username, email, password, fullName, phone;

    while (true)
    {
        cout << "Nhap username: ";
        cin >> username;
        if (isUsernameTaken(sheet, username))
        {
            cout << "⚠ Username da ton tai. Vui long nhap username khac.\n";
        }
        else
        {
            break;
        }
    }

    // Password
    while (true)
    {
        cout << "Nhap mat khau (>=6 ky tu): ";
        cin >> password;
        cin.ignore();
        if (!isValidPassword(password))
        {
            cout << "⚠ Mat khau qua ngan.\n";
        }
        else
        {
            break;
        }
    }

    // Ho ten
    cout << "Nhap ho ten: ";
    getline(cin, fullName);
    while (!isValidFullName(fullName))
    {
        cout << "⚠ Ho ten khong hop le. Nhap lai: ";
        getline(cin, fullName);
    }

    // Email
    cout << "Nhap email: ";
    getline(cin, email);
    while (!isValidEmail(email))
    {
        cout << "⚠ Email khong hop le. Nhap lai: ";
        getline(cin, email);
    }

    // So dien thoai
    cout << "Nhap so dien thoai: ";
    getline(cin, phone);
    while (!isValidPhone(phone))
    {
        cout << "⚠ So dien thoai khong hop le. Nhap lai: ";
        getline(cin, phone);
    }

    string hashedPassword = hashPassword(password);
    string defaultRole = "user";

    double initialBalance = 500.0;
    // Trừ điểm từ ví hệ thống ID = 1(row 2, col 11)

    int systemRow = 2;
    int balanceCol = 11;

    auto sysBalanceCell = sheet.cell(systemRow, balanceCol);
    if (sysBalanceCell.value().type() != XLValueType::Float && sysBalanceCell.value().type() != XLValueType::Integer)
    {
        cout << "❌ Lỗi: Không đọc được số dư từ ví hệ thống (ID 1).\n";
        doc.close();
        return;
    }

    double sysBalance = sysBalanceCell.value().get<double>();
    if (sysBalance < initialBalance)
    {
        cout << "❌ Ví hệ thống không đủ điểm để cấp cho tài khoản mới.\n";
        doc.close();
        return;
    }

    // Trừ điểm hệ thống và cấp cho user mới
    sheet.cell(systemRow, balanceCol).value() = sysBalance - initialBalance;

    // Ghi vào Excel
    sheet.cell(newRow, 1).value() = newID;
    sheet.cell(newRow, 2).value() = username;
    sheet.cell(newRow, 3).value() = hashedPassword;
    sheet.cell(newRow, 4).value() = fullName;
    sheet.cell(newRow, 5).value() = phone;
    sheet.cell(newRow, 6).value() = "";
    sheet.cell(newRow, 7).value() = email;
    sheet.cell(newRow, 8).value() = "01/01/2000";
    sheet.cell(newRow, 9).value() = getCurrentDateTime();
    sheet.cell(newRow, balanceCol).value() = initialBalance;
    sheet.cell(newRow, 13).value() = 0; // is_admin mặc định
    sheet.cell(newRow, 14).value() = 1; // is_active mặc định
    sheet.cell(newRow, 15).value() = defaultRole;

    // Ghi log giao dịch nạp điểm
    XLDocument docLog;
    docLog.open("../data/logs.xlsx");
    auto logSheet = docLog.workbook().worksheet("Sheet1");

    // Ghi log cho ví hệ thống (bị trừ)
    logTransaction(
        logSheet,
        systemRow - 1, // ID giao dịch cho dòng 2 (ID 1)
        "Trừ điểm để nạp tài khoản mới",
        sysBalance,
        sysBalance - initialBalance,
        systemRow,
        newRow);

    // Ghi log cho tài khoản mới (được nạp)
    logTransaction(
        logSheet,
        newRow - 1, // ID giao dịch cho user mới
        "Nạp điểm khi tạo tài khoản",
        0,
        initialBalance,
        systemRow,
        newRow);

    cout << "Da them nguoi dung thanh cong! ID = " << newID << endl;

    doc.save();
    doc.close();

    docLog.save();
    docLog.close();
}

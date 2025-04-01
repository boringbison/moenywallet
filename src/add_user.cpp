#include <iostream>
#include <OpenXLSX.hpp>
#include <vector>
#include <string>
#include "user.h"

using namespace OpenXLSX;
using namespace std;

void addUser(const string& filePath, bool isAdmin) {
    XLDocument doc;
    doc.open(filePath);
    auto sheet = doc.workbook().worksheet("Sheet1");

    int newRow = 6;
    while (sheet.cell(newRow, 1).value().type() != XLValueType::Empty) ++newRow;

    cout << "nhap ID: ";
    int id;
    cin >> id;
    cout << "nhap username: ";
    string username;
    cin >> username;
    cout << "Nhap email: ";
    string email;
    cin >> email;
    string role = isAdmin ? "admin" : "user";

    sheet.cell(newRow, 1).value() = id;
    sheet.cell(newRow, 2).value() = username;
    sheet.cell(newRow, 3).value() = email;
    sheet.cell(newRow, 4).value() = role;

    cout << "da them nguoi dung!" << endl;

    doc.save();
    doc.close();
}

void deleteUser(const string& filePath, const string& targetUser, bool isAdmin) {
    XLDocument doc;
    doc.open(filePath);
    auto sheet = doc.workbook().worksheet("User");
    bool found = false;

    for (int row = 2; sheet.cell(row, 2).value().type() != XLValueType::Empty; ++row) {
        if (sheet.cell(row, 2).value().get<string>() == targetUser) {
            string role = sheet.cell(row, 4).value().get<string>();
            if (!isAdmin && role == "admin") {
                cout << "Bam khong the xoa tai khoan admin!" << endl;
                return;
            }

            found = true;
            cout << "da xoa: " << targetUser << endl;
            for (int col = 1; col <= 4; ++col) {
                sheet.cell(row, col).value().clear();
            }
            break;
        }
    }
    if (!found) cout << "Khong tim thay!" << endl;

    doc.save();
    doc.close();
}
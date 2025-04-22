#include <iostream>
#include <OpenXLSX.hpp>
#include <vector>
#include <string>
#include "user.h"

using namespace OpenXLSX;
using namespace std;

void addUser(const string& filePath, const string& role) {
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
    

    sheet.cell(newRow, 1).value() = id;
    sheet.cell(newRow, 2).value() = username;
    sheet.cell(newRow, 3).value() = email;
    sheet.cell(newRow, 4).value() = role;

    cout << "da them nguoi dung!" << endl;

    doc.save();
    doc.close();
}
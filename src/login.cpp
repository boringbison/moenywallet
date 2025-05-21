#include "login.h"
#include "user_column_index.h"
#include "user.h"
#include <OpenXLSX.hpp>
#include <iostream>
#include <optional>
#include "hash_utils.h"

using namespace std;
using namespace OpenXLSX;

optional<User> loginUser(const string &username, const string &password)
{
    try
    {
        int retryCount = 0;
        const int maxRetries = 10;

        while (true)
        {
            XLDocument doc;
            doc.open("../data/users.xlsx");
            auto wks = doc.workbook().worksheet("Sheet1");

            int row = 2;
            bool foundMatch = false;

            while (true)
            {
                auto userCellRaw = wks.cell(row, COL_USERNAME);

                if (userCellRaw.value().type() == XLValueType::Empty)
                {
                    break; // hết dòng dữ liệu
                }

                auto passCellRaw = wks.cell(row, COL_PASSWORD);
                auto roleCellRaw = wks.cell(row, COL_ROLE);
                auto balanceCellRaw = wks.cell(row, COL_BALANCE);
                auto isAdminCellRaw = wks.cell(row, COL_IS_ADMIN);
                auto isActiveCellRaw = wks.cell(row, COL_IS_ACTIVE);

                if (passCellRaw.value().type() == XLValueType::Empty)
                {
                    std::cerr << "Dữ liệu mật khẩu bị rỗng tại dòng " << row << ". Bỏ qua.\n";
                    row++;
                    continue;
                }

                auto &userCell = userCellRaw.value();
                auto &passCell = passCellRaw.value();
                auto &roleCell = roleCellRaw.value();
                auto &balanceCell = balanceCellRaw.value();
                auto &isAdminCell = isAdminCellRaw.value();

                // std::cout << "[DEBUG] userCell: '" << userCell << "'\n";
                // std::cout << "[DEBUG] passCell: '" << passCell << "'\n";

                string storedUsername = (userCell.type() == XLValueType::String)
                                            ? userCell.get<std::string>()
                                        : (userCell.type() == XLValueType::Integer) ? std::to_string(userCell.get<int>())
                                                                                    : "";
                string storedPassword = (passCell.type() == XLValueType::String)
                                            ? passCell.get<std::string>()
                                        : (passCell.type() == XLValueType::Integer) ? std::to_string(passCell.get<int>())
                                                                                    : "";

                // std::cout << "[DEBUG] storedUsername: '" << storedUsername << "'\n";
                // std::cout << "[DEBUG] storedPassword: '" << hashPassword(password) << "'\n";

                if (storedUsername.empty() || storedPassword.empty())
                {
                    row++;
                    continue;
                }

                string storedRole = (roleCell.type() == XLValueType::String) ? roleCell.get<string>() : "user";
                transform(storedRole.begin(), storedRole.end(), storedRole.begin(), ::tolower);
                storedRole.erase(remove_if(storedRole.begin(), storedRole.end(), ::isspace), storedRole.end());

                // So sánh tên đăng nhập và mật khẩu (đã hash)
                if (storedUsername == username && storedPassword == hashPassword(password))
                {
                    // Kiểm tra is_active
                    bool isActive = true;
                    if (isActiveCellRaw.value().type() == XLValueType::Integer)
                        isActive = isActiveCellRaw.value().get<int>() == 1;
                    else if (isActiveCellRaw.value().type() == XLValueType::Boolean)
                        isActive = isActiveCellRaw.value().get<bool>();
                    else
                        isActive = false;

                    if (!isActive)
                    {
                        cerr << "Tài khoản này đã bị vô hiệu hóa. Không thể đăng nhập.\n";
                        doc.close();
                        return nullopt;
                    }

                    // Nếu hợp lệ và còn hoạt động, tạo user object
                    User user;
                    user.username = storedUsername;
                    user.password = storedPassword;
                    user.role = storedRole;
                    user.user_id = row; // dùng dòng Excel hiện tại làm user_id

                    try
                    {
                        if (balanceCell.type() == XLValueType::Float)
                            user.balance = static_cast<int>(balanceCell.get<double>());
                        else
                            user.balance = stoi(balanceCell.get<string>());
                    }
                    catch (...)
                    {
                        user.balance = 0;
                    }

                    user.is_admin = (isAdminCell.type() == XLValueType::Integer) ? isAdminCell.get<int>() == 1 : false;

                    cout << " >>> Đăng nhập thành công!" << "\n";
                    doc.close();
                    return user;
                }

                row++;
            }

            retryCount++;
            if (retryCount > maxRetries)
            {
                cerr << "Tên đăng nhập hoặc mật khẩu không đúng. Vui lòng thử lại.\n";
                return nullopt;
            }

            // std::cout << "Tên đăng nhập hoặc mật khẩu không đúng. Vui lòng thử lại. (" << retryCount << "/" << maxRetries << ")\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

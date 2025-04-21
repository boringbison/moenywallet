#include "login.h"
#include "user.h"
#include <OpenXLSX.hpp>
#include <iostream>
#include <optional>

using namespace std;
using namespace OpenXLSX;

std::optional<User> loginUser(const std::string &username, const std::string &password)
{
    try
    {
        int retryCount = 0;
        const int maxRetries = 3;

        while (true)
        {
            XLDocument doc;
            doc.open("../data/users.xlsx");
            auto wks = doc.workbook().worksheet("Sheet1");

            int row = 2;
            bool foundMatch = false;

            while (true)
            {
                auto &userCell = wks.cell("B" + std::to_string(row)).value();
                auto &passCell = wks.cell("C" + std::to_string(row)).value();
                auto &roleCell = wks.cell("O" + std::to_string(row)).value();
                auto &balanceCell = wks.cell("K" + std::to_string(row)).value();
                auto &idCell = wks.cell("A" + std::to_string(row)).value();
                auto &isAdminCell = wks.cell("M" + std::to_string(row)).value();

                // Nếu user hoặc pass trống → reset
                if (userCell.type() == XLValueType::Empty || passCell.type() == XLValueType::Empty)
                {
                    std::cerr << "Phát hiện ô dữ liệu trống tại dòng " << row << ". Reset lại vòng lặp từ đầu...\n";
                    doc.close();
                    break;  // ra khỏi vòng while nội → thực hiện retry ở vòng ngoài
                }

                       // Log sau chuẩn hóa
                std::cout << "[DEBUG] userCell: '" << userCell << "'\n";
                std::cout << "[DEBUG] passCell: '" << passCell << "'\n";

                std::string storedUsername = (userCell.type() == XLValueType::String)
                                                ? userCell.get<std::string>()
                                                : (userCell.type() == XLValueType::Integer) ? std::to_string(userCell.get<int>()) : "";
                std::string storedPassword = (passCell.type() == XLValueType::String)
                                                ? passCell.get<std::string>()
                                                : (passCell.type() == XLValueType::Integer) ? std::to_string(passCell.get<int>()) : "";

                                                                                                
                // Log sau chuẩn hóa
                std::cout << "[DEBUG] storedUsername: '" << storedUsername << "'\n";
                std::cout << "[DEBUG] storedPassword: '" << storedPassword << "'\n";


                if (storedUsername.empty() || storedPassword.empty())
                {
                    row++;
                    continue;
                }

                std::string storedRole = (roleCell.type() == XLValueType::String) ? roleCell.get<std::string>() : "user";
                std::transform(storedRole.begin(), storedRole.end(), storedRole.begin(), ::tolower);
                storedRole.erase(remove_if(storedRole.begin(), storedRole.end(), ::isspace), storedRole.end());

                if (storedUsername == username && storedPassword == password)
                {
                    User user;
                    user.username = storedUsername;
                    user.password = storedPassword;
                    user.role = storedRole;
                    user.user_id = (idCell.type() == XLValueType::Integer) ? idCell.get<int>() : 0;

                    try
                    {
                        if (balanceCell.type() == XLValueType::Float)
                            user.balance = static_cast<int>(balanceCell.get<double>());
                        else
                            user.balance = stoi(balanceCell.get<std::string>());
                    }
                    catch (...)
                    {
                        user.balance = 0;
                    }

                    user.is_admin = (isAdminCell.type() == XLValueType::Integer) ? isAdminCell.get<int>() == 1 : false;

                    std::cout << " >>> Đăng nhập thành công! Role: " << user.role << ", ID: " << user.user_id << "\n";
                    doc.close();
                    return user;
                }

                row++;
            }

            // Nếu duyệt hết mà không return → thất bại
            retryCount++;
            if (retryCount > maxRetries)
            {
                std::cerr << "Quá số lần đăng nhập tối đa. Thoát.\n";
                return std::nullopt;
            }

            std::cout << "Tên đăng nhập hoặc mật khẩu không đúng. Vui lòng thử lại. (" << retryCount << "/" << maxRetries << ")\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

# 📘 Hệ thống quản lý ví điểm thưởng

## 🧾 Giới thiệu dự án

Đây là hệ thống dòng lệnh viết bằng **C++**, cho phép người dùng đăng ký, đăng nhập, quản lý thông tin tài khoản và thực hiện các giao dịch điểm thưởng giữa các ví. Dữ liệu được lưu trữ bằng **file Excel (.xlsx)** thông qua thư viện **OpenXLSX**. Giao diện sử dụng dòng lệnh đơn giản, thân thiện, phù hợp với mô hình bài tập lớn học phần C++.

## 👨‍💻 Thành viên thực hiện

| Họ tên          | MSSV    | Vai trò/Phân công                                                                            |
| --------------- | ------- | -------------------------------------------------------------------------------------------- |
| Nguyễn Huy Hoàn | K24DTCN | Trưởng nhóm / phân tích, thiết kế hệ thống, cập nhật OTP, fix bug                            |
| Lê Thanh Nguyên | K24DTCN | Thành viên / đăng nhập, đăng xuất, cập nhật thông tin cá nhân, phân luồng admin user, fixbug |
| Trần Anh Đức    | K24DTCN | Thành viên / đăng ký, xóa tài khoản                                                          |
| Hà Ngọc Quang   | K24DTCN | qThành viên / quản lý ví, giao dịch điểm, ghi nhận log giao dịch                             |

> 💡 Dự án được thực hiện cá nhân. Mọi commit thể hiện rõ tiến độ từng phần trên GitHub.

📌 Các chức năng đã hoàn thành

1.  Hệ thống đăng nhập & đăng ký

- ✅ Đăng ký tài khoản mới
- ✅ Admin có thể tạo tài khoản hộ
- ✅ Mã hóa mật khẩu bằng SHA256 trước khi lưu vào `users.xlsx`
- ✅ Phân biệt user / admin, lưu vai trò và trạng thái hoạt động

2.  Cập nhật & quản lý thông tin

- ✅ Người dùng có thể xem, cập nhật thông tin cá nhân
- ✅ Thông tin được lưu vào file Excel
- ✅ OTP được yêu cầu khi cập nhật thông tin quan trọng

3.  Giao dịch & ví điểm

- ✅ Mỗi user có 1 ví riêng (ID ví duy nhất)
- ✅ Chuyển điểm giữa các ví (giao dịch atomic)
- ✅ Kiểm tra số dư trước khi giao dịch điểm
- ✅ Ghi log giao dịch vào `logs.xlsx`
- ✅ Chặn tự chuyển điểm cho chính mình

4.  Phân quyền rõ ràng

- ✅ Người dùng thường: chỉ chỉnh sửa, xóa, thêm thông tin, xem ví, giao dịch điểm cá nhân
- ✅ Admin: thêm, xóa người dùng, xem toàn bộ danh sách, ví điểm, giao dịch tổng toàn bộ hệ thống

🧩 Cấu trúc thư mục

```
moenywallet/
├── include/         # Header files
├── src/             # Source files (.cpp)
├── data/            # users.xlsx, logs.xlsx
├── build/           # Build output
├── CMakeLists.txt   # File cấu hình CMake
└── README.md        # Tài liệu mô tả dự án
```

🧱 Cách build & chạy

⚙️ Yêu cầu

- C++17 trở lên
- VS Code, CMake, OpenXLSX đã cấu hình
- MinGW hoặc compiler tương thích

▶️ Build & chạy (trên terminal)

```bash
cmake -S . -B build
cmake --build build
./build/moenywallet
```

## 🗃️ Các file dữ liệu

| File         | Vai trò                               |
| ------------ | ------------------------------------- |
| `users.xlsx` | Chứa thông tin tài khoản người dùng   |
| `logs.xlsx`  | Ghi lại lịch sử giao dịch điểm thưởng |

## 🔐 Bảo mật & OTP

- ✅ Sử dụng SHA256 để băm mật khẩu
- ✅ OTP sinh ngẫu nhiên khi cập nhật thông tin / giao dịch
- ✅ Người dùng phải nhập đúng OTP mới được xác nhận

## 📚 Tài liệu tham khảo

- OpenXLSX: https://github.com/troldal/OpenXLSX
- SHA256 C++: https://github.com/B-Con/crypto-algorithms

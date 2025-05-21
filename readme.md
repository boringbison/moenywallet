# 📘 Hệ thống quản lý ví điểm thưởng

## 🧾 Giới thiệu dự án

Đây là hệ thống dòng lệnh viết bằng **C++**, cho phép người dùng đăng ký, đăng nhập, quản lý thông tin tài khoản và thực hiện các giao dịch điểm thưởng giữa các ví. Dữ liệu được lưu trữ bằng **file Excel (.xlsx)** thông qua thư viện **OpenXLSX**. Giao diện sử dụng dòng lệnh đơn giản, thân thiện, phù hợp với mô hình bài tập lớn học phần C++.

## 👨‍💻 Thành viên thực hiện

| Họ tên          | MSSV       | Vai trò/Phân công                                                                            |
| --------------- | ---------- | -------------------------------------------------------------------------------------------- |
| Nguyễn Huy Hoàn | K24DTCN369 | Trưởng nhóm / phân tích, thiết kế hệ thống, cập nhật OTP, fix bug                            |
| Lê Thanh Nguyên | K24DTCN379 | Thành viên / đăng nhập, đăng xuất, cập nhật thông tin cá nhân, phân luồng admin user, fixbug |
| Trần Anh Đức    | K24DTCN366 | Thành viên / đăng ký, xóa tài khoản                                                          |
| Hà Ngọc Quang   | K24DTCN382 | Thành viên / quản lý ví, giao dịch điểm, ghi nhận log giao dịch                              |

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

5.  Phân tích & Đặc tả chức năng
    a. Tạo tài khoản:
    Admin: Khi tạo mới, mật khẩu sẽ được sinh ngẫu nhiên (role: admin).

Client: Khi tạo mới, người dùng tự đặt mật khẩu (role: user).

Cập nhật thông tin tài khoản:

Admin: Có quyền cập nhật mọi tài khoản.

Client: Chỉ cập nhật được tài khoản của chính mình. Nếu tài khoản do admin tạo, bắt buộc đổi mật khẩu dựa vào trường isAdmin.

b. Xóa tài khoản:

Admin: Xóa được tất cả tài khoản trừ tài khoản admin.

Client: Chỉ xóa được tài khoản của chính mình.

c. Ví điện tử Nạp, chuyển, nhận điểm:
Admin: Khi client nạp điểm, số dư admin giảm, số dư client tăng (ghi trạng thái vào transaction_status).

Client: Khi nạp/chuyển điểm, số dư thay đổi tương ứng (ghi trạng thái vào transaction_status).

d. Theo dõi lịch sử giao dịch và số dư:

Khi chuyển tiền, lưu giá trị cũ (old_value), giá trị mới (new_value), trạng thái (action: chuyển thành công/thất bại), id người gửi và nhận vào file Excel.

Khi nhận tiền, lưu tương tự, action là "Nhận tiền".

🧩 Cấu trúc thư mục
moenywallet/
├── CMakeLists.txt # File cấu hình CMake để build dự án
├── README.md # Tài liệu mô tả dự án (đã tạo)
├── build/ # Thư mục output sau khi build (sinh tự động)
├── data/ # Dữ liệu chính của hệ thống
│ ├── logs.xlsx # Lưu lịch sử giao dịch điểm
│ └── users.xlsx # Lưu danh sách tài khoản người dùng
├── src/ # Mã nguồn chính
│ ├── OTPManager.h
│ ├── add_user.h
│ ├── admin.h
│ ├── delete_user.h
│ ├── get_user_info.h
│ ├── hash_utils.h
│ ├── login.h
│ ├── menu.h
│ ├── transfer.h
│ ├── update_user.h
│ ├── user.h
│ ├── user_column_index.h
│ ├── utils.h
│ ├── validate.h
│ ├── add_user.cpp
│ ├── admin.cpp
│ ├── delete_user.cpp
│ ├── get_user_info.cpp
│ ├── hash_utils.cpp
│ ├── login.cpp
│ ├── main.cpp
│ ├── menu.cpp
│ ├── transfer.cpp
│ ├── update_user.cpp
│ ├── user.cpp
│ ├── utils.cpp
│ └── validate.cpp
├── test/ # (Chưa có nội dung - để dành kiểm thử sau)

🧱 Cách build & chạy

⚙️ Yêu cầu

- C++17 trở lên
- VS Code, CMake, OpenXLSX đã cấu hình
- MinGW hoặc compiler tương thích

▶️ Build & chạy (trên terminal)

```bash
cmake -S . -B build
cmake --build .
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

# 💰 MoenyWallet

MoenyWallet là một ứng dụng quản lý ví cá nhân được viết bằng ngôn ngữ C++. Dự án sử dụng thư viện [OpenXLSX](https://github.com/troldal/OpenXLSX) để lưu trữ và thao tác dữ liệu người dùng trong các file Excel.

## 🧩 Tính năng chính

- 📥 Thêm người dùng mới
- 🔄 Cập nhật thông tin người dùng
- ❌ Xóa người dùng (phân quyền: không cho phép user thường xóa admin)
- 🔐 Đăng nhập và kiểm tra thông tin đăng nhập
- 💸 Chuyển tiền giữa các ví
- 🧾 Lưu trữ dữ liệu người dùng và giao dịch vào file Excel (`data/User.xlsx`)

## 🗂 Cấu trúc thư mục

src/ 
├── add_user.cpp / .h # Thêm người dùng 
├── delete_user.cpp / .h # Xóa người dùng 
├── get_user_info.cpp / .h # Lấy thông tin người dùng 
├── login.cpp / .h # Đăng nhập 
├── transfer.cpp / .h # Chuyển tiền 
├── update_user.cpp / .h # Cập nhật người dùng 
├── validate.cpp / .h # Kiểm tra hợp lệ 
├── main.cpp # Chương trình chính 
├── menu.cpp / .h # Menu CLI 
└── user.h # Struct định nghĩa User


## ⚙️ Cài đặt

### 1. Yêu cầu

- Trình biên dịch C++ (GCC / MSVC / Clang)
- CMake
- [OpenXLSX](https://github.com/troldal/OpenXLSX)

### 2. Build dự án

```bash
git clone https://github.com/boringbison/moenywallet.git
cd moenywallet
mkdir build && cd build
cmake ..
make

3. Chạy chương trình

./main
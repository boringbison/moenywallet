#ifndef DELETE_USER_H
#define DELETE_USER_H

#include <string>

// Deletes a user with the given username from the Excel file at the specified path.
// If the user is not an admin or if the caller is not an admin, restrictions apply.
void deleteUser(const std::string& filePath, const std::string& targetUser, bool isAdmin);

#endif 
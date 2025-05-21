#include "hash_utils.h"
#include "picosha2.h"

std::string hashPassword(const std::string &password)
{
    return picosha2::hash256_hex_string(password);
}

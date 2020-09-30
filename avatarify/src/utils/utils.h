#ifndef UTILS_H
#define UTILS_H

#include <filesystem>

class Utils {
public:
    Utils() = delete;
    static size_t fileSize(const std::filesystem::path& path_);
    static size_t modPow(size_t base, size_t exp, size_t modulus);
};

#endif // UTILS_H

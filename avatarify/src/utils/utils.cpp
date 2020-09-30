#include "utils.h"

size_t Utils::fileSize(const std::filesystem::path &path_) {
#if defined __MINGW64__ || defined _MSC_VER
    struct ::_stat64 st{};
    ::_wstat64(path_.wstring().c_str(), &st);
    return static_cast<unsigned long long>(st.st_size);
#else
    std::ifstream in(path_, std::ios::binary);
    const auto begin = in.tellg();
    in.seekg(0, std::ios::end);
    const auto end = in.tellg();
    return static_cast<unsigned long long>(end - begin);
#endif
}

size_t Utils::modPow(size_t base, size_t exp, size_t modulus) {
    base %= modulus;
    size_t result = 1;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % modulus;
        base = (base * base) % modulus;
        exp >>= 1;
    }
    return result;
}

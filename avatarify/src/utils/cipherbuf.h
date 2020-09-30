#ifndef CIPHERBUF_H
#define CIPHERBUF_H

#include <filesystem>
#include <fstream>

class CipherBuf : public std::streambuf {
public:
    CipherBuf() = delete;
    CipherBuf(const CipherBuf&) = delete;
    CipherBuf& operator=(const CipherBuf&) = delete;
    explicit CipherBuf(const std::filesystem::path& path_);
    static void apply(const std::filesystem::path& src_,
                      const std::filesystem::path& dest_);
    void ignore(size_t n_);
private:
    static size_t constexpr CHUNK = 1024 * 1024;
    static size_t constexpr A = 29;
    static size_t constexpr A_MINUS_ONE_INV = 220458552;
    static size_t constexpr C = 13;
    static size_t constexpr X = 617283967;
    static size_t constexpr M = 1234567891;

    size_t m_left{0};
    std::ifstream m_fileStream;
    std::vector<char> m_buffer;
    unsigned long long m_first{X};
    char getChar();
    static inline size_t getX(size_t n_);

    int underflow() override;
};

#endif // CIPHERBUF_H

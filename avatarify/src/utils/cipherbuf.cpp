#include "cipherbuf.h"

#include "utils.h"

CipherBuf::CipherBuf(const std::filesystem::path &path_) :
    m_left{Utils::fileSize(path_)}, m_fileStream(path_, std::ios_base::binary) { }

void CipherBuf::apply(const std::filesystem::path &src_, const std::filesystem::path &dest_) {
    CipherBuf cipherBuf(src_);
    std::istream in(&cipherBuf);
    std::ofstream out(dest_, std::ios_base::binary);
    std::vector<char> buffer(CHUNK);
    do {
        in.read(buffer.data(), buffer.size());
        out.write(buffer.data(), in.gcount());
    } while (in.gcount() > 0);
}

void CipherBuf::ignore(size_t n_) {
    if (n_ == 0) {
        return;
    }
    m_first = getX(n_);
    m_fileStream.seekg(n_);
    m_left -= n_;
}

char CipherBuf::getChar() {
    m_first = (m_first * A + C) % M;
    return static_cast<char>(m_first % std::numeric_limits<unsigned char>::max());
}

size_t CipherBuf::getX(size_t n_) {
    auto an = Utils::modPow(A, n_, M);
    auto c = (an - 1) * A_MINUS_ONE_INV % M; // / (A - 1);
    return ((an * X + c * C) % M);
}

int CipherBuf::underflow() {
    if (this->gptr() == this->egptr()) {
        std::vector<char> readBuffer(std::min(CHUNK, m_left));
        if (!readBuffer.empty()) {
            m_fileStream.read(readBuffer.data(), static_cast<long long>(readBuffer.size()));
            m_left -= readBuffer.size();
            m_buffer.resize(readBuffer.size());
            for (size_t i = 0; i < readBuffer.size(); ++i) {
                m_buffer[i] = readBuffer[i] ^ getChar();
            }
            this->setg(m_buffer.data(), m_buffer.data(), m_buffer.data() + m_buffer.size());
        }
    }
    return this->gptr() == this->egptr()
            ? std::char_traits<char>::eof()
            : std::char_traits<char>::to_int_type(*this->gptr());
}

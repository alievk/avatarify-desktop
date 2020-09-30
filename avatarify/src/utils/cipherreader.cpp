#include "cipherreader.h"

#include "../utils/utils.h"
#include "cipherbuf.h"

CipherReader::CipherReader(const std::filesystem::path& path_) :
    m_path{path_} { }

size_t CipherReader::size() const {
    return Utils::fileSize(m_path);
}

size_t CipherReader::read(uint64_t pos, void* buf, size_t n,
                          const char* /*what*/) const {
    CipherBuf cipherBuf(m_path);
    cipherBuf.ignore(pos);
    std::istream in(&cipherBuf);
    in.read(reinterpret_cast<char*>(buf), n);
    return in.gcount();
}

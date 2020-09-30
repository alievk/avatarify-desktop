#ifndef CIPHERREADER_H
#define CIPHERREADER_H

#include <filesystem>
#include "caffe2/serialize/read_adapter_interface.h"

class CipherReader : public caffe2::serialize::ReadAdapterInterface {
public:
    CipherReader(const std::filesystem::path& path_);
    size_t size() const override;
    size_t read(uint64_t pos, void* buf, size_t n, const char* what = "") const override;
private:
    std::filesystem::path m_path;
};

#endif // CIPHERREADER_H

/**
 * @file   XFileCrypt.h
 * @brief  
 *
 * @details   
 *
 * @author 31667
 * @date   2025-10-17
 */

#ifndef XFILECRYPT_H
#define XFILECRYPT_H

#include <memory>
#include <string>

class XFileCrypt
{
public:
    XFileCrypt();
    virtual ~XFileCrypt();
    using Ptr = std::shared_ptr<XFileCrypt>;
    static auto create() -> XFileCrypt::Ptr;

public:
    auto start(const std::string& in_path, const std::string& out_path, const std::string& pwd,
               bool is_encrypt = true) const -> void;

    auto wait() const -> void;

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;
};


#endif // XFILECRYPT_H

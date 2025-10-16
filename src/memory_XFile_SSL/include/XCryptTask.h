/**
 * @file   XCrypt.h
 * @brief  
 *
 * @details   
 *
 * @author 31667
 * @date   2025-10-16
 */

#ifndef XCRYPT_H
#define XCRYPT_H

#include "XIOStream.h"
#include <memory>
#include <string>

class XCryptTask : public XIOStream
{
public:
    XCryptTask();
    ~XCryptTask() override;
    using Ptr = std::shared_ptr<XCryptTask>;
    static auto create() -> XCryptTask::Ptr;

public:
    /// \brief
    /// \param pwd   密钥
    /// \param is_en 加密/解密
    auto init(const std::string &pwd, bool is_en = true) -> void;

    auto run() -> void override;

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;
};


#endif // XCRYPT_H

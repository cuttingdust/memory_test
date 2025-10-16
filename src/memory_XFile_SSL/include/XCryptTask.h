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

class XCryptTask : public XIOStream
{
public:
    XCryptTask();
    ~XCryptTask() override;
    using Ptr = std::shared_ptr<XCryptTask>;
    static auto create() -> XCryptTask::Ptr;

public:
    auto run() -> void override;
};


#endif // XCRYPT_H

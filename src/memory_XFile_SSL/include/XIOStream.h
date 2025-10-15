/**
 * @file   XIOStream.h
 * @brief  
 *
 * @details   
 *
 * @author 31667
 * @date   2025-10-15
 */

#ifndef XIOSTREAM_H
#define XIOSTREAM_H

#include <memory>

class XIOStream
{
public:
    XIOStream();
    virtual ~XIOStream();

public:
    auto start() -> void;

    auto wait() -> void;

    auto stop() -> void;

    auto isRunning() -> bool;

    virtual auto run() -> void = 0;

protected:
    auto dataByte() const -> long long;

    auto setDataByte(long long data_byte) -> void;

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;
};


#endif // XIOSTREAM_H

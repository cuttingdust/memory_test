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

#include "XData.h"
#include <memory>

class XIOStream
{
public:
    XIOStream();
    virtual ~XIOStream();
    using Ptr = std::shared_ptr<XIOStream>;
    /// 纯虚类 无法create 需要继承

public:
    auto start() -> void;

    auto wait() -> void;

    auto stop() -> void;

    auto isRunning() -> bool;

    virtual auto run() -> void = 0;

public:
    /// \brief 设置数据生产者
    /// \param p
    auto setMemPool(const SmartMemPool &p) -> void;

    auto memPool() const -> SmartMemPool;

    /// \brief 设置责任链下一个节点
    /// \param next
    auto setNext(const XIOStream::Ptr &next) -> void;

    auto next() const -> XIOStream::Ptr;

    /// \brief 给对象传递数据，线程安全
    /// \param data
    auto pushBack(const XData::Ptr &data) -> void;

    auto popFront() -> XData::Ptr;

protected:
    auto dataByte() const -> long long;

    auto setDataByte(long long data_byte) -> void;

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;
};


#endif // XIOSTREAM_H

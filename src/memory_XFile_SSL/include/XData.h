/**
 * @file   XData.h
 * @brief  
 *
 * @details   
 *
 * @author 31667
 * @date   2025-10-15
 */

#ifndef XDATA_H
#define XDATA_H

#include <memory>

namespace std::pmr
{
    class memory_resource;
}

using SmartMemPool = std::shared_ptr<std::pmr::memory_resource>;

class XData
{
public:
    virtual ~XData();
    using Ptr = std::shared_ptr<XData>;
    static auto make(const SmartMemPool &pool) -> XData::Ptr;

protected:
    XData();
    XData(const SmartMemPool &pool);


public:
    /// \brief
    /// \param mem_size  占用内存字节数
    /// \return
    auto allocate(const long long mem_size) -> void *;

    auto setSize(const long long s) -> void;

    auto size() const -> long long;

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;
};


#endif // XDATA_H

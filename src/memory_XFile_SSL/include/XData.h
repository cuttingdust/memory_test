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


class XData
{
public:
    using Ptr          = std::shared_ptr<XData>;
    using SmartMemPool = std::shared_ptr<std::pmr::memory_resource>;

public:
    static auto create(const SmartMemPool &pool) -> XData::Ptr;

    /// \brief
    /// \param mem_size  占用内存字节数
    /// \return
    auto allocate(long long mem_size) -> void *;

private:
    XData();
    XData(const SmartMemPool &pool);
    virtual ~XData();

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;
};


#endif // XDATA_H

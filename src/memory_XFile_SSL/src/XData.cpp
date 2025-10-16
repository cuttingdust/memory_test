#include "XData.h"

#include <iostream>

class XData::PImpl
{
public:
    PImpl(XData *owenr);
    PImpl(XData *owenr, SmartMemPool pool);
    ~PImpl() = default;

public:
    XData       *owenr_ = nullptr;
    SmartMemPool mem_pool_;
    long long    mem_size_ = 0; ///< 占用空间字节数
    long long    size_     = 0; ///< 数据字节数
    void        *data_     = nullptr;
    bool         end_      = false;
};

XData::PImpl::PImpl(XData *owenr) : owenr_(owenr)
{
}

XData::PImpl::PImpl(XData *owenr, SmartMemPool pool) : owenr_(owenr), mem_pool_(std::move(pool))
{
}

XData::XData()
{
    impl_ = std::make_unique<XData::PImpl>(this);
}

XData::XData(const SmartMemPool &pool)
{
    impl_ = std::make_unique<XData::PImpl>(this, pool);
}

XData::~XData()
{
    if (!impl_->data_ || !impl_->mem_pool_)
    {
        return;
    }

    impl_->mem_pool_->deallocate(impl_->data_, impl_->mem_size_);
    impl_->data_     = nullptr;
    impl_->mem_size_ = 0;
    impl_->size_     = 0;

    std::cout << "-" << std::flush;
}

auto XData::make(const SmartMemPool &pool) -> XData::Ptr
{
    return std::shared_ptr<XData>(new XData(pool));
}

auto XData::allocate(const long long mem_size) -> void *
{
    if (mem_size <= 0)
    {
        std::cerr << "XData::allocate failed! mem_size<=0" << std::endl;
        return nullptr;
    }
    if (!impl_->mem_pool_)
    {
        std::cerr << "XData::allocate failed! mem_pool_ is null!" << std::endl;
        return nullptr;
    }

    impl_->data_     = impl_->mem_pool_->allocate(mem_size);
    impl_->mem_size_ = mem_size;
    impl_->size_     = mem_size;
    std::cout << "+" << std::flush;
    return impl_->data_;
}

auto XData::setSize(const long long s) -> void
{
    impl_->size_ = s;
}

auto XData::data() const -> void *
{
    return impl_->data_;
}

auto XData::size() const -> long long
{
    return impl_->size_;
}

auto XData::setEnd(bool is_end) -> void
{
    impl_->end_ = is_end;
}

auto XData::isEnd() const -> bool
{
    return impl_->end_;
}

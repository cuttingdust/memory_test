#include "XIOStream.h"

#include <thread>
#include <iostream>
#include <list>
#include <mutex>

class XIOStream::PImpl
{
public:
    PImpl(XIOStream *owenr);
    ~PImpl() = default;

public:
    auto exec() -> void;

public:
    XIOStream  *owenr_      = nullptr;
    bool        is_running_ = false;
    std::thread th_;
    std::mutex  mtx_;

    SmartMemPool          mem_pool_;
    XIOStream::Ptr        next_;
    std::list<XData::Ptr> datas_;

    long long data_byte_ = 0; /// 所有要处理数据的字节数
};

XIOStream::PImpl::PImpl(XIOStream *owenr) : owenr_(owenr)
{
}

auto XIOStream::PImpl::exec() -> void
{
    owenr_->run();
    is_running_ = false;
}

XIOStream::XIOStream()
{
    impl_ = std::make_unique<XIOStream::PImpl>(this);
    std::cout << "XIOStream Create." << std::endl;
}

XIOStream::~XIOStream()
{
    std::cout << "XIOStream Destroy!" << std::endl;
    XIOStream::wait();
}

auto XIOStream::start() -> void
{
    impl_->is_running_ = true;
    impl_->th_         = std::thread(&XIOStream::PImpl::exec, impl_.get());
}

auto XIOStream::wait() -> void
{
    if (impl_->th_.joinable())
        impl_->th_.join();
}

auto XIOStream::stop() -> void
{
    impl_->is_running_ = false;
    wait();
}

auto XIOStream::isRunning() -> bool
{
    return impl_->is_running_;
}

auto XIOStream::setMemPool(const SmartMemPool &p) -> void
{
    impl_->mem_pool_ = p;
}

auto XIOStream::memPool() const -> SmartMemPool
{
    return impl_->mem_pool_;
}

auto XIOStream::setNext(const XIOStream::Ptr &next) -> void
{
    impl_->next_ = next;
}

auto XIOStream::next() const -> XIOStream::Ptr
{
    return impl_->next_;
}

auto XIOStream::pushBack(const XData::Ptr &data) -> void
{
    std::unique_lock<std::mutex> lock(impl_->mtx_);
    impl_->datas_.push_back(data);
    /// TODO 考虑最大列表 生产者达到一定数量 让消费者消费一些 再生产 以避免达到内存瓶颈
}

auto XIOStream::popFront() -> XData::Ptr
{
    std::unique_lock<std::mutex> lock(impl_->mtx_);
    if (impl_->datas_.empty())
    {
        return nullptr;
    }

    auto re = impl_->datas_.front();
    impl_->datas_.pop_front();
    return re;
}

auto XIOStream::dataByte() const -> long long
{
    return impl_->data_byte_;
}

auto XIOStream::setDataByte(long long data_byte) -> void
{
    impl_->data_byte_ = data_byte;
}

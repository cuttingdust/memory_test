#include "XIOStream.h"

#include <thread>
#include <iostream>

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
    long long   data_byte_ = 0; /// 所有要处理数据的字节数
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

auto XIOStream::dataByte() const -> long long
{
    return impl_->data_byte_;
}

auto XIOStream::setDataByte(long long data_byte) -> void
{
    impl_->data_byte_ = data_byte;
}

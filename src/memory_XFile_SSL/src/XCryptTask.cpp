#include "XCryptTask.h"

#include "XSec.h"

#include <iostream>
#include <thread>

class XCryptTask::PImpl
{
public:
    PImpl(XCryptTask *owenr);
    ~PImpl() = default;

public:
    XCryptTask *owenr_ = nullptr;
    XSec::Ptr   enc_   = nullptr;
};

XCryptTask::PImpl::PImpl(XCryptTask *owenr) : owenr_(owenr)
{
}

XCryptTask::XCryptTask()
{
    impl_ = std::make_unique<XCryptTask::PImpl>(this);
}

XCryptTask::~XCryptTask() = default;

auto XCryptTask::create() -> XCryptTask::Ptr
{
    return std::make_shared<XCryptTask>();
}

auto XCryptTask::init(const std::string &pwd, bool is_en) -> void
{
    impl_->enc_ = XSec::create();
    impl_->enc_->init(XSec::XDES_ECB, pwd, is_en); /// 加密
}

auto XCryptTask::run() -> void
{
    std::cout << "========================" << std::endl;
    std::cout << "begin XCryptTask::Main()" << std::endl;
    while (isRunning())
    {
        const auto data = popFront();
        if (!data)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            continue;
        }

        int  outsize = data->size() + impl_->enc_->getPadding(data->size());
        auto out     = XData::make(memPool());
        out->allocate(outsize);

        int data_size = impl_->enc_->encrypt(static_cast<const unsigned char *>(data->data()), data->size(),
                                             static_cast<unsigned char *>(out->data()), data->isEnd());
        out->setSize(data_size);
        out->setEnd(data->isEnd());

        if (next())
        {
            next()->pushBack(out);
        }
        if (data->isEnd())
        {
            break;
        }

        std::cout << "<" << data->size() << ">" << std::flush;
    }
    std::cout << std::endl;
    std::cout << "end XCryptTask::Main()" << std::endl;
    std::cout << "========================" << std::endl;
}

#include "XFileCrypt.h"

#include "XReadTask.h"
#include "XCryptTask.h"
#include "XWriteTask.h"

#include <memory_resource>

class XFileCrypt::PImpl
{
public:
    PImpl(XFileCrypt *owenr);
    ~PImpl() = default;

public:
    auto init() -> void;

public:
    XFileCrypt     *owenr_ = nullptr;
    XReadTask::Ptr  rt_    = nullptr;
    XCryptTask::Ptr ct_    = nullptr;
    XWriteTask::Ptr wt_    = nullptr;
    SmartMemPool    creator_ = nullptr;
};

XFileCrypt::PImpl::PImpl(XFileCrypt *owenr) : owenr_(owenr)
{
    init();
}

auto XFileCrypt::PImpl::init() -> void
{
    creator_ = std::make_shared<std::pmr::synchronized_pool_resource>();

    rt_ = XReadTask::create();
    ct_ = XCryptTask::create();
    wt_ = XWriteTask::create();

    rt_->setMemPool(creator_);
    ct_->setMemPool(creator_);
    wt_->setMemPool(creator_);

    rt_->setNext(ct_);
    ct_->setNext(wt_);
}

XFileCrypt::XFileCrypt()
{
    impl_ = std::make_unique<XFileCrypt::PImpl>(this);
}

XFileCrypt::~XFileCrypt() = default;

auto XFileCrypt::create() -> XFileCrypt::Ptr
{
    return std::make_shared<XFileCrypt>();
}

auto XFileCrypt::start(const std::string &in_path, const std::string &out_path, const std::string &pwd,
                       bool is_encrypt) const -> void
{

    impl_->rt_->init(in_path);
    impl_->ct_->init(pwd, is_encrypt);
    impl_->wt_->init(out_path);

    impl_->rt_->start();
    impl_->ct_->start();
    impl_->wt_->start();
}

auto XFileCrypt::wait() const -> void
{
    impl_->rt_->wait();
    impl_->ct_->wait();
    impl_->wt_->wait();
}

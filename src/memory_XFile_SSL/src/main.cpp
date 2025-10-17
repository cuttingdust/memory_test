#include "XReadTask.h"
#include "XCryptTask.h"
#include "XWriteTask.h"

#include <iostream>
#include <memory_resource>


int main(int argc, char *argv[])
{
    std::cout << "目录下文件批量加密" << std::endl;
    const auto pwd = "12345678";
    {
        /// 创建一个内存池
        auto creator = std::make_shared<std::pmr::synchronized_pool_resource>();

        const auto rt = XReadTask::create();
        rt->init("assert/lena_hed.jpg");
        rt->setMemPool(creator);

        const auto ct = XCryptTask::create();
        ct->init(pwd);
        ct->setMemPool(creator);
        rt->setNext(ct);
        
        const auto wt = XWriteTask::create();
        wt->init("assert/lena_hed_out1.jpg");
        wt->setMemPool(creator);
        ct->setNext(wt);
        
        rt->start();
        ct->start();
        wt->start();
        
        rt->wait();
        ct->wait();
        wt->wait();
    }

    std::cout << "###########################################################" << std::endl;

    /// 开始解密
    {
        /// 创建一个内存池
        auto creator = std::make_shared<std::pmr::synchronized_pool_resource>();

        const auto rt = XReadTask::create();
        rt->init("assert/lena_hed_out1.jpg");
        rt->setMemPool(creator);

        const auto ct = XCryptTask::create();
        ct->init(pwd, false);
        ct->setMemPool(creator);
        rt->setNext(ct);
        
        const auto wt = XWriteTask::create();
        wt->init("assert/lena_hed_out2.jpg");
        wt->setMemPool(creator);
        ct->setNext(wt);
        
        rt->start();
        ct->start();
        wt->start();
        
        rt->wait();
        ct->wait();
        wt->wait();
    }

    getchar();
    return 0;
}

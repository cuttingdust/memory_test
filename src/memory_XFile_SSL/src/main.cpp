#include "XReadTask.h"
#include "XCryptTask.h"
#include "XWriteTask.h"

#include <iostream>
#include <memory_resource>


int main(int argc, char *argv[])
{
    {
        std::cout << "目录下文件批量加密" << std::endl;
        /// 创建一个内存池
        std::shared_ptr<std::pmr::memory_resource> creator = std::make_shared<std::pmr::synchronized_pool_resource>();

        const auto rt = XReadTask::create();
        rt->init("assert/lena_hed.jpg");
        rt->setMemPool(creator);

        const auto ct = XCryptTask::create();
        ct->init("12345678");
        ct->setMemPool(creator);
        rt->setNext(ct);

        const auto wt = XWriteTask::create();
        wt->init("assert/lena_hed_out.jpg");
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

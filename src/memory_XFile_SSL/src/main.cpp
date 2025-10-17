#include "XReadTask.h"
#include "XCryptTask.h"
#include "XFileCrypt.h"
#include "XWriteTask.h"


#include <iostream>
#include <memory_resource>


int main(int argc, char *argv[])
{
    std::cout << "目录下文件批量加密" << std::endl;
    constexpr auto pwd      = "12345678";
    constexpr auto in_path  = "assert/lena_hed.jpg";
    constexpr auto cry_path = "assert/lena_hed_cry.jpg";
    constexpr auto out_path = "assert/lena_hed_out.jpg";

    {
        auto xfc = XFileCrypt::create();
        xfc->start(in_path, cry_path, pwd, true);
        xfc->wait();
    }

    std::cout << "###########################################################" << std::endl;

    {
        auto xfc = XFileCrypt::create();
        xfc->start(cry_path, out_path, pwd, false);
        xfc->wait();
    }

    getchar();
    return 0;
}

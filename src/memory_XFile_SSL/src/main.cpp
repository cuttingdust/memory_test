#include "xsec.h"

#include <iostream>


int main(int argc, char *argv[])
{
    {
        std::cout << "目录下文件批量加密" << std::endl;
        XSec crypt;
        crypt.init(XSec::XDES_ECB, "12345678", true);
        unsigned char out[1024] = { 0 };
        int           en_size   = crypt.encrypt((unsigned char *)"abcdefg", 7, out, true);
        std::cout << "en_size = " << en_size << std::endl;
        std::cout << out << std::endl;

        return 0;
    }
    getchar();
    return 0;
}

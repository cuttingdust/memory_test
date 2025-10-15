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

        std::cout << "=============================================" << std::endl;

        crypt.init(XSec::XDES_ECB, "12345678", false);
        unsigned char de_out[1024] = { 0 };
        int           de_size      = crypt.encrypt(out, en_size, de_out, true);
        std::cout << "de_size = " << de_size << std::endl;
        std::cout << de_out << std::endl;
    }
    getchar();
    return 0;
}

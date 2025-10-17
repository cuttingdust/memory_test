
#include "XFileCrypt.h"

#include <argparse/argparse.hpp>

#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    /// 创建 ArgumentParser 对象
    argparse::ArgumentParser program("FileCrypt");

    /// 添加参数
    program.add_argument("-i", "--input").help("input file or directory path").required();
    program.add_argument("-o", "--output").help("output encrypted file or encrypted directory path").required();
    program.add_argument("-p", "--password").help("password for encryption/decryption").required();
    program.add_argument("-de", "--decrypt")
            .help("encrypt flag (use this flag for decryption, omit for encryption)")
            .default_value(true)
            .implicit_value(false);

    try
    {
        /// 解析命令行参数
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "错误: " << e.what() << std::endl;
        std::cerr << "使用 -h 或 --help 查看帮助信息。" << std::endl;
        std::cerr << std::endl;
        std::cerr << program; // 输出帮助信息
        return 1;             // 返回错误代码
    }

    /// 获取参数值
    const auto& in  = program.get<std::string>("--input");
    const auto& out = program.get<std::string>("--output");
    const auto& pwd = program.get<std::string>("--password");
    const auto& en  = program.get<bool>("--decrypt");

    auto in_path  = fs::path(in);
    auto out_path = fs::path(out);

    /// 检查输入路径是文件还是目录
    if (fs::exists(in_path))
    {
        if (fs::is_directory(in_path))
        {
            std::cout << "输入是一个目录: " << in_path << std::endl;

            /// 处理目录中的每个文件
            for (const auto& entry : fs::directory_iterator(in_path))
            {
                if (fs::is_regular_file(entry.path()))
                {
                    const auto& file_path = entry.path();

                    auto file_name   = file_path.stem();      /// 不包括扩展名
                    auto file_suffix = file_path.extension(); /// 包括扩展名

                    /// 构建输出文件名，添加 "_out" 后缀
                    auto out_file_name = file_name.string() + "_out" + file_suffix.string();
                    auto out_file_path = out_path / out_file_name;


                    /// 加密文件
                    auto xfc = XFileCrypt::create();
                    xfc->start(file_path.string(), out_file_path.string(), pwd, en);
                    xfc->wait();


                    if (en)
                    {
                        std::cout << "加密文件:" << file_path << " -> " << out_file_path << std::endl;
                    }
                    else
                    {
                        std::cout << "解密文件:" << file_path << " -> " << out_file_path << std::endl;
                    }
                }
            }
        }
        else if (fs::is_regular_file(in_path))
        {
            // std::cout << "输入是一个文件: " << in_path << std::endl;
            /// 解密文件
            auto xfc = XFileCrypt::create();
            xfc->start(in_path.string(), out_path.string(), pwd, en);
            xfc->wait();

            if (en)
            {
                std::cout << "加密文件:" << in_path << " -> " << out_path << std::endl;
            }
            else
            {
                std::cout << "解密文件:" << in_path << " -> " << out_path << std::endl;
            }

            return 1;
        }
        else
        {
            std::cerr << "输入路径既不是文件也不是目录: " << in_path << std::endl;
            return 0;
        }
    }
    else
    {
        std::cerr << "输入路径不存在: " << in_path << std::endl;
        return 0;
    }
}

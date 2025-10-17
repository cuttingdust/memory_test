/**
 * @file   XWriteTask.h
 * @brief  
 *
 * @details   
 *
 * @author 31667
 * @date   2025-10-17
 */

#ifndef XWRITETASK_H
#define XWRITETASK_H

#include "XIOStream.h"

#include <string>

class XWriteTask : public XIOStream
{
public:
    XWriteTask();
    ~XWriteTask() override;
    using Ptr = std::shared_ptr<XWriteTask>;
    static auto create() -> XWriteTask::Ptr;

public:
    auto init(const std::string& filepath) -> bool;

    auto run() -> void override;

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;
};


#endif // XWRITETASK_H

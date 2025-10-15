/**
 * @file   XReadTask.h
 * @brief  
 *
 * @details   
 *
 * @author 31667
 * @date   2025-10-15
 */

#ifndef XREADTASK_H
#define XREADTASK_H

#include "XIOStream.h"

#include <string>

class XReadTask : public XIOStream
{
public:
    XReadTask();
    ~XReadTask() override;

public:
    auto init(const std::string& filename) -> bool;

private:
    auto run() -> void override;

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;
};


#endif // XREADTASK_H

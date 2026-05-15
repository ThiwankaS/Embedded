#pragma once

#include "SystemTask.hpp"
#include <cstddef>

class MonitorTask : public SystemTask {
public:
    using SystemTask::SystemTask;

protected:
    void run() override;
};

#pragma once

#include "freertos/idf_additions.h"
#include "freertos/task.h"
#include "portmacro.h"
#include <cstdint>
#include <string>

class SystemTask {
public:
    SystemTask(std::string name, uint32_t stackSize, UBaseType_t priority);
    virtual ~SystemTask() = default;

    void start(BaseType_t coreId = tskNO_AFFINITY);

protected:
    virtual void run() = 0;

    TaskHandle_t m_handle = nullptr;
    std::string m_name;
    uint32_t m_stackSize;
    UBaseType_t m_priority;

private:
    static void TaskEntry(void* pvParameters);
};

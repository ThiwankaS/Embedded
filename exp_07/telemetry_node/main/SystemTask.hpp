#pragma once
#include "freertos/idf_additions.h"
#include "freertos/task.h"
#include "portmacro.h"
#include <cstdint>
#include <string>

class SystemTask {
    public:
        SystemTask(std::string name, uint32_t stackSize, UBaseType_t priority) 
            : m_name(name), m_stackSize(stackSize), m_priority(priority){}

        virtual ~SystemTask() = default;

        void start(BaseType_t coreId = tskNO_AFFINITY) {
            xTaskCreatePinnedToCore(
                    TaskEntry,
                    m_name.c_str(),
                    m_stackSize,
                    this,
                    m_priority,
                    &m_handle,
                    coreId);
        } 
 
    protected:
        virtual void run () = 0;

        TaskHandle_t    m_handle = nullptr;
        std::string     m_name;
        uint32_t        m_stackSize;
        UBaseType_t     m_priority;

    private:
        static void TaskEntry(void* pvParameters) {
            auto* instance = static_cast<SystemTask*>(pvParameters);
            instance->run();
            vTaskDelete(nullptr);
        }
};

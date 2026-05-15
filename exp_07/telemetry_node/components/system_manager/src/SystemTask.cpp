#include "SystemTask.hpp"

SystemTask::SystemTask(std::string name, uint32_t stackSize, UBaseType_t priority)
    : m_name(std::move(name)), m_stackSize(stackSize), m_priority(priority) {}

void SystemTask::start(BaseType_t coreId) {
    xTaskCreatePinnedToCore(
        TaskEntry,
        m_name.c_str(),
        m_stackSize,
        this,
        m_priority,
        &m_handle,
        coreId);
}

void SystemTask::TaskEntry(void* pvParameters) {
    auto* instance = static_cast<SystemTask*>(pvParameters);
    instance->run();
    vTaskDelete(nullptr);
}

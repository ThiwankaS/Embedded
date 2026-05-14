#pragma once
#include "SystemTask.hpp"
#include "freertos/idf_additions.h"
#include "freertos/portable.h"
#include "portmacro.h"
#include <cstddef>
#include <iostream>

class MonitorTask : public SystemTask {
    public:
        using SystemTask::SystemTask;

    protected:
        void run () override {
            while (true) {
                size_t freeHeap = xPortGetFreeHeapSize();
                std::cout << "[" 
                    << m_name << "] Core : " 
                    << xPortGetCoreID() 
                    << " | Free Heap : " 
                    << freeHeap 
                    << "bytes \n";

                vTaskDelay(5000);
            }
        }
};

#include "MonitorTask.hpp"

#include "freertos/idf_additions.h"
#include "freertos/portable.h"
#include "portmacro.h"
#include <iostream>

void MonitorTask::run() {
    while (true) {
        size_t freeHeap = xPortGetFreeHeapSize();
        std::cout << "[" << m_name << "] Core : " << xPortGetCoreID() << " | Free Heap : " << freeHeap
                  << "bytes \n";

        vTaskDelay(5000);
    }
}

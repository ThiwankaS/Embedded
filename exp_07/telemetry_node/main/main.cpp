#include "MonitorTask.hpp"
#include "freertos/idf_additions.h"
#include "portmacro.h"
#include <iostream>

extern "C" void app_main(void) {
    std::cout << "--- Telemetry Node --- \n";
    auto* monitor = new MonitorTask("Sys_monitor", 4096, 1);
    monitor->start(0);

    while (true) {
        vTaskDelay(portMAX_DELAY);
    }
}

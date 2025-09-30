#include "Arduino_FreeRTOS.h"

#include "model/hover/HoverRepo.h"

[[noreturn]] void vTaskHover(void *pvParameters) {
    auto* repo = static_cast<HoverRepo*>(pvParameters);

    for(;;)
    {
        taskENTER_CRITICAL();
        repo->update();
        taskEXIT_CRITICAL();
        delay(50);
    }
}
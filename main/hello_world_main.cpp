#include "esp_log.h"
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#include <chrono>
#include <cstdlib>
#include <thread>
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

extern "C" {
void app_main(void);
}
void threadOne(int time);
void threadTwo(int time);

void app_main() {
  ESP_LOGI("CPP", "Hello world!\n");

  std::srand(666);
  int time1 = std::rand() % (150 - 100) + 100;
  int time2 = std::rand() % (150 - 100) + 100;

  std::thread first(threadOne, time1);
  std::thread second(threadTwo, time2);

  /* Print chip information */
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);

  ESP_LOGI("CPP", "This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
           chip_info.cores, (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

  ESP_LOGI("CPP", "silicon revision %d, ", chip_info.revision);

  ESP_LOGI("CPP", "%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded"
                                                         : "external");

  for (int i = 10; i >= 0; i--) {
    ESP_LOGI("CPP", "Restarting in %d seconds...\n", i);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  ESP_LOGI("CPP", "Restarting now.\n");
  fflush(stdout);
  esp_restart();
}

void threadOne(int time) {
  while (1) {
    ESP_LOGI("T1", "Printando Thread 1 - SleepTime: %d", time);
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
  }
}

void threadTwo(int time) {
  while (1) {
    ESP_LOGI("T2", "Printando Thread 2 - SleepTime: %d", time);
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
  }
}
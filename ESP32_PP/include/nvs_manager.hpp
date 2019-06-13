#include "nvs_flash.h"
#include "nvs.h"

void initNVS();
int32_t nvs_get(const char* dataName);
void nvs_set(const char* dataName, int data);
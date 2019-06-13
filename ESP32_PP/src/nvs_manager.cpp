#include "nvs_manager.hpp"

void initNVS()
{
     esp_err_t err =  nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
            // NVS partition was truncated and needs to be erased
            // Retry nvs_flash_init
            ESP_ERROR_CHECK(nvs_flash_erase());
            err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

}
int32_t nvs_get(const char* dataName)
{
    esp_err_t err;
    nvs_handle handle_NVS;

    err = nvs_open("storage", NVS_READWRITE, &handle_NVS);
      if (err != ESP_OK) 
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } 
    else 
    {
        printf("Done\n");

        // Read
        int32_t tempData = 0; // value will default to 0, if not set yet in NVS
        err = nvs_get_i32(handle_NVS, dataName, &tempData);
        switch (err) 
        {
            case ESP_OK:
                printf("Done\n");
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                tempData = 0xDEADBEEF;
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }



        // Close
        nvs_close(handle_NVS);
        return tempData;
}
nvs_close(handle_NVS);

}

void nvs_set(const char* dataName, int data)
{
    int32_t tempData = data;
    nvs_handle handle_NVS;
    esp_err_t err;
     err = nvs_open("storage", NVS_READWRITE, &handle_NVS);
      if (err != ESP_OK) 
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } 
    else 
    {
        err = nvs_set_i32(handle_NVS, dataName, tempData);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        printf("Committing updates in NVS ... ");
        err = nvs_commit(handle_NVS);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        nvs_close(handle_NVS);

    }

}

/* ESP HTTP Client Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include <stdio.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_websocket_client.h"
#include "esp_event.h"

#include "cJSON.h"

#define NO_DATA_TIMEOUT_SEC 10

#define CONFIG_WEBSOCKET_URI_FROM_STDIN 0

#define CONFIG_WEBSOCKET_URI "ws://192.168.15.11"

static const char *TAG = "WEBSOCKET";

static TimerHandle_t shutdown_signal_timer;
static SemaphoreHandle_t shutdown_sema;
static esp_websocket_client_handle_t client;
extern const uint8_t start_ecg[] asm("_binary_dump_txt_start");
extern const uint8_t end_ecg[] asm("_binary_dump_txt_end");

static uint8_t my_key[32] = {0x39,0x79,0x24,0x42,0x26,0x45,0x29,0x48,0x40,0x4D,0x63,
                            0x51,0x66,0x54,0x6A,0x57,0x6D,0x5A,0x71,0x34,0x74,0x37,
                            0x77,0x21,0x7A,0x25,0x43,0x2A,0x46,0x2D,0x4A,0x61};

static uint8_t my_iv[16] = {0x2B,0x4D,0x62,0x51,0x65,0x54,0x68,0x57,0x6D,0x5A,0x71,0x34,
                                0x74,0x37,0x77,0x21};

typedef enum {START = 0, READY_TO_SEND = 1, SENDING_RAW, WAIT_RESPONSE, SENDING_AES_CBC }send_status_t;

static send_status_t state_machine = START;

static void send_aes_cbc_flash_data(esp_websocket_client_handle_t client){
    ESP_LOGI(TAG,"Sending AES CBC Cipher block data");
}

static void send_flash_data(esp_websocket_client_handle_t client){
    int data_len = end_ecg - start_ecg;
    data_len--;
    ESP_LOGI(TAG,"Sending data of %d bytes",data_len);
    for(int i = 0; i < data_len ;){
        i += esp_websocket_client_send_bin(client, (char*)&start_ecg, data_len, portMAX_DELAY);
        ESP_LOGI(TAG,"%d bytes sent",i);
    }

}

//sends the header of the binary data
static void prepare_data_sent(esp_websocket_client_handle_t client,uint8_t type,uint32_t size, uint8_t * key, uint8_t * iv){
    (void)type;
    char * jsonBuffer;
    cJSON * data = cJSON_CreateObject();
    switch(type){
        case 0:
            cJSON_AddStringToObject(data,"crypto","none");
        break;
        case 1:
            cJSON_AddStringToObject(data,"crypto","aes_cbc");
        break;
        case 2:
            cJSON_AddStringToObject(data,"crypto","3DES");
        break;
        default:
            cJSON_AddStringToObject(data,"crypto","Unknown");
        break;

        }
    cJSON_AddNumberToObject(data,"size",size);
    char key_str[65]; //64 + 1 \0 null char
    char iv_str[33];  //32 + 1 \0 null char
    if(key != NULL){
        for(uint8_t i = 0; i < sizeof(key_str)/2;i++){
            sprintf(&key_str[i*2],"%02X",key[i]);
        }
        key_str[64] = '\0';
        cJSON_AddStringToObject(data,"key",key_str);

    }else{
        cJSON_AddStringToObject(data,"key","key_str");
    }
    if(iv != NULL){
        for(uint8_t i = 0; i < sizeof(iv_str)/2;i++){
            sprintf(&iv_str[i*2],"%02X",iv[i]);
        }
        iv_str[32] = '\0';
        cJSON_AddStringToObject(data,"IV",iv_str);

    }else{
        cJSON_AddStringToObject(data,"IV","iv_str");
    }

    jsonBuffer = cJSON_Print(data);
    esp_websocket_client_send_text(client, jsonBuffer, strlen(jsonBuffer), portMAX_DELAY);
}

static void first_message(esp_websocket_client_handle_t client){
    ESP_LOGI(TAG,"Enviando identificacao");
    char * jsonBuffer;
    cJSON * data = cJSON_CreateObject();
    char mac[9];
    mac[8] = '\0';
    cJSON_AddStringToObject(data,"IP","192.168.10.1");
    esp_read_mac((uint8_t*)&mac[0],ESP_MAC_WIFI_SOFTAP);
    cJSON_AddStringToObject(data,"MAC",mac);
    jsonBuffer = cJSON_Print(data);

    esp_websocket_client_send_text(client, jsonBuffer, strlen(jsonBuffer), portMAX_DELAY);

    
}

static void shutdown_signaler(TimerHandle_t xTimer)
{
    ESP_LOGI(TAG, "No data received for %d seconds, signaling shutdown", NO_DATA_TIMEOUT_SEC);
    xSemaphoreGive(shutdown_sema);
    
}

static void process_state_machine(void){

    switch(state_machine){
        case START:
        break;
        
        case READY_TO_SEND:
        first_message(client);
        vTaskDelay(10 / portTICK_RATE_MS);
        state_machine = SENDING_RAW;
        break;

        case SENDING_RAW:
        prepare_data_sent(client,0,300,NULL,NULL);
        vTaskDelay(10 / portTICK_RATE_MS);
        send_flash_data(client);
        state_machine = WAIT_RESPONSE;
        break;

        case WAIT_RESPONSE:
        break;

        case SENDING_AES_CBC:
        break;
    }
        vTaskDelay(10 / portTICK_RATE_MS);

}

static void websocket_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;
    switch (event_id) {
    case WEBSOCKET_EVENT_CONNECTED:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_CONNECTED");
        if(state_machine == START){
            state_machine = READY_TO_SEND;
        }
        break;
    case WEBSOCKET_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_DISCONNECTED");
        break;
    case WEBSOCKET_EVENT_DATA:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_DATA");
        ESP_LOGI(TAG, "Received opcode=%d", data->op_code);
        if (data->op_code == 0x08 && data->data_len == 2) {
            ESP_LOGW(TAG, "Received closed message with code=%d", 256*data->data_ptr[0] + data->data_ptr[1]);
        } else {
            ESP_LOGW(TAG, "Received=%.*s", data->data_len, (char *)data->data_ptr);
        }
        ESP_LOGW(TAG, "Total payload length=%d, data_len=%d, current payload offset=%d\r\n", data->payload_len, data->data_len, data->payload_offset);

        xTimerReset(shutdown_signal_timer, portMAX_DELAY);
        break;
    case WEBSOCKET_EVENT_ERROR:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_ERROR");
        break;
    }
}

static void websocket_app_start(void)
{
    esp_websocket_client_config_t websocket_cfg = {};

    shutdown_signal_timer = xTimerCreate("Websocket shutdown timer", NO_DATA_TIMEOUT_SEC * 1000 / portTICK_PERIOD_MS,
                                         pdFALSE, NULL, shutdown_signaler);
    shutdown_sema = xSemaphoreCreateBinary();

    websocket_cfg.uri = CONFIG_WEBSOCKET_URI;
    websocket_cfg.port = 8765;

    ESP_LOGI(TAG, "Connecting to %s...", websocket_cfg.uri);

    client = esp_websocket_client_init(&websocket_cfg);
    esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, websocket_event_handler, (void *)client);
    esp_websocket_client_start(client);
    xTimerStart(shutdown_signal_timer, portMAX_DELAY);
    char data[32];
    int len;
    //wait connection
    while(esp_websocket_client_is_connected(client) != true){
    vTaskDelay(10 / portTICK_RATE_MS);
    }
    // while(1){
    //     process_state_machine();
    // }
    if (esp_websocket_client_is_connected(client)) {
        len = sprintf(data, "BEGIN");
        ESP_LOGI(TAG, "Sending %s", data);
        esp_websocket_client_send_text(client, data, len, portMAX_DELAY);
    }
    vTaskDelay(1000 / portTICK_RATE_MS);
    prepare_data_sent(client,0,300,NULL,NULL);
    vTaskDelay(1000 / portTICK_RATE_MS);
    send_flash_data(client);
    vTaskDelay(1000 / portTICK_RATE_MS);
    prepare_data_sent(client,1,300,my_key,my_iv);
    // prepare_data_sent(client,1,300,NULL,NULL);
    // vTaskDelay(1000 / portTICK_RATE_MS);
    // prepare_data_sent(client,2,300,NULL,NULL);


    // send_flash_data(client);
    // for(uint32_t i = 0 ; i < 1000 ;){
    //     esp_websocket_client_send_bin(client, (char*)&start_ecg[i], 100, portMAX_DELAY);
    //     i = i + 100;
    //     vTaskDelay(10 / portTICK_RATE_MS);

    // }
    // vTaskDelay(1000 / portTICK_RATE_MS);
    // xSemaphoreTake(shutdown_sema, portMAX_DELAY);
    // esp_websocket_client_close(client, portMAX_DELAY);
    // ESP_LOGI(TAG, "Websocket Stopped");
    // esp_websocket_client_destroy(client);
}

void init_data_transmission(void){
    websocket_app_start();
}
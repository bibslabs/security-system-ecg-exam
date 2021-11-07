
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

static const char *TAG = "DATA TRANSMISSION";
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
//Cabecalho com dado
/**
{
    "MAC":"MAC-ID",
    "Paciente":"Pessoa",
    "criptografia":"aes_cbc",
    "Tamanho":80000,
    "chave publica":"<chave>"
}
criptografia pode ser none,aes_cbc,3DES por enquanto
*/
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

//Processa as respostas do websocket
void process_state_machine(const char * data, size_t data_len){

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

//inicializa o esquema de transmissao
//chamado quanto o websocket é formado
void start_data_transmission(esp_websocket_client_handle_t websocket_client){
    //modulo local recebe o cliente websocket
    client = websocket_client;
}

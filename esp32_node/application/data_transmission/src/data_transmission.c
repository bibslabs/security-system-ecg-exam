
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

#define PING_PERIOD (10)
#define PING_PERIO_MS (PING_PERIOD * 1000)

static const char *TAG = "DATA TRANSMISSION";
static esp_websocket_client_handle_t client;

extern const uint8_t start_ecg[] asm("_binary_dump_txt_start");
extern const uint8_t end_ecg[] asm("_binary_dump_txt_end");

static uint8_t my_key[32] = {0x39,0x79,0x24,0x42,0x26,0x45,0x29,0x48,0x40,0x4D,0x63,
                            0x51,0x66,0x54,0x6A,0x57,0x6D,0x5A,0x71,0x34,0x74,0x37,
                            0x77,0x21,0x7A,0x25,0x43,0x2A,0x46,0x2D,0x4A,0x61};

static uint8_t my_iv[16] = {0x2B,0x4D,0x62,0x51,0x65,0x54,0x68,0x57,0x6D,0x5A,0x71,0x34,
                                0x74,0x37,0x77,0x21};

//maquina de estados da transferenceia dos dados
typedef enum {START = 0, AUTH = 1, HEADER_RAW ,SEND_RAW, WAIT, HEADER_AES, SEND_AES }send_status_t;

static send_status_t state_machine = START;

static TimerHandle_t ping_timer;


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
    jsonBuffer = cJSON_Print(data);
    esp_websocket_client_send_text(client, jsonBuffer, strlen(jsonBuffer), portMAX_DELAY);
}


static void periodic_ping(TimerHandle_t xTimer)
{
    ESP_LOGI(TAG, "Pinging the websocket server");
    
}

/**
 * @brief Envia a primeira mensagem (autenticacao)
 * 
 * envia a primeira mensagem com os seguines dados
 * {
 *   "MAC":"MAC-ID",
 *   "Paciente":"Pessoa",
 *   "chave publica":"<chave>"
 *  }
 * 
 * @param client 
 */
static void first_message(esp_websocket_client_handle_t client){
    ESP_LOGI(TAG,"Enviando identificacao");
    char * jsonBuffer;
    cJSON * data = cJSON_CreateObject();
    char mac[9];
    mac[8] = '\0';
    cJSON_AddStringToObject(data,"IP","192.168.10.1");
    esp_read_mac((uint8_t*)&mac[0],ESP_MAC_WIFI_SOFTAP);
    cJSON_AddStringToObject(data,"MAC",mac);
    cJSON_AddStringToObject(data,"Paciente","Joao");
    cJSON_AddStringToObject(data,"pubkey","2A3B4B1102BEEF");

    jsonBuffer = cJSON_Print(data);

    esp_websocket_client_send_text(client, jsonBuffer, strlen(jsonBuffer), portMAX_DELAY);
    
    
}

static void process_json(cJSON * data){
    //processar json recebido dependendo do estado da maquina de estados

}


//processa as respostas do websocket
void process_state_machine(const char * data, size_t data_len){

    cJSON * parse = cJSON_ParseWithLength(data,data_len);
    if(parse != NULL){
        ESP_LOGI(TAG,"Valid Json");
        process_json(parse);
    }else{
        ESP_LOGE(TAG,"Invalid JSON");
    }
    switch(state_machine){
        case START:
        state_machine = AUTH;
        break;
        case AUTH:
        state_machine = HEADER_RAW;
        break;

        case HEADER_RAW:
        //enviar cabecalho de dado sem criptografia
        prepare_data_sent(client,0,end_ecg - start_ecg,NULL,NULL);
        break;
        case SEND_RAW:
        send_flash_data(client);
        state_machine = WAIT;
        break;

        case WAIT:
        break;

        case SEND_AES:
        send_aes_cbc_flash_data(client);
        break;

        default:
        break;
    }
        // vTaskDelay(10 / portTICK_RATE_MS);

}

//inicializa o esquema de transmissao
//chamado quanto o websocket é formado
void start_data_transmission(esp_websocket_client_handle_t websocket_client){
    //modulo local recebe o cliente websocket
    ESP_LOGI(TAG,"Iniciando  transmissao de dados, enviando cabecalho inicial");
    client = websocket_client;
    ESP_LOGI(TAG,"My key %02x %02x %02x",my_key[0],my_key[1],my_key[2]);
    ESP_LOGI(TAG,"My IV %02x %02x %02x",my_iv[0],my_iv[1],my_iv[2]);
    first_message(client);

    ping_timer = xTimerCreate("Websocket Ping Timer", PING_PERIO_MS / portTICK_PERIOD_MS,
    pdFALSE, NULL, periodic_ping);

}

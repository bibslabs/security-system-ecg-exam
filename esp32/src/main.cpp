#ifdef CORE_DEBUG_LEVEL
#undef CORE_DEBUG_LEVEL
#endif

#define CORE_DEBUG_LEVEL 3
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#define WEBSOCKETS_NETWORK_TYPE NETWORK_ESP8266_ASYNC 

#include <WebSocketsClient_Generic.h>
#include <SocketIOclient_Generic.h>


#include "crypto_test.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "crypto.h"
#include "clefia.h"
#include "base64.h"

// Note: SocketIOClient used ArduinoJson and Websockets library, so you must
// import them into your project.

// Event
// Server send
#define SSAC "server-send-ack-connected"
#define SSM "server-send-message"

// Client send
#define CC "client-connect"
#define CSM "client-send-message"

#define UDP_PORT 3333

//esp list
#define MAC_P "94:B9:7E:C2:DF:B8"
#define TOK_P "PLIMNEUFJEUABGHQUTMN"

#define MAC_M "24:6F:28:17:FA:5C"
#define TOK_M "MHUEMFNAJFIRLOFNEQUX"

#define MAC_G "24:6F:28:17:E9:A0"
#define TOK_G "GJFUEKIGUAJTNDKEOLRM"

////////////////////////////////
// SSID and password WiFi
// const char *ssid = "VIVOFIBRA-C481";
// const char *pwd = "iYuUd3qeGA";

const char *ssid = "FLIN_RFE 201 A";
const char *pwd = "LinhaLIVRE314089";

static uint32_t package_size = 500;

typedef enum state {START, CRYPTO_SET, SEND}state_t;

static state_t esp_state = START;

// Config server side
// const char host[] = "192.168.15.6";
const char host[] = "192.168.169.106";

uint16_t port = 3333; // If you connect to a VPS hosting run HTTP, you must set port = 80.
const char path[] = "/socket.io/?EIO=4"; // If you connect with namespace, set path = "/your-nsp". Default path = "/".

const uint8_t message[] = "Ola boa noite!";

static uint8_t cryptographic_message[256];
static uint8_t encrypted[8192];


SocketIOclient socket;

// static void socket_io_task(void * arg);
static void socketEvent(socketIOmessageType_t type, uint8_t*payload, size_t length);
static void process_state(void);
static DynamicJsonDocument response_json(4096);

DynamicJsonDocument doc(2048);

void serverSendAckConnected(const char *payload, size_t length) {
   Serial.print("Server says: ");
   Serial.println(payload);
}

void serverSendMessage(const char *payload, size_t length) {
   Serial.print("Server says: ");
   Serial.println(payload);
}

const static char * get_tok(void){
   if(memcmp(WiFi.macAddress().c_str(),MAC_P,17) == 0){
      return TOK_P;
   }else if (memcmp(WiFi.macAddress().c_str(),MAC_M,17) == 0){
      return TOK_M;
   }else {
      return TOK_G;
   }
}


static void set_package_size(uint32_t val){
   package_size = val;
}
// Use for customizing event handle function. If you don't want to customize,
// you can ignore it. 
void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length)
{
    switch (type)
    {
    case sIOtype_DISCONNECT:
        Serial.printf("[SIoC] Disconnected!\n");
        esp_state = START;
        break;
    case sIOtype_CONNECT:{
         Serial.print("[IOc] Connected to url: ");
         Serial.println((char*) payload);

         // join default namespace (no auto join in Socket.IO V3)
         socket.send(sIOtype_CONNECT,"/");

        break;
    }
    case sIOtype_EVENT:
      Serial.printf("[SIoC] get event: %s\n", payload);
      deserializeJson(response_json,(unsigned char*)payload);
      if(response_json[0] == "set-crypt"){
         Serial.printf("Eita ! \n");
         String key;
         serializeJson(response_json[1]["key"],key);
         String algo;
         serializeJson(response_json[1]["algo"],algo);
         Serial.printf("Received Crypto Key -> %s \n",key.c_str());
         Serial.printf("Received Crypto Algo-> %s \n",algo.c_str());
         set_crypto(algo.substring(1,algo.length()-1),key.substring(1,key.length()));
         esp_state = CRYPTO_SET;
      }else if (response_json[0] == "reset"){
         esp_state = START;
      }else if (response_json[0] == "throughput-start"){
         if(response_json[1].containsKey("pkgsize")){
            //ajusta tamanho da payload de acordo.
         }
         esp_state = SEND;
      }else if (response_json[0] == "test-cfg"){

         String algo = response_json[1]["algo"];
         String pkg = response_json[1]["pkg_s"];
         Serial.printf("Configurando algoritmo %s com pacote de %s leituras\n",algo.c_str(),pkg.c_str());
         set_package_size(atoi(pkg.c_str()));
         set_crypto_only(algo);
      }

        
      //   Serial.println(payload);
        break;
    case sIOtype_ACK:
        Serial.printf("[SIoC] get ack: %u\n", length);
      //   hexdump(payload, length);
        break;
    case sIOtype_ERROR:
        Serial.printf("[SIoCc] get error: %u\n", length);
      //   hexdump(payload, length);
        break;
    case sIOtype_BINARY_EVENT:
        Serial.printf("[SIoC] get binary: %u\n", length);
      //   hexdump(payload, length);
        break;
    case sIOtype_BINARY_ACK:
        Serial.printf("[SIoC] get binary ack: %u\n", length);
      //   hexdump(payload, length);
        break; 

    }
}

void setup() {
   // put your setup code here, to run once:
   Serial.begin(115200);
   Serial.setDebugOutput(true);

   Serial.println("acordei");
   Serial.println(WiFi.macAddress());
   Serial.println("Meu token");
   Serial.println(get_tok());
   // Connect to wifi
   Serial.print("Connecting to ");
   Serial.print(ssid);

   // if (WiFi.config(staticIP, gateway, subnet, dns, dns) == false) {
   //    Serial.println("Configuration failed.");
   // }
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, pwd);

   //  Wait esp8266 connected to wifi
   while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print('.');
   }

   Serial.println();
   Serial.print("Connected WiFi: ");
   Serial.println(ssid);
   Serial.println("IP address(ESP):");
   Serial.println(WiFi.localIP());

   // Connect to server with namespace

   // or socket.begin(host, port); if you want to connect to root path. Default
   // nsp(path) = "/". If you want to connect to server witn ssl, you can follow
   // code below: socket.beginSSL(ssl_host, path); // Default port = 443.

   // If you want to override event handle function, you can use below code:
   // function was called.
   socket.begin(host, port,"/socket.io/?EIO=3");
   socket.onEvent(socketIOEvent); // this function must be called after begin
   // //  Setup 'on' event listeners
   // socket.onEvent(SSAC, serverSendAckConnected);
   // socket.onEvent(SSM, serverSendMessage);
   // udpSocket.begin(UDP_PORT);
   // Remove event handle functions
   // socket.remove(SSM);
   // test_all_cryptos();

   //tasks do  freeRTOS dao conflito com runtime do arduino
   //devido a implementações nao serem thread safe
   // xTaskCreate(socket_io_task,"sio",1024*16,NULL,2,NULL);

}

// Send message
// void sendMessage(String message) { socket.send(CSM); }

long previousTime = 0;
unsigned long duration = 5000;

void loop() {
   // Loop function
   socket.loop();

   unsigned long now = millis();
   if(esp_state == SEND){
      duration = 0;
   }else{
      duration = 5000;
   }
   if((now - previousTime) > duration){
      previousTime = now;
      if(socket.isConnected()){
         process_state();
      }else{
         Serial.println("Debug: estou offline");
      }
   }
}

static void process_state(void){
   switch(esp_state){
      case START: {
         static int i = 1;
         String output;
         doc[0] = "esp-message";
         doc[1]["data"] = get_tok();
         serializeJson(doc,output);
         // socket.emit("esp-message","output");
         socket.sendEVENT(output);
         Serial.printf("Sent Json %d\n\r",i++);
         break;
      }
      case CRYPTO_SET: {
         char secret[17] = "Ready to start!";
         secret[16] = '\0';
         base64 base;
         String text = base.encode((uint8_t*)secret,16);
         DynamicJsonDocument pacotin(256);
         pacotin[0] = "crypto-set";
         Serial.printf("Base text -> %s \n",text.c_str());
         encrypt_data((uint8_t*)&secret[0],(uint8_t*)&cryptographic_message[0],16);
         String encoded = base.encode(cryptographic_message,16);
         pacotin[1] = encoded;
         text.clear();
         serializeJson(pacotin,text);
         Serial.printf("Send cryptographic payload -> %s\n",text.c_str());
         socket.sendEVENT(text);
         break;
      }
      case SEND: {

         DynamicJsonDocument throughput(8192);

         throughput[0] = "throughput";
         for(uint16_t i = 1; i < package_size ; i++){
            throughput[1]["data"][i-1] = i*23;
         }
         String throughput_str;
         serializeJson(throughput[1],throughput_str);

         base64 base;
         uint32_t len = throughput_str.length() - (throughput_str.length() % 16);
         memset(encrypted,0,sizeof(encrypted));
         encrypt_data((uint8_t*)throughput_str.c_str(),encrypted,throughput_str.length());
         String encoded_encrypt = base.encode(encrypted,throughput_str.length());
         throughput[1] = encoded_encrypt;
         throughput_str.clear();
         serializeJson(throughput,throughput_str);
         socket.sendEVENT(throughput_str);
         break;
      }
      default:
         Serial.printf("Uknown Stage\n");
   }

}

// static void socket_io_task(void * arg){

//    DynamicJsonDocument doc(2048);

//    while(true){
//       if(socket.isConnected()){
//          static int i = 1;
//          // String output;
//          // for(uint8_t j = 0 ; j < 20 ; j++){
//          //    doc["data"][j] = j*i * 3; 
//          // }
//          // serializeJson(doc,output);
//          // socket.emit("esp-message","BATATEX");
//          Serial.printf("Sent Json %d\n\r",i++);
//       }else{
//          Serial.println("Debug: estou offline");
//       }

//       vTaskDelay(pdMS_TO_TICKS(5000));
//    } 
// }
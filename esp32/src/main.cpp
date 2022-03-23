#ifdef CORE_DEBUG_LEVEL
#undef CORE_DEBUG_LEVEL
#endif

#if !defined(ESP32)
  #error This code is intended to run only on the ESP32 boards ! Please check your Tools->Board setting.
#endif

#define _WEBSOCKETS_LOGLEVEL_     2

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include "WiFiUdp.h"

#include <ArduinoJson.h>

#include <WebSocketsClient_Generic.h>
#include <SocketIOclient_Generic.h>
#include "libb64/cencode.h"
#include "ESPNtpClient.h"

#include "crypto_test.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "crypto.h"
#include "Crypto.h"
#include "Curve25519.h"
#include "clefia.h"
#include "base64.h"

// Note: SocketIOClient used ArduinoJson and Websockets library, so you must
// import them into your project.

// Event
// Server send
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

static uint8_t dh_pub[32];
static uint8_t dh_priv[32];
static uint8_t dh_ext_pub[32];
static uint8_t shared_key[32];
typedef enum state {AUTH,DERIVE_KEY,OPEN_SOCKET,START, CRYPTO_SET, SEND}state_t;

static state_t esp_state = AUTH;

// Config server side
// const char host[] = "192.168.15.3";
const char host[] = "192.168.169.106";

const uint16_t port = 3333; // If you connect to a VPS hosting run HTTP, you must set port = 80.
const char path[] = "/socket.io/?EIO=4"; // If you connect with namespace, set path = "/your-nsp". Default path = "/".
const uint16_t udp_port = 41234;
const uint8_t message[] = "Ola boa noite!";

static uint8_t cryptographic_message[256];
static uint8_t encrypted[8192];

WiFiUDP udp;

#define _getCycleCount() micros()

//adquirir ciclos de clock do Program counter

SocketIOclient socket;
#define BIG_BUFS_SIZE 10000
// static void socket_io_task(void * arg);
static void process_state(void);

static DynamicJsonDocument response_json(2048);

static char throughput_str[BIG_BUFS_SIZE],encoded_encrypt[BIG_BUFS_SIZE];

DynamicJsonDocument doc(2048);

static void generate_keys(){
   Curve25519::dh1(dh_pub,dh_priv);
   Serial.printf("Public key ->");
   for(uint8_t i = 0 ; i < 32 ; i++){
      Serial.printf("%02X ",dh_pub[i]);
   }
   Serial.printf("\n\r");
}

static void derive_key(uint8_t * external_pubkey){
   Curve25519::dh2(external_pubkey,dh_priv);
   Serial.printf("derived final key -> ");
   memcpy(shared_key,external_pubkey,32 );
   for(uint8_t i = 0 ; i < 32 ; i++){
      Serial.printf("%02X ",shared_key[i]);
   }
   Serial.printf("\n");
}

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

static decrypt_udp_message(String received){
   char resultado[256];
   decrypt_data((uint8_t*)received.c_str(),(uint8_t*)resultado,received.length());
   Serial.printf("Dado UDP criptografado -> %s",resultado);
}

static void set_package_size(uint32_t val){
   package_size = val;
}
// Use for customizing event handle function. If you don't want to customize,
// you can ignore it. f
void socketIOEvent(const socketIOmessageType_t& type, uint8_t * payload, const size_t& length)
{
    switch (type)
    {
    case sIOtype_DISCONNECT:
      Serial.println("[IOc] Disconnected");
      
      break;
      
    case sIOtype_CONNECT:
      Serial.print("[IOc] Connected to url: ");
      Serial.println((char*) payload);

      // join default namespace (no auto join in Socket.IO V3)
      socket.send(sIOtype_CONNECT, "/");
      
      break;
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
      }else if(response_json[0] == "pub-key"){
         //external public key is received via datagram/http
      }

        
      //   Serial.println(payload);
        break;
    case sIOtype_ACK:
      Serial.print("[IOc] Get ack: ");
      Serial.println(length);
      
      //hexdump(payload, length);
      
      break;
      
    case sIOtype_ERROR:
      Serial.print("[IOc] Get error: ");
      Serial.println(length);
      
      //hexdump(payload, length);
      
      break;
      
    case sIOtype_BINARY_EVENT:
      Serial.print("[IOc] Get binary: ");
      Serial.println(length);
      
      //hexdump(payload, length);
      
      break;
      
    case sIOtype_BINARY_ACK:
       Serial.print("[IOc] Get binary ack: ");
      Serial.println(length);
      
      //hexdump(payload, length);
      
      break;
      
    case sIOtype_PING:
      Serial.println("[IOc] Get PING");

      break;

   case sIOtype_PONG:
      Serial.println("[IOc] Get PONG");

      break;   
      
    default:
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

   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, pwd);


   //  Wait esp8266 connected to wifi
   while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print('.');
   }
   generate_keys();


   Serial.println();
   Serial.print("Connected WiFi: ");
   Serial.println(ssid);
   Serial.println("IP address(ESP):");
   Serial.println(WiFi.localIP());

   NTP.setTimeZone(TZ_Etc_GMTm3);
   NTP.setInterval(1,200);
   NTP.begin("pool.ntp.br",false);
   socket.setReconnectInterval(10000);
   // socket.begin(host, port);
   // socket.onEvent(socketIOEvent); // this function must be called after begin
   udp.begin(41234);
   
}

// Send message
// void sendMessage(String message) { socket.send(CSM); }

long previousTime = 0;
unsigned long duration = 5000;

/**
 * @brief Parse the received public key
 * 
 * @param pubkey 
 */
static void parse_pubkey(const char * pubkey){
         Serial.printf("Chave publica recebida -> %s\n",pubkey);
         for(uint8_t i = 0 ; i < 32 ; i++){
            char val[3] = {pubkey[i*2],pubkey[i*2+1],0};
            dh_ext_pub[i] = strtol(val,NULL,16);
            Serial.printf("%02X ",dh_ext_pub[i]);
         }
         Serial.printf("\n");  
         
         Serial.printf("\nEnviando minha chave pública\n");
         udp.beginPacket(host,udp_port);
         DynamicJsonDocument response_json(1024);
         String mykey_str;
         char hex_buff[65];
         hex_buff[64] = '\0';
         for(uint8_t i = 0 ; i < 32 ; i++){
            sprintf(&hex_buff[i*2],"%02X",dh_pub[i]);
         }
         response_json["pubkey"] = hex_buff;
         serializeJson(response_json,mykey_str);
         udp.printf("%s",mykey_str.c_str());
         udp.endPacket();
         derive_key(dh_ext_pub);
         set_crypto_bytes("AESCBC",dh_ext_pub);
         esp_state = DERIVE_KEY;
}

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
      if(esp_state >= START){//websocket based state
         if(socket.isConnected()){
            // Serial.printf("My time -> %lld \n",NTP.micros());
            process_state();
         }else{
            Serial.println("Debug: estou offline");
            socket.send(sIOtype_CONNECT,path);
            esp_state = START;
         }
      }else{//pre socket http/udp states
      if (udp.parsePacket() > 0)//Se houver pacotes para serem lidos
      {
         String recebido;
         //Após todos os dados serem lidos, a String estara pronta.
         while (udp.available() > 0)//Enquanto houver dados para serem lidos
         {
            char c = udp.read();
            recebido += c;
         }
         Serial.printf("Mensagem -> %s",recebido.c_str());
         DynamicJsonDocument rec_json(1024);
         deserializeJson(rec_json,recebido);
         if(esp_state >= DERIVE_KEY){
            //recebi um pacote UDP após pronto para criptografar
            //descriptografar para ler

         }else{
            if(rec_json["pubkey"] != NULL){
            Serial.printf("Adquirindo chave publica\n");
            parse_pubkey(rec_json["pubkey"]);
            }     
         }





      }
         process_state();
      }

   }
}

static void process_state(void){
   switch(esp_state){
      case AUTH:
         Serial.printf("Authenticating with server\n");
         udp.beginPacket(host,udp_port);
         udp.printf("{\"connection\":\"begin\"}");
         udp.endPacket();
      break;
      case DERIVE_KEY:
      break;
      case OPEN_SOCKET:
         socket.begin(host, port);
         socket.onEvent(socketIOEvent);
      break;
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

         //armazenar o tempo necessario em ciclos de clock para cada operacao
         DynamicJsonDocument throughput(BIG_BUFS_SIZE);
         //primeiro argumento da lista é o evento do socket.io
         throughput.clear();
         throughput[0] = "throughput";
         //segundo argumento é o conteudo em si
         for(uint16_t i = 0; i < package_size ; i++){
            throughput[1]["data"][i] = 1000+i;
         }
         //tempo para criar json
         //transformar objeto em string
         serializeJson(throughput[1],throughput_str,sizeof(throughput_str));
         uint32_t len = strlen(throughput_str)- (strlen(throughput_str) % 16);
         //encriptar os dados
         memset(encrypted,0,sizeof(encrypted));
         encrypt_data((uint8_t*)throughput_str,encrypted,len);
         //encodar os dados encriptados em base64
         base64_encode_chars((char*)encrypted,strlen(throughput_str),encoded_encrypt);
         throughput.clear();
         char micro_str[30];
         sprintf(micro_str,"%lld",NTP.micros());
         throughput[0] = "throughput";
         throughput[1] = encoded_encrypt;
         throughput[2] = micro_str;
         serializeJson(throughput,throughput_str,sizeof(throughput_str));
         socket.sendEVENT(throughput_str,strlen(throughput_str));
         //tempo em microsegundos de cada operacao
         // if((esp_random() %100) == 0){
         //    Serial.printf("microseconds (%u samples, %u bytes) : create_json:%lu, serialize:%lu, encrypt:%lu, encode:%lu, send:%lu\n",package_size,strlen(throughput_str),create_json,serialize,encrypt,encode,send);
         // }
         
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
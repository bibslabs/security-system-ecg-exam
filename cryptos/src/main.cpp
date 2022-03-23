
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "simdjson.h"


#define CBC 1

#define ECB 1

#define CTR 1


//crypto functions
#include "cipher_constants.h"
#include "clefia.h"
#include "present.h"
#include "simon.h"
#include "speck.h"

//windowds crypto engine
// #include "windows.h"
// #include "bcrypt.h"

#include "json.hpp"
#include "base64.h"

#include "aes.h"


using namespace simdjson;

using namespace std;

using json = nlohmann::json;

typedef void (*crypto_func_t)(const uint8_t * input, uint8_t *output, size_t size);

class Config_data {
   public:
      std::string algo;
      std::string mode;
      std::string data;
      std::string key;
      uint8_t bytes[16834];
      std::vector<uint8_t> byte_vec;
      crypto_func_t func;
      unsigned int block_size;
};

static std::vector<uint8_t> result;


static void process_request(Config_data& cfg_data);

//simon and speck config
SimSpk_Cipher simspeck_ctx;
//clefia config
unsigned char rk[8 * 26 + 16];
static int r;
static uint8_t key[32];
static AES_ctx global_ctx;


//crypto library functions implemented for desktop usage
#include "crypto_wrappers.h"
static void set_func(Config_data& cfg_data);

//up to 256  bits key

json response;

static uint8_t giant_array[4096*16];

static std::vector<char> HexToBytes(const std::string& hex) {
  std::vector<char> bytes;

  for (unsigned int i = 0; i < hex.length(); i += 2) {
    std::string byteString = hex.substr(i, 2);
    char byte = (char) strtol(byteString.c_str(), NULL, 16);
    bytes.push_back(byte);
  }

  return bytes;
}

int main(int argc,char *argv[]){

   Config_data cfg;
   // cout << "argumentos" << endl;
   for(int i = 0; i < argc ; i++){
      // cout << i << "-> " <<argv[i] << endl;
   }
   std::string input = argv[1]; 
   padded_string in = input;
   
   dom::object object;
   dom::parser parser;

   auto error = parser.parse(in).get(object); 
   
   if (error) { cerr << error << endl; return error; }

   for (auto [key, value] : object) {
      if(key == "mode"){cfg.mode = value;}
      if(key == "algo"){cfg.algo = value;}
      if(key == "data"){cfg.data = value;}
      if(key == "key"){cfg.key = value;}
   }
   // if(!is_empty(cfg))   
   process_request(cfg);
   memcpy(key,HexToBytes(cfg.key).data(),32);
   result.resize(cfg.byte_vec.size());
   cfg.func(&cfg.bytes[0],&giant_array[0],cfg.byte_vec.size());

   // cfg.func(&cfg.bytes[0],&giant_array[0],16);
   cout << "Data size -> " << cfg.byte_vec.size() << endl;

   std::string test((char*)giant_array);
   cout << "data result -> " << giant_array << endl;
   test = b64encode(test);
   response["result"] = test;
   cerr << response << endl;
   // simdjson::to_json_string(argv[1]);
}


static void process_request(Config_data& cfg_data){

   if(cfg_data.mode.find("encrpyt")){
   }else if(cfg_data.mode.find("decrypt")){
   }else{ 
      //invalid mode
      cout << "INVALID MODE" << endl;
      return;
   }
   if((cfg_data.algo == "AESCBC") ||
      (cfg_data.algo == "CLEFIA") ||
      (cfg_data.algo == "SPECK" ||
      cfg_data.algo == "PRESENT")){

      }else{
         cout << "INVALID ALGO" << endl;
         return;
      }
   std::string test = b64decode(cfg_data.data); //decode the data
   int padding;
   if(test.size() % 16){ //restante nao zero
      padding = 16 - (test.size() % 16);
   }
   test.resize(test.size()+padding,'\0');
   // cfg_data.bytes = (uint8_t*)test.c_str();
   memcpy(cfg_data.bytes,test.c_str(),test.length());
   cfg_data.data = test;
   cout  << "text input -> " << cfg_data.bytes << endl;
   strncpy((char*)key, cfg_data.key.c_str(),32);
   cout << "Key -> "<<key << endl;
   std::vector<uint8_t> vec(test.begin(), test.end());
   cfg_data.byte_vec = vec;
   cout << "base64 decode result -> " << test.size() << " bytes" << test  << endl;
   set_func(cfg_data);
   cout << "Set mode to " << cfg_data.mode << " " <<cfg_data.data.size() << " bytes of data" << endl; 
   cout << "using algo -> "<< cfg_data.algo << " And key -> " << cfg_data.key << endl;


}

static void set_func(Config_data& cfg_data){
   if(cfg_data.algo.compare("AESCBC") == 0){
      if(cfg_data.mode.compare("encrypt") == 0){
         cfg_data.func = AES_CBC_enc_wrapper;
      }else{
         cfg_data.func = AES_CBC_dec_wrapper;
      }
   }
   if(cfg_data.algo.compare("CLEFIA") == 0){
      // cout << "SET ALGO CLEF" << endl;
      r = ClefiaKeySet(rk, key, 256);
      if(cfg_data.mode.compare("encrypt") == 0){
         // cout << "SET ENC" << endl;
         cfg_data.func = clefia_enc_wrap;
      }else{
         // cout << "SET DEC" << endl;
         cfg_data.func = clefia_dec_wrap;
      }
   }
   if(cfg_data.algo.compare("PRESENT") == 0){
      if(cfg_data.mode.compare("encrypt")){
         cfg_data.func = present_enc_wrap;
      }else{
         cfg_data.func = present_dec_wrap;
      }
   }
   if(cfg_data.algo.compare("SPECK") == 0){
      if(cfg_data.mode.compare("encrypt") == 0){
         cfg_data.func = speck_enc_wrap;
      }else{
         cfg_data.func = speck_dec_wrap;
      }
   }
}

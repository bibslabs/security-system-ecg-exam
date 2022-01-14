#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "cJSON.h"

/**
 * @brief Creates a HL7 FHIR json packet
 * 
 * This json is based on the "observation" type of medical
 * exam.
 * 
 * @param data 
 */
void mount_hl7_fhir_ecg(unsigned char * data){
    cJSON * dados = cJSON_CreateObject();
    printf("LETS PRINT SOME CRAP!");
/*
  "resourceType": "Observation",
  "id": "ekg",
  "text": {
    "status": "generated",
    "div": ""
  },
  "status": "final",
*/
    //tipo de dado HL7, observaçao
    cJSON_AddStringToObject(dados,"resourceType","Observation");
    //identificação - eletrocardiograma
    cJSON_AddStringToObject(dados,"id","ekg");
    //texto
    cJSON * text = cJSON_AddObjectToObject(dados,"text");

    //status
    cJSON_AddStringToObject(text,"status","finalizado");
    //div
    cJSON_AddStringToObject(text,"div","o que seria div");
    //final da observação
    cJSON_AddStringToObject(dados,"status","final");
/*
  "category": [
    {
      "coding": [
        {
          "system": "http://terminology.hl7.org/CodeSystem/observation-category",
          "code": "procedure",
          "display": "Procedure"
        }
      ]
    }
  ],
*/

    //category array list, coding array list, e objeto em si
    cJSON * category = cJSON_AddArrayToObject(dados,"category");
    cJSON * code_obj = cJSON_AddObjectToObject(category,"coding");
    cJSON * code_obj_arr = cJSON_AddArrayToObject(code_obj,"coding");
    cJSON * sys_code_disp =  cJSON_CreateObject();
    cJSON_AddStringToObject(sys_code_disp,"system","sistema de observacao");
    cJSON_AddStringToObject(sys_code_disp,"code","procedimento");
    cJSON_AddStringToObject(sys_code_disp,"display","Procedimento");
    cJSON_AddItemToArray(code_obj_arr,sys_code_disp);

/*
  "code": {
    "coding": [
      {
        "system": "urn:oid:2.16.840.1.113883.6.24",
        "code": "131328",
        "display": "MDC_ECG_ELEC_POTL"
      }
    ]
  },
*/

    //code que possui os códigos do paciente, do tipo de observação e o que é mostrado
    cJSON * code = cJSON_AddObjectToObject(dados,"code");
    cJSON * coding2 = cJSON_AddArrayToObject(code,"coding");
    cJSON * sys_code_disp2 = cJSON_CreateObject();
    cJSON_AddStringToObject(sys_code_disp2,"system","urn:oid:2.16.840.1.113883.6.24");
    cJSON_AddStringToObject(sys_code_disp2,"code","131328");
    cJSON_AddStringToObject(sys_code_disp2,"display","MDC_ECG_ELEC_POTL");
    cJSON_AddItemToArray(coding2,sys_code_disp);
/*
  "subject": {
    "reference": "Patient/f001",
    "display": "P. van de Heuvel"
  },
*/
    //dados do paciente
    cJSON * subject = cJSON_AddObjectToObject(dados,"subject");
    cJSON_AddStringToObject(subject,"reference","Patient/ef002");
    cJSON_AddStringToObject(subject,"display","Zezinho da Silva");
    cJSON_AddStringToObject(dados,"effectiveDateTime","2021-02-19T09:30:35+01:00");
    //dados do tecnico ou médico que realizou o exame
/*
  "effectiveDateTime": "2015-02-19T09:30:35+01:00",
  "performer": [
    {
      "reference": "Practitioner/f005",
      "display": "A. Langeveld"
    }
  ],
  "device": {
    "display": "12 lead EKG Device Metric"
  },
*/
    cJSON * performer = cJSON_AddObjectToObject(dados,"performer");
    cJSON_AddStringToObject(performer,"reference","Practioner/z003");
    cJSON_AddStringToObject(performer,"display","Dr. Juquinha Baladoce");
    cJSON_AddStringToObject(dados,"device","ECG Siemens B3000");

/*
  "component": [
    {
      "code": {
        "coding": [
          {
            "system": "urn:oid:2.16.840.1.113883.6.24",
            "code": "131329",
            "display": "MDC_ECG_ELEC_POTL_I"
          }
        ]
      },
      "valueSampledData": {
        "origin": {
          "value": 2048
        },
        "period": 10,
        "factor": 1.612,
        "lowerLimit": -3300,
        "upperLimit": 3300,
        "dimensions": 1,
        "data": "2041 2043 2037 2047 2060 2062 2051 2023 2014 2027 2034 2033 2040 2047 2047 2053 2058 2064 2059 2063 2061 2052 2053 2038 1966 1885 1884 2009 2129 2166 2137 2102 2086 2077 2067 2067 2060 2059 2062 2062 2060 2057 2045 2047 2057 2054 2042 2029 2027 2018 2007 1995 2001 2012 2024 2039 2068 2092 2111 2125 2131 2148 2137 2138 2128 2128 2115 2099 2097 2096 2101 2101 2091 2073 2076 2077 2084 2081 2088 2092 2070 2069 2074 2077 2075 2068 2064 2060 2062 2074 2075 2074 2075 2063 2058 2058 2064 2064 2070 2074 2067 2060 2062 2063 2061 2059 2048 2052 2049 2048 2051 2059 2059 2066 2077 2073"
      }
    },
    {
      "code": {
        "coding": [
          {
            "system": "urn:oid:2.16.840.1.113883.6.24",
            "code": "131330",
            "display": "MDC_ECG_ELEC_POTL_II"
          }
        ]
      },
      "valueSampledData": {
        "origin": {
          "value": 2048
        },
        "period": 10,
        "factor": 1.612,
        "lowerLimit": -3300,
        "upperLimit": 3300,
        "dimensions": 1,
        "data": "2041 2043 2037 2047 2060 2062 2051 2023 2014 2027 2034 2033 2040 2047 2047 2053 2058 2064 2059 2063 2061 2052 2053 2038 1966 1885 1884 2009 2129 2166 2137 2102 2086 2077 2067 2067 2060 2059 2062 2062 2060 2057 2045 2047 2057 2054 2042 2029 2027 2018 2007 1995 2001 2012 2024 2039 2068 2092 2111 2125 2131 2148 2137 2138 2128 2128 2115 2099 2097 2096 2101 2101 2091 2073 2076 2077 2084 2081 2088 2092 2070 2069 2074 2077 2075 2068 2064 2060 2062 2074 2075 2074 2075 2063 2058 2058 2064 2064 2070 2074 2067 2060 2062 2063 2061 2059 2048 2052 2049 2048 2051 2059 2059 2066 2077 2073"
      }
    },
    {
      "code": {
        "coding": [
          {
            "system": "urn:oid:2.16.840.1.113883.6.24",
            "code": "131389",
            "display": "MDC_ECG_ELEC_POTL_III"
          }
        ]
      },
      "valueSampledData": {
        "origin": {
          "value": 2048
        },
        "period": 10,
        "factor": 1.612,
        "lowerLimit": -3300,
        "upperLimit": 3300,
        "dimensions": 1,
        "data": "2041 2043 2037 2047 2060 2062 2051 2023 2014 2027 2034 2033 2040 2047 2047 2053 2058 2064 2059 2063 2061 2052 2053 2038 1966 1885 1884 2009 2129 2166 2137 2102 2086 2077 2067 2067 2060 2059 2062 2062 2060 2057 2045 2047 2057 2054 2042 2029 2027 2018 2007 1995 2001 2012 2024 2039 2068 2092 2111 2125 2131 2148 2137 2138 2128 2128 2115 2099 2097 2096 2101 2101 2091 2073 2076 2077 2084 2081 2088 2092 2070 2069 2074 2077 2075 2068 2064 2060 2062 2074 2075 2074 2075 2063 2058 2058 2064 2064 2070 2074 2067 2060 2062 2063 2061 2059 2048 2052 2049 2048 2051 2059 2059 2066 2077 2073"
      }
    }
  ]
}
*/
    cJSON * component = cJSON_AddObjectToObject(dados,"component");
    cJSON * coding = cJSON_AddObjectToObject(component,"coding");
    cJSON_AddStringToObject(coding,"system","urn:oid:2.16.840.1.113883.6.24");
    cJSON_AddStringToObject(coding,"code","1231452");
    cJSON_AddStringToObject(coding,"display","MDC_ECG_ELEC_POTL_I");


    printf("MY JSON -> %s",cJSON_Print(dados));



}
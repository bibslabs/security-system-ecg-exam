idf_component_register(SRC_DIRS "src"
                    INCLUDE_DIRS "inc" "../../crypto/inc" REQUIRES wpa_supplicant nvs_flash esp-tls esp_http_client esp_websocket_client json
                    EMBED_TXTFILES cert.pem dump.txt)
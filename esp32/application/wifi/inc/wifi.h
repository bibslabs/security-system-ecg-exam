
typedef enum wifi_states {
    START,
    CONNECT,
    CONNECTED,
    ACQUIRE_IP,
    READY,
}wifi_states_t;

void start_wifi(void);
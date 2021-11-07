
typedef enum data_state{
    CHECK_CONNECTION,
    READY_TO_SEND,
    PREPARE_DATA,
    SEND,
    SEND_ERROR,
}data_state_t;

void init_websocket(void);

//data transmission
void start_data_transmission();
void process_state_machine(const char * data, size_t data_len);

typedef enum data_state{
    CHECK_CONNECTION,
    READY_TO_SEND,
    PREPARE_DATA,
    SEND,
    SEND_ERROR,
}data_state_t;

void init_data_transmission(void);


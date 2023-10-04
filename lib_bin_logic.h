#ifndef LIB_BIN_LOGIC_
#define LIB_BIN_LOGIC_

#include "lib_bin_protocol.h"

#define LIB_BIN_PROTOCOL_NO_SEND 1
#define LIB_BIN_PROTOCOL_SEND 0

/**
 * @brief
 *
 */
typedef struct
{
    uint8_t p_reg_adr;                                                               // Адрес регистра
    uint8_t *p_reg_data;                                                             // Указатель на данные регистра
    uint32_t *p_reg_len;                                                             // Указатель на длину данных, хранящихся в регистре
    int8_t (*callback_before_send)(uint8_t id_broadcast, protocol_packet_t *packet); // Указатель на функцию, которая выполняется до отправки ответа. Вернуть результат 0 если хотим отправить ответ, любое другое, если не хотим отправлять. TODO: Проверять broadcast этот пакет и решение отвечать или нет, будду в callback
    void (*callback_after_send)(protocol_packet_t *packet);                          // Указатель на функцию, которая выполняется после отправки ответа.
} lib_bin_logic_reg_t;

/**
 * @brief
 *
 */
typedef struct
{
    uint8_t id;                                            // Индентификатор устройства
    uint8_t id_broadcast;                                  // Индентификатор устройства для выполнения широковещательных команд или записей в регистр
    uint8_t *p_data_rx;                                    // Указатель на буфер приёмника
    uint32_t *p_data_rx_len;                               // Указатель на количество данных в буфере приёмника
    uint8_t *p_data_tx;                                    // Указатель на буфер передатчика
    uint32_t *p_data_tx_len;                               // Указатель на количество данных в буфере передатчика
    lib_bin_logic_reg_t *regs[256];                        // Указатель на экземпляр регистра
    void (*send_data)(uint8_t *p_data, uint32_t data_len); // Указатель на функцию для передачи данных
} lib_bin_logic_handle_t;

/**
 * @brief Добавить регистр в логику, вызывать в конце инициализиции, но перед бесконечным циклом.
 *
 * @param handle указатель на указатель на структуру
 * @param reg указатель на регистр
 */
void lib_bin_logic_add_reg(lib_bin_logic_handle_t *handle, lib_bin_logic_reg_t *reg);

/**
 * @brief Вызывать в бесконечном цикле самым последним
 *
 * @param handle указатель на структуру
 */
void lib_bin_logic_update(lib_bin_logic_handle_t *handle);

#endif // LIB_BIN_LOGIC
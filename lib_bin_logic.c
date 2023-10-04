#include "lib_bin_logic.h"

void lib_bin_logic_add_reg(lib_bin_logic_handle_t *handle, lib_bin_logic_reg_t *reg)
{
    handle->regs[reg->p_reg_adr] = reg;
}

void lib_bin_logic_update(lib_bin_logic_handle_t *handle)
{

    protocol_packet_t packet = {0};

    int8_t result = 0;

    // Если не инициализировали буфер и указатель на длину, то выходим из метода
    if (handle->p_data_rx == NULL || handle->p_data_rx_len == NULL)
    {
        return;
    }

    // Если данных нет, то выходим из метода
    if (*handle->p_data_rx_len == 0)
    {
        return;
    }

    // Парсим
    result = protocol_data_parse(handle->p_data_rx, *handle->p_data_rx_len, &packet);

    // Обнуляем количество данных в буфере приёма
    *handle->p_data_rx_len = 0;

    // Если пакет с ошибкой, то выходим из метода
    if (result != 0)
    {
        return;
    }

    // Если индентификатор назначения пакета не сходится с нашим индентификатором, то выходим из метода
    if (packet.id_dest != handle->id && packet.id_dest != handle->id_broadcast)
    {
        return;
    }

    if (handle->regs[packet.reg]->callback_before_send != NULL)
    {
        // Вызвать callback_before_send перед отправкой, нужен для обновления данных в p_reg_data и p_reg_data_len или выполнения команд
        result = handle->regs[packet.reg]->callback_before_send(handle->id_broadcast, &packet);
    }

    if (handle->send_data != NULL && result == LIB_BIN_PROTOCOL_SEND)
    {
        if (handle->regs[packet.reg]->p_reg_data == NULL ||
            handle->regs[packet.reg]->p_reg_len == NULL)
        {
            return;
        }

        // Создаём пакет
        result = protocol_packet_create(packet.id_src, handle->id, packet.reg,
                                        handle->regs[packet.reg]->p_reg_data, *handle->regs[packet.reg]->p_reg_len,
                                        handle->p_data_tx, handle->p_data_tx_len);
        // Отправляем пакет
        handle->send_data(handle->p_data_tx, *handle->p_data_tx_len);
    }

    if (handle->regs[packet.reg]->callback_before_send != NULL)
    {
        // Вызвать callback_after_send после парсинга или выполнения команд, например перезагрузки
        handle->regs[packet.reg]->callback_after_send(&packet);
    }
}

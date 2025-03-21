//
// Created by Neko on 21.03.2025.
//

#ifndef UART_H
#define UART_H

#include <stdint.h>

#define UART_SPEED (115200)

enum UART_COM_PORT
{
  COM1 = 0x3F8,
  COM2 = 0x2F8,
  COM3 = 0x3E8,
  COM4 = 0x2E8,
  COM5 = 0x5F8,
  COM6 = 0x4F8,
  COM7 = 0x5E8,
  COM8 = 0x4E8
};

enum UART_BAUD_RATE
{
  BAUD_115200 = 1,
  BAUD_57600  = 2,
  BAUD_38400  = 3,
  BAUD_28800  = 4,
  BAUD_23040  = 5,
  BAUD_19200  = 6,
  BAUD_14400  = 8,
  BAUD_12800  = 9,
  BAUD_11520  = 10,
  BAUD_9600   = 12
};

void initialize_uart_port(
  enum UART_COM_PORT port,
  enum UART_BAUD_RATE baud
);

void send_data_uart(
  enum UART_COM_PORT port,
  uint8_t data);

#endif //UART_H

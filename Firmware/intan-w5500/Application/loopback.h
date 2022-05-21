#ifndef _LOOPBACK_H_
#define _LOOPBACK_H_

#include <stdint.h>

int32_t loopback_tcps(uint8_t sn, uint8_t* buf, uint16_t port);
//int32_t loopback_udps(uint8_t sn, uint8_t* buf, uint16_t port);
int32_t rcvonly_tcps(uint8_t sn, uint8_t* buf, uint16_t port);

/************************/
/* Select LOOPBACK_MODE */
/************************/
#define LOOPBACK_MAIN_NOBLOCK    0
#define LOOPBACK_BLOCK_API       1
#define LOOPBACK_NONBLOCK_API    2

#define LOOPBACK_MODE   LOOPBACK_NONBLOCK_API

#endif

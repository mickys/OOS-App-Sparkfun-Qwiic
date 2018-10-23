#if !defined(_UTIL_H_INCLUDED)
#define _UTIL_H_INCLUDED

#include <stdint.h>

#define UTIL_DEBUG                0


#define OMEGA_I2C_DEV_NUM 		0

uint8_t     readRegisterReturn  (uint8_t devAddr, uint8_t addr);
uint16_t    readRegister16      (uint8_t devAddr, uint8_t addr);

int         writeRegister       (uint8_t devAddr, uint8_t addr, uint8_t data);
int         writeRegister16     (uint8_t devAddr, uint8_t addr, uint16_t data);

#endif	/* _UTIL_H_INCLUDED */
/* COMP2215 15/16 Task 4---SAMPLE */

#ifndef _EEPROMFS_H_
#define _EEPROMFS_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/eeprom.h> 



#define MAX_FILES 10
#define EEPROM_LEN 1600
#define EEPROM_START 447
#define BLOCK_SIZE  64



/* Special entries in block list: */
#define END_BLOCK   (UINT8_MAX - 1U)   
#define FREE_BLOCK  UINT8_MAX   

/* If file is unopened, current position is set to: */
#define CLOSED_FILE   UINT16_MAX





void init_eepromfs(void);

void open_for_write(uint8_t filename);
void open_for_append(uint8_t filename);
void open_for_read(uint8_t filename);

void close(uint8_t filename);
void write(uint8_t filename, uint8_t *buff, uint16_t len);
void read(uint8_t filename,  uint8_t *buff, uint16_t len);
void delete(uint8_t filename);


#endif /* _EEPROMFS_H_ */

/*
Each file will need to store it's lenght, name, and start block
*/

/*
 * SWI EEPROM
 */

#define EEPROM_READ							0x1
#define EEPROM_WRITE							0x0

#define AT21CS_OPCODE_FREEZE_ROM_ZONE_STATE		0x1
#define AT21CS_OPCODE_LOCK_SECURITY_REGISTER		0x2
#define AT21CS_OPCODE_ROM_ZONE_REGISTER_ACCESS	0x7
#define AT21CS_OPCODE_EEPROM_ACCESS			0xa
#define AT21CS_OPCODE_SECURITY_REGISTER_ACCESS	0xb
#define AT21CS_OPCODE_MANUFACTURER_ID_READ		0xc
#define AT21CS_OPCODE_STANDARD_SPEED_MODE		0xd
#define AT21CS_OPCODE_HIGH_SPEED_MODE			0xe

bool EEPROMIdentify(void);
bool EEPROMReadManufID(uint8_t slave_address);
bool EEPROMRead(uint8_t slave_address, uint8_t memory_address, uint8_t *data);
bool EEPROMWrite(uint8_t slave_address, uint8_t memory_address, uint8_t data);
bool EEPROMWritePage(uint8_t slave_address, uint8_t memory_address, uint8_t *data, uint8_t length);
bool EEPROMReadSecurity(uint8_t slave_address, uint8_t memory_address, uint8_t *data);
bool EEPROMWriteSecurity(uint8_t slave_address, uint8_t memory_address, uint8_t data);
bool EEPROMDump(uint8_t slave_address);
bool EEPROMLockSecurity(uint8_t slave_address);

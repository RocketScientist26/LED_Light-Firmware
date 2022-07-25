//Hardware
#define W25Q_BLOCKING_TIMEOUT 		10000

//W25Q
#define W25Q_SECTORS 				1024
#define W25Q_SECTOR_SIZE 			4096
#define W25Q_PAGE_SIZE 				256

//Commands
#define W25Q_CMD_WRIRE_ENABLE 		0x06
#define W25Q_CMD_WRITE_DISABLE 		0x04
#define W25Q_CMD_READ_STATUS_R1 	0x05
#define W25Q_CMD_READ_STATUS_R2 	0x35
#define W25Q_CMD_READ_STATUS_R3 	0x15
#define W25Q_CMD_WRITE_STATUS_R 	0x01
#define W25Q_CMD_PAGE_PROGRAM 		0x02
#define W25Q_CMD_QUAD_PAGE_PROGRAM 	0x32
#define W25Q_CMD_BLOCK_ERASE64KB 	0xd8
#define W25Q_CMD_BLOCK_ERASE32KB 	0x52
#define W25Q_CMD_SECTOR_ERASE 		0x20
#define W25Q_CMD_CHIP_ERASE 		0xC7
#define W25Q_CMD_ERASE_SUPPEND 		0x75
#define W25Q_CMD_ERASE_RESUME 		0x7A
#define W25Q_CMD_POWER_DOWN 		0xB9
#define W25Q_CMD_HIGH_PERFORM_MODE 	0xA3
#define W25Q_CMD_CNT_READ_MODE_RST 	0xFF
#define W25Q_CMD_RELEASE_PDOWN_ID 	0xAB
#define W25Q_CMD_MANUFACURER_ID 	0x90
#define W25Q_CMD_READ_UNIQUE_ID 	0x4B
#define W25Q_CMD_JEDEC_ID 			0x9f
#define W25Q_CMD_READ_DATA 			0x03
#define W25Q_CMD_FAST_READ 			0x0B
#define W25Q_CMD_READ_DUAL_OUTPUT 	0x3B
#define W25Q_CMD_READ_DUAL_IO 		0xBB
#define W25Q_CMD_READ_QUAD_OUTPUT 	0x6B
#define W25Q_CMD_READ_QUAD_IO 		0xEB
#define W25Q_CMD_WORD_READ 			0xE3

void W25Q_SPI_Error_Interrupt();
void W25Q_SPI_Receive_DMA_Interrupt();
void W25Q_SPI_Receive_DMA(uint8_t *data, uint16_t length);
void W25Q_SPI_Receive(uint8_t *data, uint16_t length);
void W25Q_SPI_Transmit(uint8_t *data, uint16_t length);
void W25Q_Set_CS(uint8_t cs);

void W25Q_Write_Enable(uint8_t enable);
uint8_t W25Q_Busy();
void W25Q_Erase_Sector(uint16_t sector);
void W25Q_Write_Sector(uint8_t* data, uint16_t sector);
void W25Q_Read_DMA(uint8_t *data, uint16_t length, uint32_t address);
void W25Q_Read(uint8_t *data, uint32_t address, uint16_t length);
void W25Q_Read_Sector(uint8_t *data, uint16_t sector);
void W25Q_Read_Dev_Info(uint8_t *data);
void W25Q_Chip_Erase();

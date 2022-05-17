/*
 * SWI protocol implementation
 */

#define SIO_BUS_SPEED_STANDARD	0
#define SIO_BUS_SPEED_HIGH		1	// AKA "OVERDRIVE"
#define NUM_SIO_BUS_SPEEDS		2

#undef __delay_us
#define __delay_us(d)	__delay32( (d) * (FCY_MHZ))

#define SIO_PERIOD_TYPE_BIT		0
#define SIO_PERIOD_TYPE_DACK	1
#define SIO_PERIOD_TYPE_DRR		2
#define SIO_PERIOD_TYPE_DSCHG	3
#define SIO_PERIOD_TYPE_HLD0	4
#define SIO_PERIOD_TYPE_HTSS	5
#define SIO_PERIOD_TYPE_LOW0	6
#define SIO_PERIOD_TYPE_LOW1	7
#define SIO_PERIOD_TYPE_MRS		8
#define SIO_PERIOD_TYPE_RD		9
#define SIO_PERIOD_TYPE_RESET	10
#define SIO_PERIOD_TYPE_RRT		11
#define NUM_SIO_PERIOD_TYPES	12

#define SIO_RETURN_TIME		2

void SWIInit(void);
bool SWIResetAndDiscoveryResponse(void);
void SWIStartAndStopCondition(void);
bool SWISendAddress(uint8_t addr);
void SWISendData(uint8_t data);
bool SWIReadData(uint8_t *data);
bool SWIGetAck(void);
void SWISendBit(bool b);
bool SWIRecvBit(bool *bit);
void SWISendAck(void);
void SWISendNack(void);
#ifndef TWI_DEFINES_H
#define TWI_DEFINES_H

//------------------------------------------------------------
// DEFINES FOR CONTROLLER RECEIVER. (CRX) (Interrupts used)
//------------------------------------------------------------


// TWCR settings ...

#define CRX_START             (1 << TWINT) | \
                              (1 << TWSTA) | \
                              (1 << TWEN)  | \
                              (1 << TWIE)

#define CRX_REP_START         (CRX_START)

#define CRX_STOP              (1 << TWINT) | \
                              (1 << TWSTO) | \
                              (1 << TWEN)

#define CRX_READ_NACK         (1 << TWINT) | \
                              (1 << TWEN)  | \
                              (1 << TWIE) 


#define CRX_READ_ACK          (CRX_READ_NACK) | \
                              (1 << TWEA)

#define CRX_SEND_READ_ADDRESS (CRX_READ_NACK)

#define CRX_TRANSMIT          (CRX_READ_NACK)


// TWSR Status codes.

#define CRX_STATUS ((TWSR) & 0xF8)

#define CRX_ILLEGAL_START_STOP 0x00

#define CRX_START_SENT         0x08
#define CRX_RESTART_SENT       0x10
#define CRX_ARBIT_LOST         0x38
#define CRX_SLAR_ACK_RCVD      0x40
#define CRX_SLAR_NACK_RCVD     0x48
#define CRX_DATA_ACK_SENT      0x50
#define CRX_DATA_NACK_SENT     0x58


# endif // TWI_DEFINES_H

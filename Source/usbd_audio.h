#ifndef __USBD_AUDIO_H
#define __USBD_AUDIO_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "usbd_ioreq.h"
#include "device.h"

#define AUDIO_DESCRIPTOR_TYPE                         0x21
#define USB_DEVICE_CLASS_AUDIO                        0x01

/* USB AUDIO class bRequest types */
#define AUDIO_REQ_SET_CUR                             0x01
#define AUDIO_REQ_GET_CUR                             0x81
#define AUDIO_REQ_SET_MIN                             0x02
#define AUDIO_REQ_GET_MIN                             0x82
#define AUDIO_REQ_SET_MAX                             0x03
#define AUDIO_REQ_GET_MAX                             0x83
#define AUDIO_REQ_SET_RES                             0x04
#define AUDIO_REQ_GET_RES                             0x84
#define AUDIO_REQ_SET_MEM                             0x05
#define AUDIO_REQ_GET_MEM                             0x85
#define AUDIO_REQ_GET_STAT                            0xff   

#define AUDIO_IN_STREAMING_CTRL                       0x03
#define AUDIO_OUT_STREAMING_CTRL                      0x04

#define AUDIO_RX_PACKET_SIZE                          (uint16_t)(((USBD_AUDIO_FREQ * USB_AUDIO_CHANNELS * AUDIO_BYTES_PER_SAMPLE) /1000)) 
#define AUDIO_TX_PACKET_SIZE                          (uint16_t)(((USBD_AUDIO_FREQ * USB_AUDIO_CHANNELS * AUDIO_BYTES_PER_SAMPLE) /1000))
#define AUDIO_FB_PACKET_SIZE                          3U
    
/* Number of sub-packets in the audio transfer buffer. You can modify this value but always make sure
  that it is an even number and higher than 3 */
#define AUDIO_RX_PACKET_NUM                           1
#define AUDIO_TX_PACKET_NUM                           1
/* Total size of the audio transfer buffer */
#define AUDIO_RX_TOTAL_BUF_SIZE                       ((size_t)(AUDIO_RX_PACKET_SIZE * AUDIO_RX_PACKET_NUM))
/* Total size of the IN (i.e. microphopne) transfer buffer */
#define AUDIO_TX_TOTAL_BUF_SIZE                       ((size_t)(AUDIO_TX_PACKET_SIZE * AUDIO_TX_PACKET_NUM))

#define MIDI_DATA_IN_PACKET_SIZE       0x40
#define MIDI_DATA_OUT_PACKET_SIZE      0x40
   
#define USBD_EP_ATTR_ISOC_NOSYNC                          0x00 /* attribute no synchro */
#define USBD_EP_ATTR_ISOC_ASYNC                           0x04 /* attribute synchro by feedback  */
#define USBD_EP_ATTR_ISOC_ADAPT                           0x08 /* attribute synchro adaptative  */
#define USBD_EP_ATTR_ISOC_SYNC                            0x0C /* attribute synchro synchronous  */


#if 1
/* Audio Control Requests */
#define AUDIO_CONTROL_REQ                             0x01U
/* Feature Unit, UAC Spec 1.0 p.102 */
#define AUDIO_CONTROL_REQ_FU_MUTE                     0x01U
#define AUDIO_CONTROL_REQ_FU_VOL                      0x02U

/* Audio Streaming Requests */
#define AUDIO_STREAMING_REQ                           0x02U
#define AUDIO_STREAMING_REQ_FREQ_CTRL                 0x01U
#define AUDIO_STREAMING_REQ_PITCH_CTRL                0x02U

/* Volume. See UAC Spec 1.0 p.77 */
#ifndef USBD_AUDIO_VOL_DEFAULT
#define USBD_AUDIO_VOL_DEFAULT                        0x8d00U
#endif
#ifndef USBD_AUDIO_VOL_MAX
#define USBD_AUDIO_VOL_MAX                            0x0000U
#endif
#ifndef USBD_AUDIO_VOL_MIN
#define USBD_AUDIO_VOL_MIN                            0x8100U
#endif
#ifndef USBD_AUDIO_VOL_STEP
#define USBD_AUDIO_VOL_STEP                           0x0100U
#endif /* Total number of steps can't be too many, host will complain. */

/** 
 * The minimum distance that the wr_ptr should keep before rd_ptr to 
 * prevent overwriting unplayed buffer
 */
#define USBD_AUDIO_FREQ_MAX USBD_AUDIO_FREQ
#define AUDIO_BUF_SAFEZONE                            ((uint16_t)((USBD_AUDIO_FREQ_MAX / 1000U + 1) * 2U * 4U))

#endif

typedef enum
{
  AUDIO_OFFSET_NONE = 0,
  AUDIO_OFFSET_HALF,
  AUDIO_OFFSET_FULL,
  AUDIO_OFFSET_UNKNOWN,
}
AUDIO_OffsetTypeDef;
/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
 typedef struct
{
   uint8_t cmd;                    /* bRequest */
   uint8_t req_type;               /* bmRequest */
   uint8_t cs;                     /* wValue (high byte): Control Selector */
   uint8_t cn;                     /* wValue (low byte): Control Number */
   uint8_t unit;                   /* wIndex: Feature Unit ID, Extension Unit ID, or Interface, Endpoint */
   uint8_t len;                    /* wLength */
   uint8_t data[USB_MAX_EP0_SIZE]; /* Data */
}
USBD_AUDIO_ControlTypeDef;



typedef struct
{
  uint8_t                   ac_alt_setting, tx_alt_setting, rx_alt_setting, midi_alt_setting;
#ifdef USE_USBD_AUDIO_TX
  uint8_t                   audio_tx_buffer[AUDIO_TX_TOTAL_BUF_SIZE];
  volatile uint8_t          audio_tx_active;
#endif
#ifdef USE_USBD_AUDIO_RX
  uint8_t                   audio_rx_buffer[AUDIO_RX_TOTAL_BUF_SIZE];
  volatile uint8_t          audio_rx_active;
#endif
#ifdef USE_USBD_MIDI
  uint8_t                   midi_rx_buffer[MIDI_DATA_OUT_PACKET_SIZE];
  volatile uint8_t          midi_tx_lock;
#endif
  int16_t                   volume;
#if 1
  AUDIO_OffsetTypeDef       offset;
  uint8_t                   rd_enable;
  uint16_t                  rd_ptr;
  uint16_t                  wr_ptr;
  uint32_t                  frequency;
  uint32_t                  bit_depth;
#endif
  USBD_AUDIO_ControlTypeDef control;   
}
USBD_AUDIO_HandleTypeDef; 

extern USBD_ClassTypeDef  USBD_AUDIO;
#define USBD_AUDIO_CLASS    &USBD_AUDIO

uint8_t  USBD_AUDIO_RegisterInterface  (USBD_HandleTypeDef   *pdev, 
                                        void *fops);

void usbd_audio_tx_start_callback(uint16_t rate, uint8_t channels);
void usbd_audio_tx_stop_callback();
void usbd_audio_tx_callback(uint8_t* data, size_t len);
void usbd_audio_rx_start_callback(uint16_t rate, uint8_t channels);
void usbd_audio_rx_stop_callback();
size_t usbd_audio_rx_callback(uint8_t* data, size_t len);
void usbd_audio_mute_callback(int16_t gain);
void usbd_audio_gain_callback(int16_t gain);
void usbd_audio_sync_callback(uint8_t gain);
void usbd_audio_write(uint8_t* buffer, size_t len);

void usbd_midi_rx(uint8_t *buffer, uint32_t length);
void usbd_midi_tx(uint8_t* buffer, uint32_t length);
uint8_t usbd_midi_connected(void);
uint8_t usbd_midi_ready(void);

#ifdef __cplusplus
}
#endif

#endif  /* __USBD_AUDIO_H */

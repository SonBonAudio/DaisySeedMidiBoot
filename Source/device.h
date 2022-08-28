#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "hardware.h"

#define FIRMWARE_VERSION_MAJOR       24
#define FIRMWARE_VERSION_MINOR       2
#define _FW_STR(x)                   #x
#define _FW_TO_STR(x)                _FW_STR(x)
#define FIRMWARE_VERSION             "v" _FW_TO_STR(FIRMWARE_VERSION_MAJOR) "." _FW_TO_STR(FIRMWARE_VERSION_MINOR)

#ifndef AUDIO_OUTPUT_GAIN
#define AUDIO_OUTPUT_GAIN            112
#endif
#ifndef AUDIO_INPUT_GAIN
#define AUDIO_INPUT_GAIN             92
#endif

#define MIDI_OMNI_CHANNEL            (-1)
#ifndef MIDI_INPUT_CHANNEL
#define MIDI_INPUT_CHANNEL           MIDI_OMNI_CHANNEL
#endif
#ifndef MIDI_OUTPUT_CHANNEL
#define MIDI_OUTPUT_CHANNEL          0
#endif
#ifndef DIGITAL_BUS_ENABLED
#define DIGITAL_BUS_ENABLED          0
#define DIGITAL_BUS_FORWARD_MIDI     0
#endif
#define USE_USBD_MIDI
#define USE_MIDI_TX_BUFFER
#define USE_MIDI_CALLBACK
#define MIDI_OUTPUT_BUFFER_SIZE      128
#define MIDI_INPUT_BUFFER_SIZE       64
#define MIDI_SYSEX_BUFFER_SIZE       256

#ifndef USBD_MAX_POWER
#define USBD_MAX_POWER               100 // 200mA
#endif

#define OWLBOOT_MAGIC_NUMBER        0xDADAB007
#define OWLBOOT_LOOP_NUMBER         0xDADADEAD
#define OWLBOOT_MAGIC_ADDRESS       ((uint32_t*)0x2000FFF0)

#ifndef STORAGE_MAX_BLOCKS
#define STORAGE_MAX_BLOCKS           64
#endif

#ifndef MINIMAL_BUILD
#define USE_FFT_TABLES
#define USE_FAST_POW
#endif

#define DEBUG_DWT
/* #define DEBUG_STACK */
#define DEBUG_STORAGE
/* #define DEBUG_BOOTLOADER */

#ifdef SSD1331
#define OLED_WIDTH		     96
#define OLED_HEIGHT		     64
#define OLED_BUFFER_SIZE             (OLED_WIDTH*OLED_HEIGHT/8)
#elif defined SEPS114A
#define OLED_WIDTH		     96
#define OLED_HEIGHT		     96
#define OLED_BUFFER_SIZE             (OLED_WIDTH*OLED_HEIGHT*sizeof(uint16_t))
#elif defined SSD1309
  #ifdef OLED_I2C
#define OLED_WIDTH		     64
#define OLED_HEIGHT		     32
  #else
#define OLED_WIDTH		     128
#define OLED_HEIGHT	       64
  #endif
#define OLED_BUFFER_SIZE             (OLED_WIDTH*OLED_HEIGHT/8)
#endif

#ifndef MAX_SYSEX_FIRMWARE_SIZE
#define MAX_SYSEX_FIRMWARE_SIZE      ((16+16+64+128+128)*1024) // FLASH sectors 2-6
#endif
#ifndef MAX_SYSEX_BOOTLOADER_SIZE
#define MAX_SYSEX_BOOTLOADER_SIZE    (64 * 1024) // OWL1 uses 32kb, must be overridden
#endif
#ifndef MAX_SYSEX_PROGRAM_SIZE
#define MAX_SYSEX_PROGRAM_SIZE       (144 * 1024)
#endif
#ifdef USE_BOOTLOADER_MODE // Flag to choose if we're flashing firmware or bootloader from SySex
#define MAX_SYSEX_PAYLOAD_SIZE       MAX_SYSEX_FIRMWARE_SIZE
#else
#define MAX_SYSEX_PAYLOAD_SIZE       (1 * 1024 * 1024) // Maximum resource size
#endif
#define BOOTLOADER_MAGIC             0xB007C0DE
#define BOOTLOADER_VERSION           "v0.3"

#ifndef MAX_SYSEX_PROGRAM_SIZE
#define MAX_SYSEX_PROGRAM_SIZE       (128*1024) // 128k, one flash sector
#endif
#define MAX_FACTORY_PATCHES          36
#define MAX_USER_PATCHES             4
#ifndef STORAGE_SECTOR_SIZE
#define STORAGE_SECTOR_SIZE          (128 * 1024)
#endif

#ifndef MAX_NUMBER_OF_PATCHES
#define MAX_NUMBER_OF_PATCHES        40
#endif
#ifndef MAX_NUMBER_OF_RESOURCES
#define MAX_NUMBER_OF_RESOURCES      12
#endif
#define APPLICATION_SETTINGS_RESOURCE_INDEX (MAX_NUMBER_OF_PATCHES + 1)
#define APPLICATION_SETTINGS_NAME    "__SETTINGS__"
#ifndef MAX_NUMBER_OF_RESOURCES
#define MAX_NUMBER_OF_RESOURCES      12
#endif

#define CODEC_BLOCKSIZE              64
#define CODEC_BUFFER_SIZE            (2*AUDIO_CHANNELS*CODEC_BLOCKSIZE)

/* +0db in and out */
#ifndef AUDIO_INPUT_OFFSET
#define AUDIO_INPUT_OFFSET           0xffffefaa /* -0.06382 * 65535 */
#endif
#ifndef AUDIO_INPUT_SCALAR
#define AUDIO_INPUT_SCALAR           0xfffbb5c7 /* -4.290 * 65535 */
#endif
#ifndef AUDIO_OUTPUT_OFFSET
#define AUDIO_OUTPUT_OFFSET          0x00001eec /* 0.1208 * 65535 */
#endif
#ifndef AUDIO_OUTPUT_SCALAR
#define AUDIO_OUTPUT_SCALAR          0xfffb5bab /* -4.642 * 65535 */
#endif
#define DEFAULT_PROGRAM              1
#define BUTTON_PROGRAM_CHANGE
#define AUDIO_BITDEPTH               24    /* bits per sample */
#define AUDIO_DATAFORMAT             24
#define AUDIO_CODEC_MASTER           true
#ifndef AUDIO_CHANNELS
#define AUDIO_CHANNELS               2
#endif
#ifndef AUDIO_SAMPLINGRATE
#define AUDIO_SAMPLINGRATE           48000
#endif
#define AUDIO_BLOCK_SIZE             CODEC_BLOCKSIZE   /* size in samples of a single channel audio block */
#define AUDIO_MAX_BLOCK_SIZE         (CODEC_BUFFER_SIZE/4)

#define USBD_AUDIO_RX_FREQ           AUDIO_SAMPLINGRATE
#define USBD_AUDIO_TX_FREQ           AUDIO_SAMPLINGRATE
#ifndef USBD_AUDIO_RX_CHANNELS
#define USBD_AUDIO_RX_CHANNELS       AUDIO_CHANNELS
#endif
#ifndef USBD_AUDIO_TX_CHANNELS
#define USBD_AUDIO_TX_CHANNELS       AUDIO_CHANNELS
#endif

#ifndef MAIN_LOOP_SLEEP_MS
#define MAIN_LOOP_SLEEP_MS           2
#endif

#ifndef DAC_PERIPH
#define DAC_PERIPH hdac
#endif

#ifndef LOAD_INDICATOR_PARAMETER
#define LOAD_INDICATOR_PARAMETER     PARAMETER_A
#endif

#ifndef PROGRAM_TASK_STACK_SIZE
#define PROGRAM_TASK_STACK_SIZE      (4*1024/sizeof(portSTACK_TYPE))
#endif
#ifndef MANAGER_TASK_STACK_SIZE
#define MANAGER_TASK_STACK_SIZE      (1024/sizeof(portSTACK_TYPE))
#endif
#ifndef FLASH_TASK_STACK_SIZE
#define FLASH_TASK_STACK_SIZE        (512/sizeof(portSTACK_TYPE))
#endif
#ifndef UTILITY_TASK_STACK_SIZE
#define UTILITY_TASK_STACK_SIZE      (512/sizeof(portSTACK_TYPE))
#endif
#ifdef DAISY
#define ARM_CYCLES_PER_SAMPLE        (480000000/AUDIO_SAMPLINGRATE) /* 480MHz / 48kHz */
#else
#define ARM_CYCLES_PER_SAMPLE        (168000000/AUDIO_SAMPLINGRATE) /* 168MHz / 48kHz */
#endif

/* The "__" prefix would be used by UI controller to prevent user from deleting system resources */
#define RESOURCE_SETTINGS_NAME       "__SETTINGS__"
#define RESOURCE_FFT_LUT_NAME        "__FFT_LUT__"
#define RESOURCE_LOG_LUT_NAME        "__LOG_LUT__"
#define RESOURCE_POW_LUT_NAME        "__POW_LUT__"
#define FFT_LUT_SIZE                 120104U

#ifdef OWL_ARCH_H7
  #define DMA_RAM                      __attribute__ ((section (".nocache")))
  #define CACHE_ALIGNED                __attribute__ ((aligned(32)))
  #define ITCM                         __attribute__ ((section (".itcm")))
  #define TABLE                        __attribute__ ((section (".tables")))
#else
  #define DMA_RAM
  #define CACHE_ALIGNED
  #define ITCM
  #define TABLE
#endif
#ifdef USE_FATFS
  #define SD_BUF                      __attribute__ ((section (".sd_buf")))
#endif

#define USE_IWDG                     // compile with support for IWDG watchdog */
#if defined(USE_IWDG) && !defined(IWDG_PERIPH)
#define IWDG_PERIPH IWDG
#endif

#ifndef NO_EXTERNAL_RAM
#define USE_EXTERNAL_RAM
#endif

#ifndef NO_CCM_RAM
#define USE_CCM_RAM
#define CCM_RAM                          __attribute__ ((section (".ccmdata")))
#else
#define CCM_RAM
#endif

#ifndef DMA_RAM
#define DMA_RAM
#endif

#if defined USE_USBD_FS
#define USB_OTG_BASE_ADDRESS  USB_OTG_FS   
#elif defined USE_USBD_HS
#define USB_OTG_BASE_ADDRESS  USB_OTG_HS   
#endif

#define USB_DIEPCTL(ep_addr) ((USB_OTG_INEndpointTypeDef *)((uint32_t)USB_OTG_BASE_ADDRESS + USB_OTG_IN_ENDPOINT_BASE \
	    + (ep_addr&0x7FU)*USB_OTG_EP_REG_SIZE))->DIEPCTL
#define USB_DOEPCTL(ep_addr) ((USB_OTG_OUTEndpointTypeDef *)((uint32_t)USB_OTG_BASE_ADDRESS + \
	     USB_OTG_OUT_ENDPOINT_BASE + (ep_addr)*USB_OTG_EP_REG_SIZE))->DOEPCTL

#define USB_CLEAR_INCOMPLETE_IN_EP(ep_addr)     if((((ep_addr) & 0x80U) == 0x80U)){ \
    USB_DIEPCTL(ep_addr) |= (USB_OTG_DIEPCTL_EPDIS | USB_OTG_DIEPCTL_SNAK); \
  };

#define USB_DISABLE_EP_BEFORE_CLOSE(ep_addr)			\
  if((((ep_addr) & 0x80U) == 0x80U))				\
    {								\
      if (USB_DIEPCTL(ep_addr)&USB_OTG_DIEPCTL_EPENA_Msk)	\
	{							\
	  USB_DIEPCTL(ep_addr)|= USB_OTG_DIEPCTL_EPDIS;		\
	}							\
    } ;

#define IS_ISO_IN_INCOMPLETE_EP(ep_addr,current_sof, transmit_soffn) ((USB_DIEPCTL(ep_addr)&USB_OTG_DIEPCTL_EPENA_Msk)&& \
								      (((current_sof&0x01) == ((USB_DIEPCTL(ep_addr)&USB_OTG_DIEPCTL_EONUM_DPID_Msk)>>USB_OTG_DIEPCTL_EONUM_DPID_Pos)) \
								       ||(current_sof== ((transmit_soffn+2)&0x7FF))))

#define USB_SOF_NUMBER() ((((USB_OTG_DeviceTypeDef *)((uint32_t )USB_OTG_BASE_ADDRESS + USB_OTG_DEVICE_BASE))->DSTS&USB_OTG_DSTS_FNSOF)>>USB_OTG_DSTS_FNSOF_Pos)

#endif /* __DEVICE_H__ */

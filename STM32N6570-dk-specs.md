# STM32N657X0H3Q

High performance Arm Cortex-M55 MCU, 800MHz, 4.2MB SRAM, Neural-ART accelerator 600 GOPS, NeoChrom GPU, JPEG Codec, H-264 enc, HW crypto
 
Product is in mass production
Unit Price for 10kU (US$) : 10.7254
  VFBGA 264 14x14x1 P 0.8 mm  

# Boards: NUCLEO-N657X0-Q - STM32N6570-DK

The STM32N6x5xx and STM32N6x7xx devices are based on the high-performance Arm® Cortex®-M55, operating at a frequency up to 800 MHz. The Cortex®-M55 core features the Arm® Helium™ vector processing technology. On top of standard microcontroller tasks, this core enables energy-efficient digital signal processing. The Cortex®-M55 is equipped with a floating-point unit (FPU) that supports single- and half-precision (IEEE 754 compliant) data-processing. The Cortex®-M55 includes a 32-Kbyte ICACHE, a 32-Kbyte DCACHE, as well as 128-Kbyte data TCM RAM and 64-Kbyte instruction TCM RAM with ECC for critical real-time routines.

These microcontrollers have TrustZone®-aware support and a memory protection unit (MPU) for enhanced application security. A secure boot ROM ensures secure booting from external interfaces.
The devices embed a 4.2-Mbyte contiguous SRAM organized in several banks, an 8-Kbyte backup SRAM active in VBAT mode, and a flexible external memory controller (FMC) for static memories, XSPI 8-/16-bit configurations.

The STM32N6x7xx devices feature an ST Neural-ART accelerator, running at a maximum frequency of 1 GHz, and providing 600 Gops using optimized hardware units for DNN (deep-neural network) inference functions to optimize power efficiency. Dedicated streaming engines are integrated into it to optimize data flow and minimize internal buffer usage and power. The accelerator supports on-the-fly weight decompression and real-time data encryption and decryption.

The Neo-Chrom graphic accelerator ensures efficient 2.5D graphic processing, by providing hardware acceleration for functions like scaling, using high-quality interpolation, free rotation, alpha blending, texture mapping, and perspective transformation.
For camera applications, a parallel and CSI interface together with an integrated hardware ISP is foreseen. The ISP provides processing of three parallel pipes on the same input stream. Supported algorithms are bad pixel, decimation, black-level tuning, exposure control, de-mosaicking, column conversion, contrast, cropping, downsizing, ROI isolation, gamma correction, YUV conversion, and pixel packer. The ISP output can be directly fed via a DMA to the NPU.
Optionally, the devices embed a hardware H264 encoding block supporting baseline profile, main profile and high profile level 1 to level 5.2, supporting frame rates of up to 30 frames per second for 1080p resolution.

A dedicated hardware accelerator ensures fast and simple JPEG and motion JPEG compression and decompression.
The devices offer an extensive range of enhanced I/Os and peripherals, and operate in the -40 to +125 °C temperature range, from 1.71 to 3.6 V power supply. A comprehensive set of low-power modes (Sleep, Stop, and Standby) allows the design of low-power applications.
The devices are offered in six VFBGA packages, ranging from 142 to 264 pins.

# Features

Includes ST state-of-the-art patented technology 

## Core 

Arm® 32-bit Cortex®-M55, 3360 CoreMark®, frequency up to 800 MHz, 1280 DMIPS, 32-Kbyte ICACHE, 32-Kbyte DCACHE 
Arm® MVE (M-Profile vector extension), Helium™ technology, TrustZone® MPU, NVIC 
Single and half-precision floating point unit (FPU) supports vector and scalar half-, single- and double-precision floating-point datatypes 
Neural processing unit (STM32N6x7 only) 
ST Neural-ART Accelerator, frequency up to 1 GHz, 600 Gops, 288 MAC/cycle 
Specialized hardware units for DNN (deep-neural network) inference functions 
Flexible dedicated stream processing engine 
Real-time encryption/decryption 
On-the-fly weight decompression 

## Memories 

4.2-Mbyte contiguous SRAM 
128-Kbyte TCM (tightly-coupled memory) RAM with ECC for critical real-time data + 64-Kbyte instruction TCM RAM with ECC for critical real-time routines 
8-Kbyte backup SRAM active in VBAT mode 
Flexible external memory controller with cypher engine supporting up to 32-bit data bus: SRAM, PSRAM, SDRAM/LPSDR SDRAM, NOR/NAND memories 
XSPI with support for serial PSRAM, NAND, NOR, HyperRAM™/ HyperFlash™ frame formats 
2 ports: XSPIM 8- and 16-bit configuration up to 200 MHz 

## Graphics 

Neo-Chrom 2.5D GPU: scaling, rotation, alpha blending, texture mapping, perspective transformation 
Chrom-ART Accelerator (DMA2D) 
Hardware JPEG codec with MJPEG 
LCD-TFT controller up to XGA resolution 

## Video 

Parallel and 2-lane CSI-2 camera interfaces 
ISP (image signal processor) with three parallel pipes on the same input stream: bad pixel, decimation, black level, exposure, de-mosaic, column conversion, contrast, crop, downsize, ROI, gamma, YUV convention, pixel packer 
H264 video encoding acceleration: baseline profile, main profile, high profile level 1 to level 5.2, 1080p30 and 720p60 
Security and cryptography 
Arm® TrustZone® and securable I/Os memories and peripherals 
SESIP Level 3 (security evaluation standard for IoT platforms), Arm® PSA (platform security architecture) certified 
Flexible life-cycle scheme with RDP and password-protected debug 
Secure provisioning of customer keys in OTP (one-time programmable) fuses 
Secure boot code in ROM, decrypting and authenticating customer uRoT (updatable root-of-trust) 
Secure data storage with hardware-unique key (HUK) 
Secure firmware upgrade support with TF-M (trusted firmware-M) 
Two AES coprocessors, including one with DPA (differential power analysis) resistance 
Public key accelerator (PKA), DPA resistant 
On-the-fly encryption/decryption of external memories 
HASH hardware accelerator 
True random number generator (RNG), NIST SP800-90B compliant 
96-bit unique ID 
1.5-Kbyte OTP fuses 
Active tampers 

## Communication peripherals 

2x USB 2.0 high-speed/full-speed device/host OTG controllers (one with UCPD USB Type-C® Power Delivery) 
10-Mbit, 100-Mbit, and 1-Gbit Ethernet with TSN (time-sensitive networking) 
4x I2C Fm+ interfaces (SMBus/PMBus®) + 2x I3C 
6x SPI, of which four I2S-capable 
2x SAI, with four DMIC support 
5x USART, 5x UART (ISO78916 interface, LIN, IrDA, up to 12.5 Mbit/s) + 1x LPUART 
2x SDMMC: MMC version 4.0, CE-ATA version 1.0, and SD version 1.0.1 
3x FDCAN with TTCAN capability 

## Low power 

Sleep, Stop and Standby modes 
VBAT supply for RTC, 32x 32-bit backup registers + 8-Kbyte backup SRAM 
Timers and watchdogs 
4x 32-bit timers with up to four IC/OC/PWM or pulse counters and quadrature (incremental) encoder input (up to 240 MHz) 
2x 16-bit advanced motor control timers (up to 240 MHz) 
13x 16-bit general-purpose and 5x 16-bit low-power timers (up to 240 MHz) 
2x watchdogs (independent and window) 
1x SysTick timer 
RTC with subsecond accuracy and hardware calendar 

## Debug 

Development support: serial-wire debug, JTAG 
Embedded Trace Macrocell™ (ETM) 
General-purpose I/Os 
Up to 165 pins 
Analog peripherals 
1x temperature sensor 
2x ADCs with 12-bit maximum resolution (up to 5 Msps), up to 20 channels 
1x ADF filter with SAD and 1x MDF (six filters) 
Reset and power management 
POR, PDR, PBVD, and BOR 
Embedded SMPS step-down converter providing VDDCORE 
1.71 to 3.6 V application supply and I/Os 
Dedicated power for USB and XSPIM1, XSPIM2, SDMMC1, and SDMMC2 I/Os 
Backup regulator (~0.9 V) 
Voltage reference for analog peripheral (VREF+) 

## Clock management 

Internal oscillators: 64 MHz HSI, 4 MHz MSI, 32 kHz LSI 
External oscillators: 16 to 48 MHz HSE, 32.768 kHz LSE 
4x PLL (one for the system clock, one for the ST Neural-ART Accelerator, two for kernel clocks) with fractional mode 

## ECOPACK2 compliant packages 

*************
Configuration
*************


Microcontroller
===============

The microcontroller used is an `STM32F405RGT6`_. For more information have a
look at the `STM32F405xx Reference Manual`_.


The following considerations have been taken into account for the
microcontroller connections:

- Pins are 5V tolerant except when in analog mode or oscillator mode (for PC14,
  PC15, PH0 and PH1)
- PC13, PC14, PC15 and PI8 are supplied through the power switch. Since the
  switch only sinks a limited amount of current (3 mA), the use of GPIOs PC13
  to PC15 and PI8 in output mode is limited:

  - The speed should not exceed 2 MHz with a maximum load of 30 pF
  - These I/Os must not be used as a current source (e.g. to drive an LED)

======  =========  ===========  ================================  ==================================================================
Number  Name       Using        Connected to                      Functions
======  =========  ===========  ================================  ==================================================================
1       VBAT       Power        3V3                               Power
2       PC13       Input        User button                       I/O
3       PC14       Output       Serial enable                     I/O, OSC32_IN
4       PC15       Output       ToF reset                         I/O, OSC32_OUT
5       PH0        NC           ? HSE crystal                     I/O, OSC_IN
6       PH1        NC           ? HSE crystal                     I/O, OSC_OUT
7       NRST       Reset        Reset button                      Reset
8       PC0        ADC123_IN10  Receiver front right              ADC123_IN10
9       PC1        ADC123_IN11  Receiver side right               ADC123_IN11
10      PC2        ADC123_IN12  Receiver side left                SPI2_MISO, ADC123_IN12
11      PC3        ADC123_IN13  Receiver front left               SPI2_MOSI, ADC123_IN13
12      VSSA       Power        AGND                              Power
13      VDDA       Power        A3V3                              Power
14      PA0        Output       LED array                         USART2_CTS, UART4_TX, TIM2_CH1_ETR, TIM5_CH1, TIM8_ETR, ADC123_IN0
15      PA1        Output       LED array                         USART2_RTS, UART4_RX, TIM2_CH2, TIM5_CH2, ADC123_IN1
16      PA2        Output       LED array                         USART2_TX, TIM2_CH3, TIM5_CH3, TIM9_CH1, ADC123_IN2
17      PA3        Output       LED array                         USART2_RX, TIM2_CH4, TIM5_CH4, TIM9_CH2, ADC123_IN3
18      VSS        Power        GND                               Power
19      VDD        Power        3V3                               Power
20      PA4        Output       Emitter front right               SPI1_NSS, SPI3_NSS, USART2_CK, ADC12_IN4
21      PA5        Output       Emitter side right                SPI1_SCK, TIM2_CH1_ETR, TIM8_CH1N, ADC12_IN5
22      PA6        Output       Emitter side left                 SPI1_MISO, TIM1_BKIN, TIM3_CH1, TIM8_BKIN, TIM13_CH1, ADC12_IN6
23      PA7        Output       Emitter front left                SPI1_MOSI, TIM1_CH1N, TIM3_CH2, TIM8_CH1N, TIM14_CH1, ADC12_IN7
24      PC4        ADC12_IN14   Battery voltage (divided)         ADC12_IN14
25      PC5        ADC12_IN15   Motor voltage (divided)           ADC12_IN15
26      PB0        NC           ?                                 TIM1_CH2N, TIM3_CH3, TIM8_CH2N, ADC12_IN8
27      PB1        NC           ?                                 TIM1_CH3N, TIM3_CH4, TIM8_CH3N, ADC12_IN9
28      PB2/BOOT1  Input        GND                               I/O
29      PB10       I2C2_SCL     ToF sensor SCL                    SPI2_SCK, USART3_TX, I2C2_SCL, TIM2_CH3
30      PB11       I2C2_SDA     ToF sensor SDA                    USART3_RX, I2C2_SDA, TIM2_CH4
31      VCAP1      Power        To GND through a 2.2uF capacitor  Power
32      VDD        Power        3V3                               Power
33      PB12       NC           ?                                 SPI2_NSS, I2C2_SMBA, USART3_CK, TIM1_BKIN
34      PB13       NC           ?                                 SPI2_SCK, USART3_CTS, TIM1_CH1N
35      PB14       Output       Gyroscope FSYNC                   SPI2_MISO, USART3_RTS, TIM1_CH2N, TIM8_CH2N, TIM12_CH1
36      PB15       Input        Gyroscope INT                     SPI2_MOSI, TIM1_CH3N, TIM8_CH3N
37      PC6        TIM8_CH1     Motor driver AIN1 (right)         USART6_TX, TIM3_CH1, TIM8_CH1
38      PC7        TIM8_CH2     Motor driver AIN2 (right)         USART6_RX, TIM3_CH2, TIM8_CH2
39      PC8        TIM8_CH3     Motor driver BIN1 (left)          USART6_CK, TIM3_CH3, TIM8_CH3
40      PC9        TIM8_CH4     Motor driver BIN2 (left)          I2C3_SDA, TIM3_CH4, TIM8_CH4
41      PA8        USART1_CK    Serial CK                         USART1_CK, TIM1_CH1, I2C3_SCL
42      PA9        USART1_TX    Serial RX                         USART1_TX, TIM1_CH2, I2C3_SMBA
43      PA10       USART1_RX    Serial TX                         USART1_RX, TIM1_CH3
44      PA11       USART1_CTS   Serial RTS                        USART1_CTS, TIM1_CH4
45      PA12       USART1_RTS   Serial CTS                        USART1_RTS, TIM1_ETR
46      PA13       SWDIO        Programmer SWDIO pin              JTMS-SWDIO
47      VCAP2      Power        To GND through a 2.2uF capacitor  Power
48      VDD        Power        3V3                               Power
49      PA14       SWCLK        Programmer SWCLK pin              JTCK-SWCLK
50      PA15       SPI3_NSS     Gyroscope NCS (SPI-CS)            JTDI, SPI1_NSS, SPI3_NSS, TIM2_CH1_ETR
51      PC10       SPI3_SCK     Gyroscope SCL (SPI-SCLK)          USART3_TX, UART4_TX, SPI3_SCK
52      PC11       SPI3_MISO    Gyroscope AD0 (SPI-SDO)           USART3_RX, UART4_RX, SPI3_MISO
53      PC12       SPI3_MOSI    Gyroscope SDA (SPI-SDI)           USART3_CK, UART5_TX, SPI3_MOSI
54      PD2        NC           ?                                 UART5_RX, TIM3_ETR
55      PB3        NC           ?                                 JTDO, TRACESWO, SPI1_SCK, SPI3_SCK, TIM2_CH2
56      PB4        TIM3_CH1     Encoder left, channel A           NJTRST, SPI1_MISO, SPI3_MISO, TIM3_CH1
57      PB5        TIM3_CH2     Encoder left, channel B           SPI1_MOSI, SPI3_MOSI, I2C1_SMBA, TIM3_CH2
58      PB6        TIM4_CH1     Encoder right, channel B          USART1_TX, I2C1_SCL, TIM4_CH1
59      PB7        TIM4_CH2     Encoder right, channel A          USART1_RX, I2C1_SDA, TIM4_CH2
60      BOOT0      BOOT0        GND/3V3 jumper (default GND)      BOOT0
61      PB8        TIM10_CH1    Fan driver                        I2C1_SCL, TIM4_CH3, TIM10_CH1
62      PB9        TIM11_CH1    Speaker signal                    SPI2_NSS, I2C1_SDA, TIM4_CH4, TIM11_CH1
63      VSS        Power        GND                               Power
64      VDD        Power        3V3                               Power
======  =========  ===========  ================================  ==================================================================


Encoders
========

Timers 3 and 4 are configured to read quadrature encoders (and thus, connected
to the `IE2-1024 encoder`_ channels A and B). They are both configured to count
on both TI1 and TI2 edges.


Gyroscope
=========

Gyros are selected as the clock source for accuracy (versus the internal
relaxation oscillator suitable for lower power consumption when only
accelerometer is being used).

The MPU-6500 gyro features the following SPI operational features:

- Data is delivered MSB first and LSB last.
- Data is latched on the rising edge of SCLK.
- Data should be transitioned on the falling edge of SCLK.
- The maximum frequency of SCLK is 1 MHz. Although the sensor and interrupt
  registers may be read using SPI at 20 MHz.
- Read/write operations are completed in 16+ cycles (2+ bytes).
- First byte contains the SPI address, and the following the SPI data.
- First bit of the first byte contains the Read (1) / Write (0) bit.
- The following 7 bits contain the Register Address.
- In cases of multiple-byte read/writes, data is two or more bytes.


References
==========

.. target-notes::

.. _`STM32F405RGT6`:
   https://www.st.com/resource/en/datasheet/stm32f405rg.pdf
.. _`STM32F405xx reference manual`:
   https://www.st.com/resource/en/reference_manual/DM00031020.pdf
.. _`IE2-1024 encoder`:
   https://fmcc.faulhaber.com/resources/img/EN_IE2-1024_DFF.PDF

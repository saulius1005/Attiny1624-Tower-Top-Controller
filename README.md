![20250424_175854](https://github.com/user-attachments/assets/705bd36b-966e-4dae-a575-08820325abdc)

# Attiny1624 Tower Top Controller

This project is designed for the **Attiny1624** microcontroller, which communicates with an external 20 MHz clock generator and handles multiple sensor measurements, including solar cell voltage and current, as well as angular data from MT6701 sensors. The program also performs data validation via CRC checks and transmits the data over a communication interface. These data are then transmitted via fiber optics for further processing by the main solar tower control PCB.

## Features

- **Attiny1624 Microcontroller**: Main control unit.
- **External 20 MHz Clock**: Provides timing for the microcontroller. You can choose to use either an external clock generator or the internal clock for the microcontroller.
- **MT6701 Sensors**: Measures elevation and azimuth angles.
- **Voltage and Current Measurement**: Solar cell voltage and current (up to 300VDC and 12A, respectively).
- **FIR Filtering**: Applied to voltage and current measurements.
- **End Switch Monitoring**: Checks the status of Y-min and Y-max end switches.
- **Data Transmission**: Sends data over USART1 with CRC-8 checksum (CDMA2000 format).

## Hardware Setup

- **Sensors**:
  - **MT6701** sensors for angle measurements:
    - Two MT6701 sensors are used: one for measuring the **elevation angle** and the other for the **azimuth angle**. Both sensors are externally connected to the PCB.
  - **AMC1311** for voltage sensing:
    - The AMC1311 is already mounted on the PCB and used for voltage measurements.
  - **TMCS1100A4** for current sensing:
    - The TMCS1100A4 is also mounted on the PCB and is used for current sensing.

The hardware setup includes external connections for the MT6701 sensors and integrated components (AMC1311 and TMCS1100A4) on the PCB.

  
- **Communication**:
  - USART0 and USART1 are used for data transfer. Both work at 500,000 baud rate, adjustable in the `USART.c` file.
  - Data transmission is done over a 3 mm PMMA optical fiber, extending the LED lifespan and conserving energy.

## Data Format

The data is transmitted with the following format:

```
<EEEEAAAAVVVCCCYXX>
```
**Where:**

* **EEEE** – Elevation angle

* **AAAA** – Azimuth angle

* **VVV** – Voltage

* **CCC** – Current

* **Y** – End switch state

* **XX** – CRC-8 checksum

The data is sent over USART1 at 500,000 baud. The baud rate can be adjusted in the ```USART.c``` file:
```
USART1.BAUD = (uint16_t)USART1_BAUD_RATE(500000);
```
## Configuration
FIR Filtering: The level of FIR filtering applied to the voltage and current measurements can be configured in the ```FIR.h``` file:

```
#define FIR_STEPS 20
```
USART0 Baud Rate: The baud rate for USART0 communication is set to 500,000 by default, but can be modified in the ```USART.c``` file:

```
USART0.BAUD = (uint16_t)USART0_BAUD_RATE(500000);
```
## Microcontroller Pin Configuration

The microcontroller pin configuration is set up in the `GPIO_init()` function. Below is the detailed description of how the pins are configured:

1. **USART Pin Routing**:
   - `PORTMUX.USARTROUTEA = PORTMUX_USART0_DEFAULT_gc | PORTMUX_USART1_DEFAULT_gc;`  
     Configures USART0 and USART1 to default pins.

2. **Pin Settings for PORTB**:
   - `PORTB.DIRSET = PIN1_bm | PIN2_bm;`  
     Sets PB1 as XCK (MT6701 CLK) output and PB2 as TX for dummy data sending.
   - `PORTB.PIN2CTRL = PORT_PULLUPEN_bm;`  
     Enables pull-up for PB2 (USART0 TX).
   - `PORTB.DIRCLR = PIN3_bm;`  
     Sets PB3 as input (USART0 RX as MISO from MT6701 DO).
   - `PORTB.PIN3CTRL = PORT_PULLUPEN_bm;`  
     Enables pull-up for PB3 (USART0 RX).

3. **Pin Settings for PORTA**:
   - `PORTA.DIRSET = PIN1_bm | PIN6_bm | PIN7_bm;`  
     Sets PA1 as USART1 LED TX, PA6 as AZSS (MT6701 CSN), and PA7 as ELSS (MT6701 CSN).
   - `PORTA.PIN1CTRL = PORT_INVEN_bm;`  
     Inverts PA1 (USART1 TX for LED TX) to extend LED life and reduce energy consumption.
   - `PORTA.DIRCLR = PIN4_bm | PIN5_bm;`  
     Sets PA4 as input (Y MAX), and PA5 as input (Y MIN).
   - `PORTA.PIN4CTRL = PORT_PULLUPEN_bm;`  
     Enables pull-up for PA4 (Y MAX).
   - `PORTA.PIN5CTRL = PORT_PULLUPEN_bm;`  
     Enables pull-up for PA5 (Y MIN).
   - `PORTA.OUTSET = PIN6_bm | PIN7_bm;`  
     Keeps both SS (Chip Select) pins high.

4. **Disabling Digital Buffers**:
   - `PORTB.PIN0CTRL = PORT_ISC_INPUT_DISABLE_gc;`  
     Disables the digital buffer for PB0 (SC Current).
   - `PORTA.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;`  
     Disables the digital buffer for PA2 (SC Voltage).

This configuration is crucial for the communication and proper functioning of various sensors, as well as for controlling the LEDs and managing the current and voltage measurements.

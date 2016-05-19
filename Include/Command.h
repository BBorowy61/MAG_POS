#ifndef COMMAND_H
#define COMMAND_H



/////// Result units ///////////////////
#define UNIT_LB		0x11		// load
#define UNIT_KG		0x12
#define UNIT_IN		0x23		// length
#define UNIT_CM		0x24
#define UNIT_MM		0x25
#define UNIT_PS		0x37		// presure
#define UNIT_NM		0x48		// torque
#define UNIT_PC		0xf6		// %
#define UNIT_MV		0xf0		// mv per volt
#define UNIT_UN		0xff		// unspecified "--"

#define UNIT_SP		9		// number to use to write space as unit

/////// unit conversion factors ////////
#define LB_TO_KG	0.45359237	// 1lb = 0.45359237kg
#define IN_TO_MM	25.4		// 1in = 25.4mm

// the lsb of the command byte is the direction of the data part
// 0 - from ICU -> GUI
// 1 - from ICU <- GUI
// the msb of the command byte is the indicater for systemwide or sensor specific
// 0 - system wide
// 1 - sensor/channel specific

// except for some commands as noted below,
// read commands take about 4ms, write commands take about 19ms-23ms

// Board calibration time commands
#define C_BCL_START	0x01		// start board calibration

#define C_BCL_30SPS	0x03		// set sample rate to 30SPS
#define C_BCL_60SPS	0x05		// set sample rate to 60SPS
#define C_BCL_120SPS	0x07		// set sample rate to 120SPS

#define C_BCL_PGA	0x08		// set PGA
#define C_BCL_64PGA	0x09		// set PGA to 64
#define C_BCL_128PGA	0x0b		// set PGA to 128

#define C_BCL_OFFSET	0x0d		// 380ms, system offset calibration
#define C_BCL_GAIN	0x0f		// 630ms, system gain calibration and finish

// ICU address management commands
#define C_ADDR_CHECK	0x10		// address check
#define C_ADDR_SET	0x11		// set unit address
#define C_R_COMM_DEV 0x12		// get communication device
#define C_ADDR_DEL	0x13		// delete unit address

// ICU system wide configurations
#define C_R_USN		0x30		// read unit serial number
#define C_W_USN		0x31		// write unit serial number

#define C_R_UNAME	0x50		// read cell name (5 bytes)
#define C_W_UNAME	0x51		// write cell name

#define C_R_SENNUM	0x32		// read sensor number
#define C_W_SENNUM	0x33		// 24ms, write sensor number

#define C_R_ADOFFSET	0x34		// read adc offset registers
#define C_W_ADOFFSET	0x35		// write adc offset registers

#define C_R_ADGAIN	0x36		// read adc gain registers
#define C_W_ADGAIN	0x37		// write adc gain registers

#define C_R_SMPRATE	0x56		// read sample rate
#define C_W_SMPRATE	0x57		// write sample rate

#define C_R_ACLK	0x38		// read aclk registers
#define C_W_ACLK	0x39		// write aclk registers

// adcon0 is PGA, adcon2, adcon3 are decimation (for sample rate)
#define C_R_ADCON	0x3a		// read adcon(0-3) registers
#define C_W_ADCON	0x3b		// write adcon(0-3) registers

#define C_R_FS_MVV	0x3c		// read full scale mv per volt
#define C_W_FS_MVV	0x3d		// write full scale mv per volt, for restore

#define C_R_TC_MODE	0x3e		// read temperature compensation mode
#define C_W_TC_MODE	0x3f		// write temperature compensation mode

#define C_R_TC_INTV	0x40		// read temperature compensation interval
#define C_W_TC_INTV	0x41		// write temperature compensation interval

#define C_R_TEMPAVE	0x42		// read temperature reading average period
#define C_W_TEMPAVE	0x43		// write temperature reading average period

#define C_R_TC_SLOPE	0x44		// read temperature compensation slope
#define C_W_TC_SLOPE	0x45		// write temperature compensation slope

#define C_R_TC_INTCPT	0x46		// read temperature compensation intercept
#define C_W_TC_INTCPT	0x47		// write temperature compensation intercept

#define C_R_BCL_TEMP	0x48		// read board calibration temperature
#define C_W_BCL_TEMP	0x49		// write board cal temp, for system restore

#define C_R_DACMODE	0x4a		// read DAC mode
#define C_W_DACMODE	0x4b		// write DAC mode

#define C_R_DACVOLT	0x4c		// read DAC control voltage
#define C_W_DACVOLT	0x4d		// write DAC control voltage

#define C_R_RELAY	0x4e		// read relay status
#define C_W_RELAY	0x4f		// write relay switch

#define C_R_OCR_T	0x52		// read adc offset registers
#define C_W_OCR_T	0x53		// write adc offset registers

#define C_R_GCR_T	0x54		// read adc gain registers
#define C_W_GCR_T	0x55		// write adc gain registers

#define C_R_DACCAL_OFF	0x58		// DAC out calibration offset
#define C_W_DACCAL_OFF	0x59

#define C_R_DACCAL_PG	0x5a		// DAC out calibration positive gain
#define C_W_DACCAL_PG	0x5b

#define C_R_DACCAL_NG	0x5c		// DAC out calibration negative gain
#define C_W_DACCAL_NG	0x5d

#define C_R_AUTOID	0x5e		// read auto id detection setting
#define C_W_AUTOID	0x5f		// set auto id detection setting

#define C_R_TAREPST	0x62		// read persist tare (remember tare)
#define C_W_TAREPST	0x63		// set persist tare (remember tare)


// shunt switch control and status commands
#define C_R_SHUNT_SW	0x60		// read shunt switch status
#define C_W_SHUNT_SW	0x61		// 9ms, set shunt switch

// misc. commands
#define C_FMVERSION	0x70		// read firmware version number

#define C_DISPLAY	0x71		// display the float data on LCD
#define C_SET_LED	0x73		// set the three colored LED

#define C_TEMP_CAL	0x77		// temperature calibration

#define C_CHANGE_BAUD	0x79		// change baudrate

#define C_R_SYSMODE	0x7c		// read system mode, multi-channel?
#define C_W_SYSMODE	0x7d		// set system mode

#define C_RESET_ALL	0x7f		// 260ms, reset all the flash to default

// data acquisition commands
#define C_STRM_COUNT	0x80		// ad count streamming
#define C_STRM_LOAD	0x82		// load reading streamming
#define C_STRM_STOP	0x83		// stop the streamming

#define C_R_ARWEIGHT	0x86		// read the average raw load (not tc'ed)
#define C_R_ATEMP	0x88		// read the average temperature

#define C_SENDSAMPLE	0x8c		// send the sample
#define C_TAKESAMPLE	0x8d		// take a sample, broadcast cmd

// channel/sensor runtime configuration
#define C_R_BASE	0x90		// send % base
#define C_W_BASE	0x91		// write % base

#define C_R_OUNIT	0x92		// send output unit
#define C_W_OUNIT	0x93		// write output unit

#define C_R_ADEC	0x94		// send digits after the decimal point
#define C_W_ADEC	0x95		// write digits after the decimal point

#define C_R_FILTER	0x96		// send filter type
#define C_W_FILTER	0x97		// write filter type

#define C_R_DACGAINP	0x98		// read sensor positive dac gain
#define C_W_DACGAINP	0x99		// write sensor positive dac gain

#define C_R_DACGAINN	0x9a		// read sensor negative dac gain
#define C_W_DACGAINN	0x9b		// write sensor negative dac gain

#define C_R_DACOFFSET	0x9c		// read sensor dac offset
#define C_W_DACOFFSET	0x9d		// write sensor dac offset

#define C_R_LABEL	0x9e		// read sensor display label
#define C_W_LABEL	0x9f		// write sensor display label


// tare & peak & valley commands
#define C_TARE		0xb1		// tare

#define C_PV_READ	0xb2		// read peak or valley value
#define C_PV_ENABLE	0xb3		// enable peak & valley check
#define C_PV_DISABLE	0xb5		// disabled peak & valley check
#define C_PV_RESET	0xb7		// reset peak & valley


#define C_R_TPAUSE	0xbc		// read temperature pause status
#define C_W_TPAUSE	0xbd		// write temperature pause setiing


// channel/sensor calibration time 
#define C_R_CALMETHOD	0xc0		// read cal method
#define C_CAL_START	0xc1		// start calibration, set the method

#define C_CAL_CANCEL	0xd8		// 5ms, cancel calibration
#define C_CAL_FINISH	0xd9		// 275ms, finish calibration

#define C_R_SSN		0xc2		// read sensor serial number
#define C_W_SSN		0xc3		// write sensor serial number

#define C_R_CAL_DATE	0xc4		// read sensor calibration date
#define C_W_CAL_DATE	0xc5		// write sensor calibration date

#define C_R_CAL_OMAX	0xc6		// read sensor max out (rated output)
#define C_W_CAL_OMAX	0xc7		// write sensor max out (rated output)

#define C_R_CAL_UNIT	0xc8		// read calibrated unit
#define C_W_CAL_UNIT	0xc9		// write calibrated unit

#define C_R_SHUNTVAL	0xca		// read calibrated shunt value
#define C_W_SHUNTVAL	0xcb		// write calibrated shunt value

#define C_R_CAL_SLOPE	0xcc		// read calibrated slope
#define C_W_CAL_SLOPE	0xcd		// write calibrated slope

#define C_R_CAL_INTC	0xce		// read calibrated intercept
#define C_W_CAL_INTC	0xcf		// write calibrated intercept

#define C_R_CAL_ADC	0xd0		// read calibrated ad count
#define C_W_CAL_ADC	0xd1		// write calibrated ad count

#define C_R_CAL_MASS	0xd2		// read calibrated mass
#define C_W_CAL_MASS	0xd3		// write calibrated mass

#define C_R_CAL_MVV	0xd4		// read calibrated mv per volt
#define C_W_CAL_MVV	0xd5		// write calibrated mv per volt

#define C_R_CAL_TEMP	0xd6		// read temperature when calibration
#define C_W_CAL_TEMP	0xd7		// read temperature when cal, for restore

#define C_R_CAL_ADMUX	0xda		// read sensor admux, A(0x45), B(0x23), C(0x67)
#define C_W_CAL_ADMUX	0xdb		// write sensor admux

#define C_R_TAGID	0xdc		// read tagid, added v5.40
#define C_W_TAGID	0xdd		// write tagid, added v6.00

#define C_R_IGAIN	0xde		// read input OP GAIN
#define C_W_IGAIN	0xdf		// write input OP GAIN

#define C_ERROR_RESP	0xee		// error code response

// system configuration backup and restore
#define C_BACKUP	0xf1		// backup configuration
#define C_RESTORE	0xf3		// restore configuration

#define C_READ_HWPC	0x14		// read processor hardware product code and ver
#define C_READ_IRAM	0x16		// read indirect ram at address in idx byte

// debug commands
#define C_DEBUG_1	0x18		// debug commands
#define C_DEBUG_2	0x1a
#define C_DEBUG_3	0x1c
#define C_DEBUG_4	0x1e

#define E_INV_VAL	0x50		// the input value is invalid

#endif 

EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Pato: attiny2313 driver for HD44780 screens"
Date "2021-05-18"
Rev "20210522"
Comp ""
Comment1 "Dimitry Kloper"
Comment2 "dimitry.kloper@gmail.com"
Comment3 "Simplified BSD License"
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATtiny:ATtiny2313-20SU U2
U 1 1 60A40E73
P 4950 3150
F 0 "U2" H 4050 4400 50  0000 C CNN
F 1 "ATtiny2313-20SU" H 4300 4300 50  0000 C CNN
F 2 "Housings_SOIC:SOIC-20W_7.5x12.8mm_Pitch1.27mm" H 4950 3150 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2543-AVR-ATtiny2313_Datasheet.pdf" H 4950 3150 50  0001 C CNN
	1    4950 3150
	1    0    0    -1  
$EndComp
Text Label 5550 2450 0    50   ~ 0
RS
Text Label 5550 2550 0    50   ~ 0
RW
Text Label 5550 2650 0    50   ~ 0
EN
Text Label 5550 2750 0    50   ~ 0
RST
Text Label 5550 3450 0    50   ~ 0
DB4
Text Label 5550 3550 0    50   ~ 0
DB5
Text Label 5550 3650 0    50   ~ 0
DB6
Text Label 5550 3750 0    50   ~ 0
DB7
$Comp
L power:+3.3V #PWR06
U 1 1 60A41C44
P 4950 1500
F 0 "#PWR06" H 4950 1350 50  0001 C CNN
F 1 "+3.3V" H 4965 1673 50  0000 C CNN
F 2 "" H 4950 1500 50  0001 C CNN
F 3 "" H 4950 1500 50  0001 C CNN
	1    4950 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 60A41E63
P 4950 4250
F 0 "#PWR07" H 4950 4000 50  0001 C CNN
F 1 "GND" H 4955 4077 50  0000 C CNN
F 2 "" H 4950 4250 50  0001 C CNN
F 3 "" H 4950 4250 50  0001 C CNN
	1    4950 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 60A42842
P 5350 1700
F 0 "C2" H 5465 1746 50  0000 L CNN
F 1 "100nF" H 5465 1655 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 5388 1550 50  0001 C CNN
F 3 "~" H 5350 1700 50  0001 C CNN
	1    5350 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C3
U 1 1 60A4417A
P 5850 1700
F 0 "C3" H 5968 1746 50  0000 L CNN
F 1 "22uF" H 5968 1655 50  0000 L CNN
F 2 "Capacitors_Tantalum_SMD:CP_Tantalum_Case-X_EIA-7343-43_Hand" H 5888 1550 50  0001 C CNN
F 3 "~" H 5850 1700 50  0001 C CNN
	1    5850 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 1500 4950 1550
Wire Wire Line
	4950 1550 5350 1550
Wire Wire Line
	5850 1550 5350 1550
Connection ~ 5350 1550
Wire Wire Line
	4950 2050 4950 1550
Connection ~ 4950 1550
$Comp
L power:GND #PWR09
U 1 1 60A44901
P 5350 1850
F 0 "#PWR09" H 5350 1600 50  0001 C CNN
F 1 "GND" H 5355 1677 50  0000 C CNN
F 2 "" H 5350 1850 50  0001 C CNN
F 3 "" H 5350 1850 50  0001 C CNN
	1    5350 1850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 60A44AD8
P 5850 1850
F 0 "#PWR012" H 5850 1600 50  0001 C CNN
F 1 "GND" H 5855 1677 50  0000 C CNN
F 2 "" H 5850 1850 50  0001 C CNN
F 3 "" H 5850 1850 50  0001 C CNN
	1    5850 1850
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J1
U 1 1 60A44DCE
P 1950 1850
F 0 "J1" H 2000 2167 50  0000 C CNN
F 1 "DEBUG" H 2000 2076 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x03_Pitch2.54mm_SMD" H 1950 1850 50  0001 C CNN
F 3 "~" H 1950 1850 50  0001 C CNN
	1    1950 1850
	1    0    0    -1  
$EndComp
Text Label 1750 1750 2    50   ~ 0
MISO
Text Label 1750 1850 2    50   ~ 0
SCK
Text Label 1750 1950 2    50   ~ 0
RESET
Text Label 2250 1850 0    50   ~ 0
MOSI
$Comp
L power:GND #PWR04
U 1 1 60A46179
P 2300 2000
F 0 "#PWR04" H 2300 1750 50  0001 C CNN
F 1 "GND" H 2305 1827 50  0000 C CNN
F 2 "" H 2300 2000 50  0001 C CNN
F 3 "" H 2300 2000 50  0001 C CNN
	1    2300 2000
	1    0    0    -1  
$EndComp
Text Label 4350 2350 2    50   ~ 0
RESET
Text Label 5550 2950 0    50   ~ 0
MISO
Text Label 5550 2850 0    50   ~ 0
MOSI
Text Label 5550 3050 0    50   ~ 0
SCK
$Comp
L Regulator_Linear:AMS1117-3.3 U1
U 1 1 60A469FD
P 2000 3050
F 0 "U1" H 2000 3292 50  0000 C CNN
F 1 "AMS1117-3.3" H 2000 3201 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-223-3_TabPin2" H 2000 3250 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 2100 2800 50  0001 C CNN
	1    2000 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 60A470CC
P 2000 3450
F 0 "#PWR02" H 2000 3200 50  0001 C CNN
F 1 "GND" H 2005 3277 50  0000 C CNN
F 2 "" H 2000 3450 50  0001 C CNN
F 3 "" H 2000 3450 50  0001 C CNN
	1    2000 3450
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR05
U 1 1 60A474C6
P 2500 3000
F 0 "#PWR05" H 2500 2850 50  0001 C CNN
F 1 "+3.3V" H 2515 3173 50  0000 C CNN
F 2 "" H 2500 3000 50  0001 C CNN
F 3 "" H 2500 3000 50  0001 C CNN
	1    2500 3000
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C1
U 1 1 60A47B56
P 1450 3250
F 0 "C1" H 1568 3296 50  0000 L CNN
F 1 "22uF" H 1568 3205 50  0000 L CNN
F 2 "Capacitors_Tantalum_SMD:CP_Tantalum_Case-X_EIA-7343-43_Hand" H 1488 3100 50  0001 C CNN
F 3 "~" H 1450 3250 50  0001 C CNN
	1    1450 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 3050 1450 3050
Wire Wire Line
	1450 3050 1450 3100
Wire Wire Line
	2300 3050 2500 3050
Wire Wire Line
	2500 3050 2500 3000
Wire Wire Line
	1450 3400 2000 3400
Wire Wire Line
	2000 3400 2000 3350
Wire Wire Line
	2000 3450 2000 3400
Connection ~ 2000 3400
$Comp
L power:+5V #PWR01
U 1 1 60A48C8C
P 1450 3000
F 0 "#PWR01" H 1450 2850 50  0001 C CNN
F 1 "+5V" H 1465 3173 50  0000 C CNN
F 2 "" H 1450 3000 50  0001 C CNN
F 3 "" H 1450 3000 50  0001 C CNN
	1    1450 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 3050 1450 3000
Connection ~ 1450 3050
$Comp
L power:+5V #PWR03
U 1 1 60A492BD
P 2300 1700
F 0 "#PWR03" H 2300 1550 50  0001 C CNN
F 1 "+5V" H 2315 1873 50  0000 C CNN
F 2 "" H 2300 1700 50  0001 C CNN
F 3 "" H 2300 1700 50  0001 C CNN
	1    2300 1700
	1    0    0    -1  
$EndComp
NoConn ~ 5550 2350
NoConn ~ 4350 2550
NoConn ~ 4350 2750
NoConn ~ 5550 3850
NoConn ~ 5550 3350
NoConn ~ 5550 3250
$Comp
L Connector_Generic:Conn_01x16 J2
U 1 1 60A49D23
P 8150 3000
F 0 "J2" H 8230 2992 50  0000 L CNN
F 1 "Conn_01x16" H 8230 2901 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x16_Pitch2.54mm" H 8150 3000 50  0001 C CNN
F 3 "~" H 8150 3000 50  0001 C CNN
	1    8150 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 60A530EC
P 7950 2300
F 0 "#PWR016" H 7950 2050 50  0001 C CNN
F 1 "GND" V 7955 2172 50  0000 R CNN
F 2 "" H 7950 2300 50  0001 C CNN
F 3 "" H 7950 2300 50  0001 C CNN
	1    7950 2300
	0    1    1    0   
$EndComp
$Comp
L Device:R_POT RV1
U 1 1 60A53F1C
P 7300 2050
F 0 "RV1" H 7231 2096 50  0000 R CNN
F 1 "10K" H 7231 2005 50  0000 R CNN
F 2 "Potentiometers:Potentiometer_Trimmer_Bourns_3005" H 7300 2050 50  0001 C CNN
F 3 "~" H 7300 2050 50  0001 C CNN
	1    7300 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 60A548E7
P 7300 2200
F 0 "#PWR014" H 7300 1950 50  0001 C CNN
F 1 "GND" H 7305 2027 50  0000 C CNN
F 2 "" H 7300 2200 50  0001 C CNN
F 3 "" H 7300 2200 50  0001 C CNN
	1    7300 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 2050 7500 2050
Wire Wire Line
	7500 2050 7500 2500
Wire Wire Line
	7500 2500 7950 2500
Text Label 7950 2700 2    50   ~ 0
RW
Text Label 7950 2800 2    50   ~ 0
EN
Text Label 7950 2600 2    50   ~ 0
RS
$Comp
L power:GND #PWR015
U 1 1 60A5586F
P 7900 3850
F 0 "#PWR015" H 7900 3600 50  0001 C CNN
F 1 "GND" H 7905 3677 50  0000 C CNN
F 2 "" H 7900 3850 50  0001 C CNN
F 3 "" H 7900 3850 50  0001 C CNN
	1    7900 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 3850 7900 3800
Wire Wire Line
	7900 3800 7950 3800
Text Label 7950 3600 2    50   ~ 0
DB7
Text Label 7950 3500 2    50   ~ 0
DB6
Text Label 7950 3400 2    50   Italic 0
DB5
Text Label 7950 3300 2    50   Italic 0
DB4
NoConn ~ 7950 3200
NoConn ~ 7950 3100
NoConn ~ 7950 3000
NoConn ~ 7950 2900
$Comp
L power:+5VD #PWR013
U 1 1 60A57C5E
P 7300 1900
F 0 "#PWR013" H 7300 1750 50  0001 C CNN
F 1 "+5VD" H 7315 2073 50  0000 C CNN
F 2 "" H 7300 1900 50  0001 C CNN
F 3 "" H 7300 1900 50  0001 C CNN
	1    7300 1900
	1    0    0    -1  
$EndComp
$Comp
L power:+5VD #PWR017
U 1 1 60A5823D
P 7950 2400
F 0 "#PWR017" H 7950 2250 50  0001 C CNN
F 1 "+5VD" V 7965 2528 50  0000 L CNN
F 2 "" H 7950 2400 50  0001 C CNN
F 3 "" H 7950 2400 50  0001 C CNN
	1    7950 2400
	0    -1   -1   0   
$EndComp
$Comp
L power:+5VD #PWR018
U 1 1 60A58F44
P 7950 3700
F 0 "#PWR018" H 7950 3550 50  0001 C CNN
F 1 "+5VD" V 7965 3828 50  0000 L CNN
F 2 "" H 7950 3700 50  0001 C CNN
F 3 "" H 7950 3700 50  0001 C CNN
	1    7950 3700
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_FET:2N7002 Q1
U 1 1 60A59998
P 5000 5800
F 0 "Q1" H 5204 5846 50  0000 L CNN
F 1 "2N7002" H 5204 5755 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 5200 5725 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 5000 5800 50  0001 L CNN
	1    5000 5800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 60A5AA5B
P 4500 5800
F 0 "R1" V 4293 5800 50  0000 C CNN
F 1 "75" V 4384 5800 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 4430 5800 50  0001 C CNN
F 3 "~" H 4500 5800 50  0001 C CNN
	1    4500 5800
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 60A5AF82
P 4700 6100
F 0 "R2" H 4770 6146 50  0000 L CNN
F 1 "7.5K" H 4770 6055 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 4630 6100 50  0001 C CNN
F 3 "~" H 4700 6100 50  0001 C CNN
	1    4700 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 6000 5100 6250
Wire Wire Line
	5100 6250 4700 6250
Wire Wire Line
	4700 5950 4700 5800
Wire Wire Line
	4700 5800 4800 5800
Wire Wire Line
	4650 5800 4700 5800
Connection ~ 4700 5800
$Comp
L power:GND #PWR08
U 1 1 60A5C65E
P 5100 6300
F 0 "#PWR08" H 5100 6050 50  0001 C CNN
F 1 "GND" H 5105 6127 50  0000 C CNN
F 2 "" H 5100 6300 50  0001 C CNN
F 3 "" H 5100 6300 50  0001 C CNN
	1    5100 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 6300 5100 6250
Connection ~ 5100 6250
$Comp
L Transistor_FET:AO3401A Q2
U 1 1 60A5D9A8
P 5550 5450
F 0 "Q2" H 5755 5496 50  0000 L CNN
F 1 "AO3403A" H 5755 5405 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 5750 5375 50  0001 L CIN
F 3 "http://www.aosmd.com/pdfs/datasheet/AO3401A.pdf" H 5550 5450 50  0001 L CNN
	1    5550 5450
	1    0    0    1   
$EndComp
Wire Wire Line
	5350 5450 5100 5450
Wire Wire Line
	5100 5450 5100 5600
$Comp
L power:+5V #PWR010
U 1 1 60A5EF32
P 5650 5050
F 0 "#PWR010" H 5650 4900 50  0001 C CNN
F 1 "+5V" H 5665 5223 50  0000 C CNN
F 2 "" H 5650 5050 50  0001 C CNN
F 3 "" H 5650 5050 50  0001 C CNN
	1    5650 5050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 60A5F621
P 5100 5250
F 0 "R3" H 5170 5296 50  0000 L CNN
F 1 "7.5K" H 5170 5205 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 5030 5250 50  0001 C CNN
F 3 "~" H 5100 5250 50  0001 C CNN
	1    5100 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 5100 5650 5100
Wire Wire Line
	5650 5100 5650 5050
Wire Wire Line
	5650 5250 5650 5100
Connection ~ 5650 5100
Wire Wire Line
	5100 5400 5100 5450
Connection ~ 5100 5450
Wire Wire Line
	5650 5650 5650 5700
$Comp
L power:+5VD #PWR011
U 1 1 60A61691
P 5650 5750
F 0 "#PWR011" H 5650 5600 50  0001 C CNN
F 1 "+5VD" H 5665 5923 50  0000 C CNN
F 2 "" H 5650 5750 50  0001 C CNN
F 3 "" H 5650 5750 50  0001 C CNN
	1    5650 5750
	-1   0    0    1   
$EndComp
Text Label 4350 5800 2    50   Italic 0
RST
Wire Wire Line
	2250 1750 2300 1750
Wire Wire Line
	2300 1750 2300 1700
Wire Wire Line
	2250 1950 2300 1950
Wire Wire Line
	2300 1950 2300 2000
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 60A6ED47
P 2500 1750
F 0 "#FLG0101" H 2500 1825 50  0001 C CNN
F 1 "PWR_FLAG" H 2500 1923 50  0001 C CNN
F 2 "" H 2500 1750 50  0001 C CNN
F 3 "~" H 2500 1750 50  0001 C CNN
	1    2500 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 1750 2300 1750
Connection ~ 2300 1750
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 60A6F733
P 2500 1950
F 0 "#FLG0102" H 2500 2025 50  0001 C CNN
F 1 "PWR_FLAG" H 2500 2123 50  0001 C CNN
F 2 "" H 2500 1950 50  0001 C CNN
F 3 "~" H 2500 1950 50  0001 C CNN
	1    2500 1950
	-1   0    0    1   
$EndComp
Wire Wire Line
	2500 1950 2300 1950
Connection ~ 2300 1950
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 60A70B67
P 5650 5700
F 0 "#FLG0103" H 5650 5775 50  0001 C CNN
F 1 "PWR_FLAG" V 5650 5828 50  0001 L CNN
F 2 "" H 5650 5700 50  0001 C CNN
F 3 "~" H 5650 5700 50  0001 C CNN
	1    5650 5700
	0    1    1    0   
$EndComp
Connection ~ 5650 5700
Wire Wire Line
	5650 5700 5650 5750
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 60A71A03
P 1300 6300
F 0 "H2" H 1400 6303 50  0000 L CNN
F 1 "MountingHole_Pad" H 1400 6258 50  0001 L CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_Pad" H 1300 6300 50  0001 C CNN
F 3 "~" H 1300 6300 50  0001 C CNN
	1    1300 6300
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 60A721D8
P 1300 6800
F 0 "H3" H 1400 6803 50  0000 L CNN
F 1 "MountingHole_Pad" H 1400 6758 50  0001 L CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_Pad" H 1300 6800 50  0001 C CNN
F 3 "~" H 1300 6800 50  0001 C CNN
	1    1300 6800
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 60A72854
P 1300 7300
F 0 "H4" H 1400 7303 50  0000 L CNN
F 1 "MountingHole_Pad" H 1400 7258 50  0001 L CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_Pad" H 1300 7300 50  0001 C CNN
F 3 "~" H 1300 7300 50  0001 C CNN
	1    1300 7300
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H7
U 1 1 60A73763
P 2050 6850
F 0 "H7" H 2150 6850 50  0000 L CNN
F 1 "MountingHole" H 2150 6805 50  0001 L CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3" H 2050 6850 50  0001 C CNN
F 3 "~" H 2050 6850 50  0001 C CNN
	1    2050 6850
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H6
U 1 1 60A73B09
P 2050 6400
F 0 "H6" H 2150 6400 50  0000 L CNN
F 1 "MountingHole" H 2150 6355 50  0001 L CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3" H 2050 6400 50  0001 C CNN
F 3 "~" H 2050 6400 50  0001 C CNN
	1    2050 6400
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H8
U 1 1 60A73C16
P 2050 7200
F 0 "H8" H 2150 7200 50  0000 L CNN
F 1 "MountingHole" H 2150 7155 50  0001 L CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3" H 2050 7200 50  0001 C CNN
F 3 "~" H 2050 7200 50  0001 C CNN
	1    2050 7200
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H5
U 1 1 60A72D0A
P 2050 5950
F 0 "H5" H 2150 5950 50  0000 L CNN
F 1 "MountingHole" H 2150 5905 50  0001 L CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3" H 2050 5950 50  0001 C CNN
F 3 "~" H 2050 5950 50  0001 C CNN
	1    2050 5950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 60A71874
P 1300 5800
F 0 "H1" H 1400 5803 50  0000 L CNN
F 1 "MountingHole_Pad" H 1400 5758 50  0001 L CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_Pad" H 1300 5800 50  0001 C CNN
F 3 "~" H 1300 5800 50  0001 C CNN
	1    1300 5800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 60A77952
P 1300 5900
F 0 "#PWR0101" H 1300 5650 50  0001 C CNN
F 1 "GND" H 1305 5727 50  0000 C CNN
F 2 "" H 1300 5900 50  0001 C CNN
F 3 "" H 1300 5900 50  0001 C CNN
	1    1300 5900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 60A783DB
P 1300 6400
F 0 "#PWR0102" H 1300 6150 50  0001 C CNN
F 1 "GND" H 1305 6227 50  0000 C CNN
F 2 "" H 1300 6400 50  0001 C CNN
F 3 "" H 1300 6400 50  0001 C CNN
	1    1300 6400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 60A78CC5
P 1300 6900
F 0 "#PWR0103" H 1300 6650 50  0001 C CNN
F 1 "GND" H 1305 6727 50  0000 C CNN
F 2 "" H 1300 6900 50  0001 C CNN
F 3 "" H 1300 6900 50  0001 C CNN
	1    1300 6900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 60A79016
P 1300 7400
F 0 "#PWR0104" H 1300 7150 50  0001 C CNN
F 1 "GND" H 1305 7227 50  0000 C CNN
F 2 "" H 1300 7400 50  0001 C CNN
F 3 "" H 1300 7400 50  0001 C CNN
	1    1300 7400
	1    0    0    -1  
$EndComp
Text Notes 2600 1850 0    50   ~ 0
SDA
Text Notes 1300 1850 0    50   ~ 0
SCK
$EndSCHEMATC

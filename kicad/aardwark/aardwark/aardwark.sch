EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Arrdwark -> Pato Adapter"
Date "2021-05-26"
Rev "20210526"
Comp ""
Comment1 "Dimitry Kloper"
Comment2 "dimitry.kloper@gmail.com"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J1
U 1 1 60AE6ED2
P 2900 2100
F 0 "J1" H 2950 2517 50  0000 C CNN
F 1 "AARDWARK" H 2950 2426 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x05_Pitch2.54mm" H 2900 2100 50  0001 C CNN
F 3 "~" H 2900 2100 50  0001 C CNN
	1    2900 2100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x06 J2
U 1 1 60AE78CD
P 4800 2000
F 0 "J2" H 4880 1992 50  0000 L CNN
F 1 "TEST" H 4880 1901 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 4800 2000 50  0001 C CNN
F 3 "~" H 4800 2000 50  0001 C CNN
	1    4800 2000
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J3
U 1 1 60AE82D8
P 6700 2100
F 0 "J3" H 6750 2417 50  0000 C CNN
F 1 "PATO" H 6750 2326 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x03_Pitch2.54mm" H 6700 2100 50  0001 C CNN
F 3 "~" H 6700 2100 50  0001 C CNN
	1    6700 2100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J4
U 1 1 60AE8966
P 8200 2100
F 0 "J4" H 8250 2417 50  0000 C CNN
F 1 "ICE" H 8250 2326 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x03_Pitch2.54mm" H 8200 2100 50  0001 C CNN
F 3 "~" H 8200 2100 50  0001 C CNN
	1    8200 2100
	1    0    0    -1  
$EndComp
Text Label 6500 2000 2    50   ~ 0
MISO
Text Label 6500 2100 2    50   ~ 0
SCK
Text Label 6500 2200 2    50   ~ 0
RESET
$Comp
L power:+5V #PWR07
U 1 1 60AE955B
P 7000 2000
F 0 "#PWR07" H 7000 1850 50  0001 C CNN
F 1 "+5V" H 7015 2173 50  0000 C CNN
F 2 "" H 7000 2000 50  0001 C CNN
F 3 "" H 7000 2000 50  0001 C CNN
	1    7000 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 60AE9CBA
P 7000 2200
F 0 "#PWR08" H 7000 1950 50  0001 C CNN
F 1 "GND" H 7005 2027 50  0000 C CNN
F 2 "" H 7000 2200 50  0001 C CNN
F 3 "" H 7000 2200 50  0001 C CNN
	1    7000 2200
	1    0    0    -1  
$EndComp
Text Label 7000 2100 0    50   ~ 0
MOSI
Text Label 8000 2000 2    50   ~ 0
MISO
Text Label 8000 2100 2    50   ~ 0
SCK
Text Label 8000 2200 2    50   ~ 0
RESET
Text Label 8500 2100 0    50   ~ 0
MOSI
$Comp
L power:+5V #PWR09
U 1 1 60AEA9D0
P 8500 2000
F 0 "#PWR09" H 8500 1850 50  0001 C CNN
F 1 "+5V" H 8515 2173 50  0000 C CNN
F 2 "" H 8500 2000 50  0001 C CNN
F 3 "" H 8500 2000 50  0001 C CNN
	1    8500 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 60AEAFB0
P 8500 2200
F 0 "#PWR010" H 8500 1950 50  0001 C CNN
F 1 "GND" H 8505 2027 50  0000 C CNN
F 2 "" H 8500 2200 50  0001 C CNN
F 3 "" H 8500 2200 50  0001 C CNN
	1    8500 2200
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J5
U 1 1 60AEB4A5
P 9400 2000
F 0 "J5" H 9480 1992 50  0000 L CNN
F 1 "POWER" H 9480 1901 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 9400 2000 50  0001 C CNN
F 3 "~" H 9400 2000 50  0001 C CNN
	1    9400 2000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR011
U 1 1 60AEC06F
P 9200 2000
F 0 "#PWR011" H 9200 1850 50  0001 C CNN
F 1 "+5V" H 9215 2173 50  0000 C CNN
F 2 "" H 9200 2000 50  0001 C CNN
F 3 "" H 9200 2000 50  0001 C CNN
	1    9200 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 60AEC2A0
P 9200 2100
F 0 "#PWR012" H 9200 1850 50  0001 C CNN
F 1 "GND" H 9205 1927 50  0000 C CNN
F 2 "" H 9200 2100 50  0001 C CNN
F 3 "" H 9200 2100 50  0001 C CNN
	1    9200 2100
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR05
U 1 1 60AEC78E
P 4600 1800
F 0 "#PWR05" H 4600 1650 50  0001 C CNN
F 1 "+5V" H 4615 1973 50  0000 C CNN
F 2 "" H 4600 1800 50  0001 C CNN
F 3 "" H 4600 1800 50  0001 C CNN
	1    4600 1800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 60AECAA6
P 4600 2300
F 0 "#PWR06" H 4600 2050 50  0001 C CNN
F 1 "GND" H 4605 2127 50  0000 C CNN
F 2 "" H 4600 2300 50  0001 C CNN
F 3 "" H 4600 2300 50  0001 C CNN
	1    4600 2300
	1    0    0    -1  
$EndComp
Text Label 4600 1900 2    50   ~ 0
MISO
Text Label 4600 2000 2    50   ~ 0
MOSI
Text Label 4600 2100 2    50   ~ 0
SCK
Text Label 4600 2200 2    50   ~ 0
RESET
Text Label 2700 1900 2    50   ~ 0
SCL
Text Label 2700 2000 2    50   ~ 0
SDA
$Comp
L power:GND #PWR01
U 1 1 60AED4C2
P 3200 1900
F 0 "#PWR01" H 3200 1650 50  0001 C CNN
F 1 "GND" V 3205 1772 50  0000 R CNN
F 2 "" H 3200 1900 50  0001 C CNN
F 3 "" H 3200 1900 50  0001 C CNN
	1    3200 1900
	0    -1   -1   0   
$EndComp
NoConn ~ 2700 2100
NoConn ~ 2700 2200
NoConn ~ 3200 2200
NoConn ~ 2700 2300
$Comp
L power:GND #PWR04
U 1 1 60AED9C5
P 3200 2300
F 0 "#PWR04" H 3200 2050 50  0001 C CNN
F 1 "GND" V 3205 2172 50  0000 R CNN
F 2 "" H 3200 2300 50  0001 C CNN
F 3 "" H 3200 2300 50  0001 C CNN
	1    3200 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2900 1400 3200 1400
Text Label 2900 1400 2    50   ~ 0
SCL
Text Label 3200 1500 0    50   ~ 0
MOSI
Wire Wire Line
	2900 1500 3200 1500
Text Label 2900 1500 2    50   ~ 0
SDA
Text Label 3200 1400 0    50   ~ 0
SCK
$Comp
L Connector_Generic:Conn_01x02 J6
U 1 1 60AEE573
P 10200 2000
F 0 "J6" H 10280 1992 50  0000 L CNN
F 1 "PW_JMP" H 10280 1901 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 10200 2000 50  0001 C CNN
F 3 "~" H 10200 2000 50  0001 C CNN
	1    10200 2000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR013
U 1 1 60AEEAD4
P 10000 2000
F 0 "#PWR013" H 10000 1850 50  0001 C CNN
F 1 "+5V" H 10015 2173 50  0000 C CNN
F 2 "" H 10000 2000 50  0001 C CNN
F 3 "" H 10000 2000 50  0001 C CNN
	1    10000 2000
	1    0    0    -1  
$EndComp
$Comp
L power:+5VD #PWR014
U 1 1 60AEF332
P 10000 2100
F 0 "#PWR014" H 10000 1950 50  0001 C CNN
F 1 "+5VD" H 10015 2273 50  0000 C CNN
F 2 "" H 10000 2100 50  0001 C CNN
F 3 "" H 10000 2100 50  0001 C CNN
	1    10000 2100
	-1   0    0    1   
$EndComp
$Comp
L power:+5VD #PWR02
U 1 1 60AEFA79
P 3200 2000
F 0 "#PWR02" H 3200 1850 50  0001 C CNN
F 1 "+5VD" V 3215 2128 50  0000 L CNN
F 2 "" H 3200 2000 50  0001 C CNN
F 3 "" H 3200 2000 50  0001 C CNN
	1    3200 2000
	0    1    1    0   
$EndComp
$Comp
L power:+5VD #PWR03
U 1 1 60AF01BC
P 3200 2100
F 0 "#PWR03" H 3200 1950 50  0001 C CNN
F 1 "+5VD" V 3215 2228 50  0000 L CNN
F 2 "" H 3200 2100 50  0001 C CNN
F 3 "" H 3200 2100 50  0001 C CNN
	1    3200 2100
	0    1    1    0   
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 60AF0A29
P 10000 2100
F 0 "#FLG0101" H 10000 2175 50  0001 C CNN
F 1 "PWR_FLAG" V 10000 2227 50  0001 L CNN
F 2 "" H 10000 2100 50  0001 C CNN
F 3 "~" H 10000 2100 50  0001 C CNN
	1    10000 2100
	0    -1   -1   0   
$EndComp
Connection ~ 10000 2100
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 60AF1041
P 10000 2000
F 0 "#FLG0102" H 10000 2075 50  0001 C CNN
F 1 "PWR_FLAG" V 10000 2127 50  0001 L CNN
F 2 "" H 10000 2000 50  0001 C CNN
F 3 "~" H 10000 2000 50  0001 C CNN
	1    10000 2000
	0    -1   -1   0   
$EndComp
Connection ~ 10000 2000
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 60AF1212
P 9200 2100
F 0 "#FLG0103" H 9200 2175 50  0001 C CNN
F 1 "PWR_FLAG" V 9200 2227 50  0001 L CNN
F 2 "" H 9200 2100 50  0001 C CNN
F 3 "~" H 9200 2100 50  0001 C CNN
	1    9200 2100
	0    -1   -1   0   
$EndComp
Connection ~ 9200 2100
$EndSCHEMATC

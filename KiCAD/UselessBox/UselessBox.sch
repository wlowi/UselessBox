EESchema Schematic File Version 4
LIBS:UselessBox-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 5FD0EDD7
P 3950 3500
F 0 "A1" H 4600 2350 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 4600 2450 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 4100 2550 50  0001 L CNN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 3950 2500 50  0001 C CNN
	1    3950 3500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 5FD0EFDC
P 2650 1250
F 0 "J1" V 2750 1350 50  0000 C CNN
F 1 "Power IN" V 2450 1200 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2650 1250 50  0001 C CNN
F 3 "~" H 2650 1250 50  0001 C CNN
	1    2650 1250
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x03_Male J4
U 1 1 5FD12A39
P 4900 1250
F 0 "J4" V 5000 1500 50  0000 R CNN
F 1 "Servo Door" V 4700 1450 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 4900 1250 50  0001 C CNN
F 3 "~" H 4900 1250 50  0001 C CNN
	1    4900 1250
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x03_Male J5
U 1 1 5FD12AB7
P 5850 1250
F 0 "J5" V 5950 1500 50  0000 R CNN
F 1 "Servo Finger" V 5650 1450 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 5850 1250 50  0001 C CNN
F 3 "~" H 5850 1250 50  0001 C CNN
	1    5850 1250
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x05_Male J6
U 1 1 5FD12BE6
P 7050 1250
F 0 "J6" V 7150 1600 50  0000 R CNN
F 1 "Calibration Switches" V 6850 1600 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 7050 1250 50  0001 C CNN
F 3 "~" H 7050 1250 50  0001 C CNN
	1    7050 1250
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x04_Male J2
U 1 1 5FD137ED
P 3200 1250
F 0 "J2" V 3300 1500 50  0000 R CNN
F 1 "Switch (2 x On)" V 3000 1550 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 3200 1250 50  0001 C CNN
F 3 "~" H 3200 1250 50  0001 C CNN
	1    3200 1250
	0    1    1    0   
$EndComp
$Comp
L Device:CP C1
U 1 1 5FD13A7C
P 5450 2500
F 0 "C1" H 5550 2550 50  0000 L CNN
F 1 "CP" H 5568 2455 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D12.5mm_P5.00mm" H 5488 2350 50  0001 C CNN
F 3 "~" H 5450 2500 50  0001 C CNN
	1    5450 2500
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC107 Q1
U 1 1 5FD13B4E
P 2200 4000
F 0 "Q1" H 2391 4046 50  0000 L CNN
F 1 "BC107" H 2391 3955 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-18-3" H 2400 3925 50  0001 L CIN
F 3 "http://www.b-kainka.de/Daten/Transistor/BC108.pdf" H 2200 4000 50  0001 L CNN
	1    2200 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5FD13C5B
P 5100 3250
F 0 "R1" H 5170 3296 50  0000 L CNN
F 1 "R" H 5170 3205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5030 3250 50  0001 C CNN
F 3 "~" H 5100 3250 50  0001 C CNN
	1    5100 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5FD13D59
P 5100 3750
F 0 "R2" H 5170 3796 50  0000 L CNN
F 1 "R" H 5170 3705 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5030 3750 50  0001 C CNN
F 3 "~" H 5100 3750 50  0001 C CNN
	1    5100 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5FD13E12
P 1950 4250
F 0 "R3" H 2020 4296 50  0000 L CNN
F 1 "R" H 2020 4205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 1880 4250 50  0001 C CNN
F 3 "~" H 1950 4250 50  0001 C CNN
	1    1950 4250
	1    0    0    -1  
$EndComp
$Comp
L Relay:G5V-2 K1
U 1 1 5FD14588
P 2550 2750
F 0 "K1" V 3350 2750 50  0000 C CNN
F 1 "G5V-2" V 3250 2750 50  0000 C CNN
F 2 "Relay_THT:Relay_DPDT_Omron_G5V-2" H 3200 2700 50  0001 L CNN
F 3 "http://omronfs.omron.com/en_US/ecb/products/pdf/en-g5v_2.pdf" H 2350 2750 50  0001 C CNN
	1    2550 2750
	0    1    1    0   
$EndComp
$Comp
L Diode:1N4148 D1
U 1 1 5FD1469A
P 2550 2000
F 0 "D1" H 2550 2216 50  0000 C CNN
F 1 "1N4148" H 2550 2125 50  0000 C CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 2550 1825 50  0001 C CNN
F 3 "http://www.nxp.com/documents/data_sheet/1N4148_1N4448.pdf" H 2550 2000 50  0001 C CNN
	1    2550 2000
	-1   0    0    -1  
$EndComp
Text Label 2650 1200 0    50   ~ 0
+Accu
Text Label 2400 1200 0    50   ~ 0
GND
Wire Wire Line
	4050 4700 4050 4500
Wire Wire Line
	3200 1950 3850 1950
Wire Wire Line
	3850 1950 3850 2500
Wire Wire Line
	2550 1550 1750 1550
Wire Wire Line
	1750 1550 1750 4700
Wire Wire Line
	1750 4700 2300 4700
Wire Wire Line
	2650 1700 1950 1700
Wire Wire Line
	2850 2850 3200 2850
Wire Wire Line
	3200 2850 3200 1950
Wire Wire Line
	1950 2750 2250 2750
Wire Wire Line
	1950 1700 1950 2750
Wire Wire Line
	2850 2350 2900 2350
Wire Wire Line
	4150 2350 4150 2500
Wire Wire Line
	2300 3800 2100 3800
Wire Wire Line
	2100 3800 2100 2350
Wire Wire Line
	2100 2350 2200 2350
Wire Wire Line
	2300 4200 2300 4700
Connection ~ 2300 4700
Wire Wire Line
	3850 1950 5100 1950
Wire Wire Line
	5100 1950 5100 3100
Connection ~ 3850 1950
Wire Wire Line
	5100 3400 5100 3500
Wire Wire Line
	5100 3900 5100 4700
Wire Wire Line
	5100 4700 4050 4700
Connection ~ 4050 4700
Wire Wire Line
	5100 3500 4450 3500
Connection ~ 5100 3500
Wire Wire Line
	5100 3500 5100 3600
Wire Wire Line
	4150 2350 4900 2350
Wire Wire Line
	4900 2350 4900 1650
Connection ~ 4150 2350
Wire Wire Line
	4900 1650 5850 1650
Wire Wire Line
	5850 1650 5850 1450
Connection ~ 4900 1650
Wire Wire Line
	4900 1650 4900 1450
Wire Wire Line
	5000 1450 5000 1550
Wire Wire Line
	5950 1550 5950 1450
Wire Wire Line
	5100 4700 5600 4700
Wire Wire Line
	5950 4700 5950 2650
Connection ~ 5100 4700
Connection ~ 5950 1550
Wire Wire Line
	2900 2350 2900 2000
Wire Wire Line
	2900 2000 2700 2000
Connection ~ 2900 2350
Wire Wire Line
	2900 2350 4150 2350
Wire Wire Line
	2400 2000 2200 2000
Wire Wire Line
	2200 2000 2200 2350
Connection ~ 2200 2350
Wire Wire Line
	2200 2350 2250 2350
Connection ~ 5000 1550
Wire Wire Line
	4900 2350 5450 2350
Connection ~ 4900 2350
Wire Wire Line
	5450 2650 5950 2650
Connection ~ 5950 2650
Wire Wire Line
	4800 1650 4800 1450
Wire Wire Line
	3350 3800 3450 3800
Wire Wire Line
	3350 1650 3350 3800
Wire Wire Line
	3450 3900 3300 3900
Wire Wire Line
	3300 3900 3300 1600
Wire Wire Line
	3300 1600 5750 1600
Wire Wire Line
	5750 1600 5750 1450
Wire Wire Line
	3450 3700 3400 3700
Wire Wire Line
	3400 3700 3400 1550
Wire Wire Line
	5950 1550 6850 1550
Wire Wire Line
	6850 1550 6850 1450
Wire Wire Line
	3450 3600 3250 3600
Wire Wire Line
	3250 3600 3250 4800
Wire Wire Line
	3250 4800 6100 4800
Wire Wire Line
	6100 4800 6100 1650
Wire Wire Line
	7050 1650 7050 1450
Wire Wire Line
	7150 1700 6150 1700
Wire Wire Line
	6150 1700 6150 4850
Wire Wire Line
	6150 4850 3200 4850
Wire Wire Line
	3200 4850 3200 3500
Wire Wire Line
	3200 3500 3450 3500
Wire Wire Line
	3450 3400 3150 3400
Wire Wire Line
	3150 3400 3150 4900
Wire Wire Line
	3150 4900 6200 4900
Wire Wire Line
	6200 4900 6200 1750
Wire Wire Line
	6200 1750 7250 1750
Wire Wire Line
	2000 4000 1950 4000
Wire Wire Line
	1950 4000 1950 4100
Wire Wire Line
	1950 4400 1950 4500
Wire Wire Line
	1950 4500 3050 4500
Wire Wire Line
	3050 4500 3050 3300
Wire Wire Line
	3050 3300 3450 3300
NoConn ~ 2250 3150
NoConn ~ 2850 3250
NoConn ~ 2850 3050
NoConn ~ 2850 2650
Wire Wire Line
	2550 1550 2550 1450
Wire Wire Line
	5000 1550 5950 1550
Wire Wire Line
	5950 1550 5950 2650
Wire Wire Line
	3350 1650 4800 1650
Wire Wire Line
	6100 1650 7050 1650
Wire Wire Line
	7150 1450 7150 1700
Wire Wire Line
	7250 1450 7250 1750
NoConn ~ 6950 1450
NoConn ~ 3950 4500
NoConn ~ 4050 2500
NoConn ~ 4450 2900
NoConn ~ 4450 3000
NoConn ~ 4450 3300
NoConn ~ 4450 3600
NoConn ~ 4450 3700
NoConn ~ 4450 3800
NoConn ~ 4450 3900
NoConn ~ 4450 4000
NoConn ~ 4450 4100
NoConn ~ 4450 4200
NoConn ~ 3450 4200
NoConn ~ 3450 4100
NoConn ~ 3450 4000
NoConn ~ 3450 2900
NoConn ~ 3450 3000
NoConn ~ 3450 3100
NoConn ~ 3450 3200
Wire Wire Line
	2300 4700 4050 4700
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5FD206B6
P 3850 1950
F 0 "#FLG0101" H 3850 2025 50  0001 C CNN
F 1 "PWR_FLAG" H 3850 2124 50  0000 C CNN
F 2 "" H 3850 1950 50  0001 C CNN
F 3 "~" H 3850 1950 50  0001 C CNN
	1    3850 1950
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5FD20702
P 5600 4700
F 0 "#FLG0102" H 5600 4775 50  0001 C CNN
F 1 "PWR_FLAG" H 5600 4874 50  0000 C CNN
F 2 "" H 5600 4700 50  0001 C CNN
F 3 "~" H 5600 4700 50  0001 C CNN
	1    5600 4700
	1    0    0    -1  
$EndComp
Connection ~ 5600 4700
Wire Wire Line
	5600 4700 5950 4700
Wire Wire Line
	2650 1450 2650 1550
Wire Wire Line
	2650 1550 3000 1550
Wire Wire Line
	3000 1550 3000 1450
Connection ~ 2650 1550
Wire Wire Line
	2650 1550 2650 1700
Wire Wire Line
	3200 1950 3100 1950
Wire Wire Line
	3100 1950 3100 1450
Connection ~ 3200 1950
Wire Wire Line
	3400 1550 3200 1550
Wire Wire Line
	3200 1550 3200 1450
Wire Wire Line
	3450 1550 3450 1500
Wire Wire Line
	3450 1500 3300 1500
Wire Wire Line
	3300 1500 3300 1450
Wire Wire Line
	3450 1550 5000 1550
$EndSCHEMATC

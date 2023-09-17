Additional development in LabVIEW automation:
------------------------------------------------------------------------------------------------
1. BF_INT_FirmWareUpgrade_Auto: 

This VI is capable of upgrading ultrasonic welding machine but only one time. 
But automatic system restart not implemented here. 

Pending task: LabVIEW script need to be developed to 
restart the system automatically to enable multiple firmare upgrade machanism

*************************************************************************************************

2. Read_WeldRecipeParams_Local_ini.vi:

This vi is capable of reading all the weld recipe parameters and make structure array
nothing but TCP packet to send to BL. All SC,AC and PC parameteres are read from the
INI file. User can able to modify any parameters according to the requirement.

Pending task:
* Palm button simulation
* Sending Recipe parameters to BL
* Reporting

*************************************************************************************************
3. Weldautomation_20KHZ_Test.vi:
This VI is capable of displaying the current status of weld automation testing, can read weld
recipe parameters and make packet to send to BL for welding and actuator setup.

Pending:
* Palm button simulation
* Estop simulation
* Auto power supply restart
* Weld results reporting


*************************************************************************************************

4. Control_DAQ_USB6008.vi : 

This VI is capable of controlling USB6008 device configured to all the digital IO pins
P0.0,P0.1,P0.2,P0.3,P0.4,P0.5,P0.6,P0.7

LabVIEW is capable to send 0v or 5v at these ports

Nothing is pending here.

*************************************************************************************************

5. Run_All_LabVIEW_test_scripts.vi:

This VI is capable of running all the LabVIEW exe's, this vi can also be called as offline framework.
It can able to execute all the exes, store all results end of execution in \Results_Next_Gen and 
consolidate the report to user how many pass and how many fail. 

Nothing is pending here.

*************************************************************************************************
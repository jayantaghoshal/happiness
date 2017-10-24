APIX driver gate test.

Test requires two components in order to pass: 

1. Android IHU with APIX driver and correct APIX configuration parameters in order to establish
   an APIX link. The GTest parts of the gate tests will run on the IHU.

2. PC host connected to INOVA INAP375R + INAP 375T/R Ethernet Board with correct configuration
   parameters. The PC host shall run the APIX_IP_Host_Server.py script.

Troubleshooting and detailed setup for APIX:
https://c1.confluence.cm.volvocars.biz/display/IHUA/APIX



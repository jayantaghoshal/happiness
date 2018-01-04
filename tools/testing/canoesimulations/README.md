This project contains scripts and Infotainment specific code used for creating CANoe simulations using the the Model Generation Wizard (MGW) tool delivered by vector in SPA_Multibus_Package version 3 and later.

Find installers and Vector documentation in our Artifactory here:
https://lts.artifactory.cm.volvocars.biz/artifactory/INFOTAINMENTAOV/Externals/Vector

Databases used for creation of configurations can be found here:
https://sharepoint.volvocars.net/sites/testforum/SDBCorrected/Forms/AllItems.aspx

Additional databases e.g. LIN, MOST, and Ethernet should be found here:
\\\Gotsnm5104\9413-APP-NASELSDB\shr-sbd\Signal_Management
These buses need to be added manually as well, including LIN nodes.


Script details:

remove_UB.py 

is used to correct the generated GW files, e.g. Output\CEM_GW.cin
by removing gateway handling of update bits. (Update bits should be handled by node layer dlls)

ASNV_CONF_make.py

is used to create database files for VCC IP command bus protocol.

CAPL:

The CAPL files can be used to replace the "placeholder" files created in \\Nodes folder.
This is wehere we should add Infotainment specific functionality, and changes we want to make for every configuration.
e.g. initializing signal values.

Car configuration files:

These files need to be created first, and they can be created using MGW.
Use the ModelGenWizard.exe to generate “CC: Default” and point out your *.ini/.tab/.profile. 
Preferred is to use *.ini file and then .vsysvar files and panels are created with as much information as possible e.g. value tables.
The .ini file could be generated from http://ccdb-prod.volvocars.biz and access to this page is provided from Andreasson, Stefan (94142) <stefan.andreasson@volvocars.com>. 
Then the InitHash functions in the files SPACommon_CarConfigRead.cin and SPACommon_CarConfig.cin needs to be updated manually.
However these files are slightly different, neew namespace and such. We need an update to this module, manual work takes too long tim and it is very easy to do it wrong.






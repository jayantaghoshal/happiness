
ASNParser.exe --field-sorting -c -v ..\..\libraries\libasn1\spec\infotainmentIpBus.asn
@rem Next step is to replace Infotainment_IP_bus_ tih Icb_, and sed would suite perfect for this task
@rem However sed changes line terminators so this has been done manually in an editor like Emacs 
@rem "C:\Program Files\Git\usr\bin\sed" -i -e 's/Infotainment_IP_bus_/Icb_/g' ..\..\libraries\libasn1\spec\infotainmentIpBus.h
@rem "C:\Program Files\Git\usr\bin\sed" -i -e 's/Infotainment_IP_bus_/Icb_/g' ..\..\libraries\libasn1\spec\infotainmentIpBus.c
pause
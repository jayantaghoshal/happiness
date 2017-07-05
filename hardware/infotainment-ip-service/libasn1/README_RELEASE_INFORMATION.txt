VDR-NOTE-SWRS-31841985-02-17 Infotainment IP Command Bus_IP23_SPA

2014-10-03
=====================


The following is a release fix minor bugs of the previous IP18 release.
This release will be called: "IP23 Release".

-eTracker: 10240964
 Chapter 3.5.7.1.10, Operation ID = WLANSTAStatus(0x040D) contains a wrong parameter range for 'signalStrength'
 Corrected range to  Integer(-100..0)
   

-eTracker: 10241655
Chapter 3.5.6.1.5 PSIM Connection Status (0x0306).
Added VCM to client list, since it needs to get the notifications for PSIMConnectionStatus (0x0306) from TEM.


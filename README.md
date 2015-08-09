OSC-Sync-Master
===============

The OSC synchronisation master maintains synchronised clocks between multiple nodes connected on an Ethernet/Wi-Fi network.  The hardware is a [Microchip PIC32 Ethernet Starter Kit](http://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=DM320004).  The code is derived from the TCPIP/Demo App in [microchip-libraries-for-applications-v2013-06-15](http://www.microchip.com/pagehandler/en-us/devtools/mla/).  The IDE ([MPLABX](http://www.microchip.com/pagehandler/en-us/family/mplabx/)) and compiler ([XC32](http://www.microchip.com/pagehandler/en_us/devtools/mplabxc)) are free and the hardware incorporates an on-board USB programmer/debugger.

<img src="https://raw.githubusercontent.com/xioTechnologies/OSC-Sync-Master/master/PIC32%20Ethernet%20Starter%20Kit.jpg"/>

The OSC synchronisation master periodically broadcasts an OSC message ("/sync") with an OSC time tag argument.  The OSC time tag is obtained from an on-board clock with a precision of 12.5 ns.  The master also sends an OSC message ("/external") in a timestamped OSC bundle each time an input pin changes state.  This allows an external clock signal (e.g. 1 Hz) to be used to evaluate the synchronisation error between the master and slaves.

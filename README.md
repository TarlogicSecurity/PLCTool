# PLCTool - A PRIME/DLMS graphical swiss-knife

This repostory contains the source code of **PLCTool**, a Qt-based graphical tool used to analyze and debug PRIME / DLMS-based smart-meter networks, with special focus on the security of these deployments.

Currently, **we support GNU/Linux and probably other Unix-based environments.** Windows support is on its way.

This tool was presented in the talk **Hacking Smart Meters** of the RootedCON 2022, during which its use with the **ATPL360-EK** evaluation kit to send and receive data in PLC networks was demonstrated.

Leave it running during a full 24-hour cycle and click on the Credential Logger button. If the PRIME you are in is not particularly secure, you may be able to see a few low-level-security DLMS passwords in the credential logger.


## Build

Build instructions and a usage guide here: [**PLCTool, the Swiss army knife of smart meters**](https://www.tarlogic.com/blog/plctool-swiss-army-knife-of-smart-meters/)

Note that in order to build PLCTool, you will need:

- A suitable C++ compiler (gcc 10.3 or higher)
- Qt 5 development files, version 5.9.1 or higher

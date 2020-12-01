# PLCTool - A PRIME/DLMS graphical swiss-knife
This repostory contains the source code of **PLCTool**, a Qt-based graphical tool used to analyze and debug PRIME / DLMS-based smart-meter networks, with special focus on the security of these deployments.

Currently, **we only support GNU/Linux and probably other Unix-based environments.** Windows support is on its way.

## Build
PLCTool is a small application that can be built with minimal requirements. In order to build PLCTool, you will need:

- A suitable C++ compiler (gcc 8.3.1 or higher)
- Qt 5 development files, version 5.9.1 or higher

And the build procedure is fairly simple, just download the sources (for instance, in a folder named PLCTool) and run:
```
% cd PLCTool
% qmake PLCTool.pro
% make
```

And if the build was successful, you can simply run it with:
```
% ./PLCTool
```

## Using it with ATPL360-EK
In order to communicate with the ATPL360-EK, you will need to build and install the [Candleblow](https://github.com/TarlogicSecurity/Candleblow) firmware in it with **ATMEL Studio 7.0** and a cheap 20-pin ARM JTAG programmer [like this](https://www.amazon.es/Gen%C3%A9rico-depurador-soporte-Ortex-M3-reemplazo/dp/B086RJZXGJ/ref=sr_1_2?__mk_es_ES=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=Jtag+Usb+arm&qid=1606854930&sr=8-2).  All these steps can be performed directly from ATMEL Studio 7.0.

Once Candleblow is properly installed, plug it to your nearest AC adapter and the USB UART0 to your computer with the provided USB cable. No drivers will be needed here, Linux will detect the board as a USB-based serial port and expose it in `/dev`. The first step of course is to locate the device name. It may take names like `/dev/ttyACM0` or `/dev/ttyUSB0` depending on your particular GNU/Linux distribution and the number of existing serial devices connected to your computer:

![](https://raw.githubusercontent.com/TarlogicSecurity/PLCTool/main/doc/plctool1.png)

Remember this device name and make sure you have both read and write access to it (if you don't, you may change this the quick and dirty way with `sudo chmod a+rw`). Now open PLCTool and click on connect. You should see the following dialog:

![](https://raw.githubusercontent.com/TarlogicSecurity/PLCTool/main/doc/plctool2.png)

Enter the full device path and press Connect. After a few seconds (or even less, depending on the number of surrounding meters around you) you should be able to capture a few (probably unencrypted) PRIME frames.

![](https://raw.githubusercontent.com/TarlogicSecurity/PLCTool/main/doc/plctool3.png)

Leave it running during a full 24-hour cycle and click on the Credential Logger button. If the PRIME you are in is not particularly secure, you may be able to see a few low-level-security DLMS passwords in the credential logger.

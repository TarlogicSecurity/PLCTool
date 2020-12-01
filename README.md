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
In order to communicate with the ATPL360-EK, you will need to build and install the [Candleblow](https://github.com/TarlogicSecurity/Candleblow) firmware in it with **ATMEL Studio 7.0** and a cheap 20-pin ARM JTAG programmer [like this](https://www.amazon.es/Gen%C3%A9rico-depurador-soporte-Ortex-M3-reemplazo/dp/B086RJZXGJ/ref=sr_1_2?__mk_es_ES=%C3%85M%C3%85%C5%BD%C3%95%C3%91&dchild=1&keywords=Jtag+Usb+arm&qid=1606854930&sr=8-2).  All these steps can be performed directly from ATMEL Studio 7.0

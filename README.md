Arduino interface to MCS-12085/12086 Optical Mouse Sensor

Connect SDIO/SCLK to pins defined in mcs-12085.cpp and then call
mcs12085_dx() and mcs12085_dy() to read distance mouse has moved since
last call.
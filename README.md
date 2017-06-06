# ThingSpeak Data Retriever
Library for download daily and monthly data from a ThingSpeak channel.

It's a very simple library: it retrieves 1 value per hour in daily mode; if there are more values in a hour, it retrieve the last ; for monthly data, it retrieves minimum and maximum value for every day.
Library can be easily modified to get more data.
 
This library is using Json Streaming Parser library by Daniel Eichhorn

This library has been tested on ESP8266 target platform but it should work on every platform which has streaming client (W-Fi or Ethernet)

## Install

You need first to download and install Json Streaming Parser library from https://github.com/squix78/json-streaming-parser

Then download this library, put it in Arduino Library Folder, rename the folder to TSClient and restart Arduino IDE

## How to use

The TSClient class:

- Dati3[24]: it contains the daily data;
- Dati1[31] and Dati2[31]: it contains the min value (on Dati1) and max value (on Dati2) for every day in a month.

For all arrays, if a value is not present, the default is 200.

To retrieve data, simply call GetData:

void GetData(const char *channelNum, uint8_t field, uint16_t yy, uint8_t mm, uint8_t dd);
   Get data from a specific day (from 00:00:00 to 23:59:59)
   channelNum: ThingSpeak channel number
   field: field number
   yy: year
   mm: month
   dd: day

void GetData(const char *channelNum, uint8_t field, uint16_t yy, uint8_t mm);
   Get data from a month 
   channelNum: ThingSpeak channel number
   field: field number
   yy: year
   mm: month
 
The WiFiClient can be replaced by EthernetClient

Further improvements in the future

## License

This code is available under the MIT license, which basically means that you can use, modify the distribute the code as long as you give credits to me (and Salsify) and add a reference back to this repository.


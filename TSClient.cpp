/**The MIT License (MIT)

Original json parsing library Copyright (c) 2015 by Daniel Eichhorn
TSClient Copyright (c) 2017 by Vittorio Colzani

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "TSClient.h"

TSClient::TSClient() {;}


void TSClient::GetData(const char *channelNum, uint8_t field, uint16_t yy, uint8_t mm, uint8_t dd) {
  char url[140];

  sprintf(field_c,"field%d",field);
  field_n=field;
  for(uint8_t n=0; n<24;n++) Dati[n]=200;
  StartEntry=false; monthly=false;
  if(TS_R_APIKEY==NULL)
   sprintf(url,"/channels/%s/fields/1.json?timezone=Europe%%2fRome&start=%04d-%02d-%02d%%2000:00:00&end=%04d-%02d-%02d%%2023:59:59",channelNum,yy,mm,dd,yy,mm,dd);
  else
   sprintf(url,"/channels/%s/fields/1.json?api_key=%s&timezone=Europe%%2fRome&start=%04d-%02d-%02d%%2000:00:00&end=%04d-%02d-%02d%%2023:59:59",channelNum,TS_R_APIKEY,yy,mm,dd,yy,mm,dd);
  Serial.println(url);
  doUpdate(url);
}

void TSClient::GetData(const char *channelNum, uint8_t field, uint16_t yy, uint8_t mm) {
  char url[140];

  sprintf(field_c,"field%d",field);
  field_n=field;
  for(uint8_t n=0; n<31;n++) {Dati[n]=200; Dati2[n]=200; if(n<24) Dati3[n]=200;}
  StartEntry=false; monthly=true;
  if(TS_R_APIKEY==NULL)
   sprintf(url,"/channels/%s/fields/%d.json?timezone=Europe%%2fRome&start=%04d-%02d-01%%2000:00:00&end=%04d-%02d-%02d%%2023:59:59",channelNum,field,yy,mm,yy,mm,GMese[mm-1]);
  else
   sprintf(url,"/channels/%s/fields/%d.json?api_key=%s&timezone=Europe%%2fRome&start=%04d-%02d-01%%2000:00:00&end=%04d-%02d-%02d%%2023:59:59",channelNum,field,TS_R_APIKEY,yy,mm,yy,mm,GMese[mm-1]);
  Serial.println(url);
  doUpdate(url);
}

void TSClient::doUpdate(String url) {
  JsonStreamingParser parser;
  parser.setListener(this);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect("api.thingspeak.com", httpPort)) {
    Serial.println("connection failed");
    error_status=255;
    return;
  }

  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.thingspeak.com\r\n" +
               "Connection: close\r\n\r\n");
  int retryCounter = 0;
  while(!client.available()) {
    delay(1000);
    retryCounter++;
    if (retryCounter > 10) {
      error_status=255;
      return;
    }
  }

  int pos = 0;
  boolean isBody = false;
  char c;

  int size = 0;
  client.setNoDelay(false);
  while(client.connected()) {
    while((size = client.available()) > 0) {
      c = client.read();
      if (c == '{' || c == '[') {
        isBody = true;
      }
      if (isBody) {
        parser.parse(c);
      }
    }
  }
  error_status=parser.state;
}

void TSClient::whitespace(char c) {
  Serial.println("whitespace");
}

void TSClient::startDocument() {
  Serial.println("Start Parsing");
}

void TSClient::key(String key) {
  currentKey = String(key);
  
  if(currentKey=="feeds") {StartEntry=true; entry=0; lastEntry=0;}
  
}

void TSClient::value(String value) {

 if(StartEntry&&!monthly)
 {
  if(currentKey=="created_at")
   {
    entry=value.substring(11,13).toInt();
   }
  if(currentKey==field_c)
  {
   Dati3[entry]=value.toFloat();
  }
 }
 else if(StartEntry&&monthly)
 {
  if(currentKey=="created_at")
  {
   entry=value.substring(8,10).toInt();
   entryH=value.substring(11,13).toInt();
   if(lastEntry==0) {lastEntry=entry;}
   if(lastEntry!=entry)
    {     
     Dati[lastEntry]=200; Dati2[lastEntry]=-200;
     for(uint8_t n=0; n<24; n++)
     {
      if(Dati3[n]!=200)
       {if(Dati3[n]>Dati2[lastEntry]) Dati2[lastEntry]=Dati3[n];
        if(Dati3[n]<Dati[lastEntry]) Dati[lastEntry]=Dati3[n];}
     }
     for(uint8_t n=0; n<24; n++) Dati3[n]=200;
     lastEntry=entry;
    }
  }
  if(currentKey==field_c)
  {
   Dati3[entryH]=value.toFloat();
  }
 }

}

void TSClient::endArray() {
     Dati[lastEntry]=200; Dati2[lastEntry]=-200;
     for(uint8_t n=0; n<24; n++)
     {
      if(Dati3[n]!=200)
       {if(Dati3[n]>Dati2[lastEntry]) Dati2[lastEntry]=Dati3[n];
        if(Dati3[n]<Dati[lastEntry]) Dati[lastEntry]=Dati3[n];}
     }
}

void TSClient::startObject() {
  currentParent = currentKey;
}

void TSClient::endObject() {
  currentParent = "";
}

void TSClient::endDocument() {
 error_status=-1;
 Serial.println("End Parsing");
}

void TSClient::startArray() {
}



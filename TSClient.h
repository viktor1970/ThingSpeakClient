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

#pragma once
#include <JsonListener.h>
#include <JsonStreamingParser.h>

#define NChannel_n XXXXX		//Insert channel number (long int)
#define NChannel_c "XXXXX"		//Insert channel number (char)
#define TS_R_APIKEY "YYYYY"		//Insert API Read key; if no key needed, set as NULL (without quotes)

const uint8_t GMese[12]={31,28,31,30,31,30,31,31,30,31,30,31}; //Number of days in every month

class TSClient: public JsonListener {
  private:
    String currentKey;
    String currentParent = "";
    uint16_t entry=0;
    uint8_t entryH;
    uint16_t lastEntry=0;
    bool monthly=false;
    boolean StartEntry=false;
    void doUpdate(String url);

    char field_c[7];
    uint8_t field_n=1;

  public:
    int error_status;
    TSClient();
    void GetData(const char *channelNum, uint8_t field, uint16_t yy, uint8_t mm, uint8_t dd);
    void GetData(const char *channelNum, uint8_t field, uint16_t yy, uint8_t mm);

    float Dati[31]={200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,
                    200,200,200,200,200,200,200,200,200,200,200};
    float Dati2[31]={200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,
                    200,200,200,200,200,200,200,200,200,200,200};
    float Dati3[24]={200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200};

    virtual void whitespace(char c);
    virtual void startDocument();
    virtual void key(String key);
    virtual void value(String value);
    virtual void endArray();
    virtual void endObject();
    virtual void endDocument();
    virtual void startArray();
    virtual void startObject();
};

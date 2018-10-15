/*
The MIT License (MIT)

Copyright (c) Alexander Nusov 2015 

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
#include "crc32.h"

Crc32::Crc32()
{
  quint32 crc;

  // initialize CRC table
  for (int i = 0; i < 256; i++) {
    crc = i;
    for (int j = 0; j < 8; j++) {
      crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
    }
    
    crc_table[i] = crc;
  }
}

void Crc32::initInstance(int i)
{
  instances[i] = 0xFFFFFFFFUL;
}

void Crc32::pushData(int i, char *data, int len)
{
  quint32 crc = instances[i];
  if(crc) {
    for(int j = 0; j < len; j++) {
      crc = crc_table[(crc ^ data[j]) & 0xFF] ^ (crc >> 8);
    }
    
    instances[i] = crc;
  }
}

quint32 Crc32::releaseInstance(int i)
{
  quint32 crc32 = instances[i];
  if (crc32) {
    instances.remove(i);
    return crc32 ^ 0xFFFFFFFFUL;
  }
  else {
    return 0;
  }
}

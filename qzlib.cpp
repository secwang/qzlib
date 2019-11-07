/*
   qgz - gzip extension for kdb+/q
   Copyright (C) 2016  Lucas Martin-King

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <cstdint>
#include <arpa/inet.h> // for ntohl, etc


#include "string.h"
#include "zlib.h"

#define KXVER 3
#include "k.h"
#define BUFLEN 65536


int gzDecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata)
{
    int err = 0;
    z_stream d_stream = {0}; /* decompression stream */

    static char dummy_head[2] = {
            0x8 + 0x7 * 0x10,
            (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };

    d_stream.zalloc = NULL;
    d_stream.zfree = NULL;
    d_stream.opaque = NULL;
    d_stream.next_in = zdata;
    d_stream.avail_in = 0;
    d_stream.next_out = data;


    if (inflateInit2(&d_stream, -MAX_WBITS) != Z_OK) {
        return -1;
    }

    // if(inflateInit2(&d_stream, 47) != Z_OK) return -1;

    while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END)
            break;

        if (err != Z_OK) {
            if (err == Z_DATA_ERROR) {
                d_stream.next_in = (Bytef*) dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) {
                    return -1;
                }
            } else {
                return -1;
            }
        }
    }

    if (inflateEnd(&d_stream)!= Z_OK)
        return -1;
    *ndata = d_stream.total_out;
    return 0;
}

template<size_t N>
std::string convert(char const(&data)[N])
{
   return std::string(data, std::find(data, data + N, '\0'));
}




extern "C" K gunzip(K x)
{
  if (x->t != KG)
  {
    return krr("expecting a byte");
  }

  const int len = x->n;
  const Byte * buf = (Byte *)&kG(x)[0];

  unsigned char data[BUFLEN] = {0};
  uLong datalen = sizeof(data);

  gzDecompress((Byte *) buf, len, (Byte *) data, &datalen);

  
  K uc = kp((char *)data);
  if (! uc)
  {
        return krr("ktn uc");
  }


  return uc;
}



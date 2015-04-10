/** -*- C++ -*-
 * \file
 *
 * \brief Simplified C++ API for Pato
 *
 * Copyright (c) 2014-2015 Dimitry Kloper <kloper@users.sf.net>. 
 * All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing
 * official policies, either expressed or implied, of the Pato Project.
 */

#ifndef _pato_print_h_
#define _pato_print_h_

namespace pato {

uint8_t print_upload(Pato &pato, const uint8_t* begin, const uint8_t *end) {
   uint8_t rc = 1;
   uint8_t a, b;

   while( begin != NULL && end != NULL && begin < end ) {
      a = *begin++;
      b = (begin < end) ? *begin++ : 0;
      rc = pato.print_put(a,b);
      if( !rc ) return rc;            
   }
   return rc;
}

template <typename T>
uint8_t print_upload(Pato &pato, const T &val) {
   return print_upload(pato, (uint8_t*)&val, (uint8_t*)&val + sizeof(T));
}

// template <typename T>
// uint8_t print_upload(Pato &pato, T *val) {
//    return print_upload(pato, (uint8_t*)val, (uint8_t*)(val+1));
// }

uint8_t print_upload(Pato &pato, const char *val) {
   const char *last = val;
   while(*last) last++; 
   return print_upload(pato, (uint8_t*)val, (uint8_t*)(last+1));
}

uint16_t print_upload_content(Pato &pato, const char *str) {
   uint16_t offset;
   uint8_t rc;
   
   rc = pato.print_get_addr(offset);
   if(!rc) return 0xffff;

   rc = print_upload(pato, str);
   if(!rc) return 0xffff;

   return offset;
}

uint16_t print_upload_content(Pato &pato, char *str) {
   return print_upload_content(pato, (const char*)str);
}

template <typename T>
uint16_t print_upload_content(Pato &pato, const T &val) {
   return 0xffff;
}

uint8_t upload_format(Pato &pato, const char *format) {
   uint8_t rc;
   
   if( !format )
      return 0;     
   
   if( format != pato.cache() ) {
      rc = pato.print_set_addr(0);
      if( !rc ) return rc;

      pato.cache(format);

      rc = print_upload(pato, format);
      if( !rc ) return rc;
   }

   const char *last = format;
   while(*last) last++;
   rc = pato.print_set_addr(last-format+1);
   return rc;
}

uint8_t print(Pato &pato, const char *format) {
   uint8_t rc = upload_format(pato, format);
   if( !rc ) return rc;

   rc = pato.print_commit(0);
   return rc;
}

template <typename A0>
uint8_t print(Pato &pato, const char *format, const A0 &a0) {
   uint8_t rc = upload_format(pato, format);
   if( !rc ) return rc;

   uint16_t a0_offset = print_upload_content(pato, a0);

   uint16_t args_offset;
   rc = pato.print_get_addr(args_offset);
   if( !rc ) return rc;

   if( a0_offset == 0xffff )
      rc = print_upload(pato, a0);
   else
      rc = pato.print_put_ptr(a0_offset);
   if( !rc ) return rc;

   rc = pato.print_commit(args_offset);
   return rc;
}

template <typename A0, typename A1>
uint8_t print(Pato &pato, const char *format, const A0 &a0, const A1 &a1) {
   uint8_t rc = upload_format(pato, format);
   if( !rc ) return rc;

   uint16_t a0_offset = print_upload_content(pato, a0);
   uint16_t a1_offset = print_upload_content(pato, a1);

   uint16_t args_offset;
   rc = pato.print_get_addr(args_offset);
   if( !rc ) return rc;

   if( a0_offset == 0xffff )
      rc = print_upload(pato, a0);
   else
      rc = pato.print_put_ptr(a0_offset);
   if( !rc ) return rc;

   if( a1_offset == 0xffff )
      rc = print_upload(pato, a1);
   else
      rc = pato.print_put_ptr(a1_offset);
   if( !rc ) return rc;

   rc = pato.print_commit(args_offset);
   return rc;
}

template <typename A0,
          typename A1,
          typename A2>
uint8_t print(Pato &pato, const char *format,
              const A0 &a0,
              const A1 &a1,
              const A2 &a2)
{
   uint8_t rc = upload_format(pato, format);
   if( !rc ) return rc;

   uint16_t a0_offset = print_upload_content(pato, a0);
   uint16_t a1_offset = print_upload_content(pato, a1);
   uint16_t a2_offset = print_upload_content(pato, a2);

   uint16_t args_offset;
   rc = pato.print_get_addr(args_offset);
   if( !rc ) return rc;

   if( a0_offset == 0xffff )
      rc = print_upload(pato, a0);
   else
      rc = pato.print_put_ptr(a0_offset);
   if( !rc ) return rc;

   if( a1_offset == 0xffff )
      rc = print_upload(pato, a1);
   else
      rc = pato.print_put_ptr(a1_offset);
   if( !rc ) return rc;

   if( a2_offset == 0xffff )
      rc = print_upload(pato, a2);
   else
      rc = pato.print_put_ptr(a2_offset);
   if( !rc ) return rc;

   rc = pato.print_commit(args_offset);
   return rc;
}

}; // end of namespace pato

#endif /* _pato_print_h_ */

/*- 
 * Copyright (c) 2017 Etienne Bagnoud <etienne@artisan-numerique.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include "strid.h"
#include <stdio.h>

#define STR_UPPER(A)       ((A) > 'Z' ? (A) - 32 : (A))

const unsigned char STRID_BASE36[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const unsigned char CRC8[256] = {
    0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83,
    0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
    0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e,
    0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
    0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0,
    0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
    0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d,
    0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
    0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5,
    0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
    0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58,
    0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
    0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6,
    0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
    0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
    0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
    0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f,
    0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
    0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92,
    0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
    0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c,
    0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
    0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1,
    0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
    0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49,
    0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
    0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4,
    0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
    0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a,
    0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
    0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7,
    0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
};

static inline long int _strid_hash(const char * str) {
    long int init = 0;
    long int prev = 0;
    int i = 0;

    for(i = 0; str[i] != 0; i++) {
        init = (init << 5) + (init >> 2) + str[i];
    }

    return init;
} 

inline void strid_init(const char * any) {
    srand48(_strid_hash(any));
}

inline static unsigned char _strid_generate(Strid strid, int from, unsigned char crc) {
    int i = 0;
    
    if(from < 0) { from = 0; }
    for(i = from; i < STRID_SIZE - 2; i++) {
        strid[i] = STRID_BASE36[lrand48() % 36];
        crc = CRC8[crc ^ strid[i]];
    }
    
    return crc;
}

inline static void _strid_close(Strid strid, unsigned char crc) {
    strid[STRID_SIZE - 2] = STRID_BASE36[(crc & 0x0F) % 36];
    strid[STRID_SIZE - 1] = STRID_BASE36[((crc& 0xF0) >> 4) % 36];
    strid[STRID_SIZE] = '\0';
}

inline void strid_generate_prefix(Strid strid, const char * prefix) {
    long int p = 0;
    unsigned char i = 0;
    unsigned char crc = 0;

    p = _strid_hash(prefix);
    
    i = ((p & 0xFF000000) >> 24) ^ ((p & 0x00FF0000) > 18);
    strid[0] = STRID_BASE36[ i % 36 ];
    i = ((p & 0x0000FF00) >> 8) ^ ((p & 0x000000FF));
    strid[1] = STRID_BASE36[ i % 36 ];

    crc = CRC8[crc ^ strid[0]];
    crc = CRC8[crc ^ strid[1]];

    crc = _strid_generate(strid, 2, crc);
    _strid_close(strid, crc);
}

inline void strid_generate_random(Strid strid) {
    unsigned char crc = 0;

    crc = _strid_generate(strid, 0, crc);
    _strid_close(strid, crc);
}

inline int strid_valid(Strid strid) {
    int i = 0;
    unsigned char crc = 0;

    for(i = 0; i < STRID_SIZE - 2; i++) {
        crc = CRC8[crc ^ STR_UPPER(strid[i])];
    }

    if(STRID_BASE36[(crc & 0x0F) % 36] == STR_UPPER(strid[STRID_SIZE - 2]) &&
    STRID_BASE36[((crc& 0xF0) >> 4) % 36] == STR_UPPER(strid[STRID_SIZE - 1])) {
        return 1;
    }
    return 0;
}

inline int strid_equal(Strid strid1, Strid strid2) {
    int i = 0;
    unsigned char a = 0, b = 0;
    for(i = 0; i <= STRID_SIZE; i++) {
        if(STR_UPPER(strid1[i]) != STR_UPPER(strid2[i])) { return 0; }
    }
    return 1;
}

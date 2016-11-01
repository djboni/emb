/*
 Copyright 2016 Djones A. Boni

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdint.h>
#include "pd_assert.h"

namespace emb {

template<class T, uint16_t Size, bool checkRange = true>
class array {
public:
    typedef T type;

    type& operator[](uint16_t n) {
        if(checkRange)
            ASSERT(n < Size);
        return Data[n];
    }

    const type& operator[](uint16_t n) const {
        if(checkRange)
            ASSERT(n < Size);
        return Data[n];
    }

    uint16_t size() const { return Size; }

private:
    type Data[Size];
};

template<uint16_t Size, class T = uint8_t, bool checkRange = true>
class bit_array {
public:
    typedef bool type;

private:
    static const uint16_t SizeX = (Size + 8U * sizeof(T) - 1U) / (8U * sizeof(T));

    class boolX {
        T Data;
    public:
        bool get(uint8_t n) const {
            return (Data >> n) & 1U;
        }
        void set(uint8_t n, bool val) {
            if(val)
                Data |= (1U << n);
            else
                Data &= ~(1U << n);
        }
    };

public:
    class typeref {
        uint8_t pos;
        boolX* data;
        typeref() {}
    public:
        typeref(boolX& d, uint8_t n): pos(n), data(&d) {}
        operator bool() const { return data->get(pos); }
        typeref& operator=(bool val) {
            data->set(pos, val);
            return *this;
        }
    };

    typeref operator[](uint16_t n) {
        if(checkRange)
            ASSERT(n < Size);
        return typeref(Data[n / 8], n % 8);
    }

    type operator[](uint16_t n) const {
        if(checkRange)
            ASSERT(n < Size);
        return Data[n / 8].get(n % 8);
    }

    uint16_t size() const { return Size; }

private:
    boolX Data[SizeX];
};

}

#endif //__ARRAY_H__

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
    static const uint16_t size = Size;

    type& operator[](uint16_t n) {
        if(checkRange)
            ASSERT(n < size);
        return Data[n];
    }

    const type& operator[](uint16_t n) const {
        if(checkRange)
            ASSERT(n < size);
        return Data[n];
    }

private:
    type Data[size];
};

template<uint16_t Size, bool checkRange>
class array<bool, Size, checkRange> {
public:
    typedef bool type;
    static const uint16_t size = Size;

private:
    static const uint16_t K = (size + 7U) / 8U;

    class bool8 {
        uint8_t Data;
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
        bool8* data;
        typeref() {}
    public:
        typeref(bool8& d, uint8_t n): pos(n), data(&d) {}
        operator bool() const { return data->get(pos); }
        typeref& operator=(bool val) {
            data->set(pos, val);
            return *this;
        }
    };

    typeref operator[](uint16_t n) {
        if(checkRange)
            ASSERT(n < size);
        return typeref(Data[n / 8], n % 8);
    }

    type operator[](uint16_t n) const {
        if(checkRange)
            ASSERT(n < size);
        return Data[n / 8].get(n % 8);
    }

private:
    bool8 Data[K];
};

}

#endif //__ARRAY_H__

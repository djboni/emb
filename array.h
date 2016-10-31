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

template<class T, uint16_t N, bool checkRange = true>
class array {
public:
    typedef T type;
    static const uint16_t size = N;

    type& operator[](uint16_t n) {
        if(checkRange)
            ASSERT(n < N);
        return Data[n];
    }

    const type& operator[](uint16_t n) const {
        if(checkRange)
            ASSERT(n < N);
        return Data[n];
    }

private:
    type Data[N];
};

}

#endif //__ARRAY_H__

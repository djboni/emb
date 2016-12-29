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

#ifndef __BOUNDEDINT_H__
#define __BOUNDEDINT_H__

#include "pd_assert.h"

namespace emb {

template<class T, T Limit>
class boundedInt;



template<class T, T Limit>
class boundedInt {
public:
    T operator++() { return val = (val < Limit - 1) ? val + 1 : 0; }
    T operator--() { return val = (val > 0) ? val - 1 : Limit - 1; }

    T operator++(int) { T x = val; operator++(); return x; }
    T operator--(int) { T x = val; operator--(); return x; }

    T operator=(T x) {
        ASSERT(x < Limit);
        val = x;
        return x;
    }

    operator T() { return val; }
private:
    T val;
};

} /* namespace emb */

#endif /* __BOUNDEDINT_H__ */

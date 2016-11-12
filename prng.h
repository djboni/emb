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

#ifndef PRNG_H_
#define PRNG_H_

#include <stdint.h>

namespace emb {

template<uint16_t PoolSizeBits, class Hash, class UintCounter = uint16_t>
class prng;



/** PRNG - Pseudo-Random Number Generator

 Template parameters:

 PoolSizeBits - Size of the entropy poll in bits. Must be a multiple of 8 (no
 checks are performed).

 Hash - Class used as one-way hash.

 UintCounter - Counter type. The counter is hashed with the entropy pool and
 incremented every time a pseudo-random number is generated. If the counter
 overflows without a reseed or adding entropy the number sequence will repeat
 (no checks are performed).

 The Hash class must:
 - Be default constructible
 - Define this constant static integer:
    static const int8_t size  -  Hash size in bytes
 - Define these member functions:
    init()                    -  Initialize the Hash object
    update(uint8_t data)      -  Update the Hash value
    final()                   -  Finalize the hash object
    operator[](int8_t i)      -  Get the ith byte of the Hash

 As an Hash class example, or ready to use code, take a look at CRC32 and
 CRC16_CCITT in https://github.com/djboni/hdlc . Be wary these are Cyclic
 Redundancy Code classes and are NOT meant to be cryptographically secure.
 */
template<uint16_t PoolSizeBits, class Hash, class UintCounter>
class prng {
public:
    static const uint8_t PoolSize = PoolSizeBits / 8U;

    /** Set entropy pool with seed_val. Previous entropy is lost. */
    template<class T> void seed(const T& seedVal);

    /** Add entropy to the entropy pool. */
    void addEntropy(const uint8_t (&entropyVal)[PoolSize]);

    /** Get a pseudo-random number of type T. */
    template<class T> T rand() const;

private:
    uint8_t EntrpyPool[PoolSize];
    mutable UintCounter Counter;
};



template<uint16_t PoolSizeBits, class Hash, class UintCounter>
template<class T>
void prng<PoolSizeBits, Hash, UintCounter>::seed(const T& seedVal)
{
    const uint8_t* pSeedVal = reinterpret_cast<const uint8_t*>(&seedVal);
    uint8_t i;
    for(i = 0U; i < sizeof(seedVal) && i < sizeof(EntrpyPool); ++i)
        EntrpyPool[i] = *pSeedVal++;
    for( ; i < sizeof(EntrpyPool); ++i)
        EntrpyPool[i] = 0U;
}

template<uint16_t PoolSizeBits, class Hash, class UintCounter>
void prng<PoolSizeBits, Hash, UintCounter>::addEntropy(const uint8_t (&entropyVal)[PoolSize])
{
    for(uint8_t i = 0U; i < sizeof(EntrpyPool); ++i)
        EntrpyPool[i] ^= entropyVal[i];
}

template<uint16_t PoolSizeBits, class Hash, class UintCounter>
template<class T>
T prng<PoolSizeBits, Hash, UintCounter>::rand() const
{
    T randval;

    uint8_t c = 0;
    uint8_t* pRandval = reinterpret_cast<uint8_t*>(&randval);
    do {
        Hash hash;

        hash.init();
        for(uint8_t i = 0U; i < PoolSize; ++i)
        hash.update(EntrpyPool[i]);

        UintCounter localCounter = Counter++;

        const uint8_t* pLocalCounter =
                reinterpret_cast<const uint8_t*>(&localCounter);
        for(uint8_t i = 0U; i < sizeof(localCounter); ++i)
            hash.update(*pLocalCounter++);

        hash.final();

        for(uint8_t i = 0U ; i < hash.size && c < sizeof(T); ++i, ++c)
            *pRandval++ = hash[i];

    } while(c < sizeof(T));

    return randval;
}

} /* namespace emb */

#endif /* PRNG_H_ */

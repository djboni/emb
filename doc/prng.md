# PRNG - Pseudo-Random Number Generator

## Example 1

Using microcontroller ATmega328P.

Code

```cpp
/* https://github.com/djboni/arduinutil */
#include "Arduinutil.h"
#include "Misc/macroutils.h"

/* https://github.com/djboni/hdlc */
#include "hdlc/CRC32.h"

/* https://github.com/djboni/emb */
#include "emb/prng.h"

emb::prng<32, CRC32> rng;

int main(void)
{
    init();
    timerBegin();
    Serial_begin(115200, SERIAL_8N1);

    Serial_write("Init\n");
    Serial_flush();
    delay(1);

    rng.seed(0);

    for(;;)
    {
        static uint32_t i = 0;
        uint16_t val = rng.rand<uint16_t>();
        ++i;

        PERIODIC_RUN(1000,
        Serial_print("%lu 0x%04X\n", i, val);
        );
    }
}
```

Serial output

```
Init
1 0xA1A3
35596 0x22C0
71186 0x37E2
106776 0xB64B
142364 0x06DA
177952 0x8912
213540 0xF67E
249128 0xADCB
284716 0x850C
320304 0x2A0C
```

## Example 2

Using microcontroller ATmega328P.

Code

```cpp
/* https://github.com/djboni/arduinutil */
#include "Arduinutil.h"
#include "Misc/macroutils.h"

/* https://github.com/djboni/hdlc */
#include "hdlc/CRC32.h"

/* https://github.com/djboni/emb */
#include "emb/prng.h"

/* Put rng in the .noinit section. This will keep the entropy and counter values
 after a reset. */
__attribute__((section(".noinit")))
emb::prng<32, CRC32> rng;

void addEntropy()
{
    CRC32 hash;
    hash.init();

    adcBegin();
    analogReference(INTERNAL1V1);

    for(uint8_t i = 0; i < 32; ++i)
    {
        /* Collect entropy from ADC and hash it. */
        uint8_t adcEntropy = 0;
        for(uint8_t j = 0; j < 8; ++j)
            adcEntropy = (adcEntropy << 1) ^ analogRead(ATEMP);
        hash.update(adcEntropy);
    }

    adcEnd();

    hash.final();

    /* Cast hash pointer to a const uint8_t[rng.PoolSize] pointer.
     The pointer is dereferenced and taken as a reference by the function. */
    rng.addEntropy(*reinterpret_cast<const uint8_t(*)[rng.PoolSize]>(&hash));
}

int main(void)
{
    init();
    timerBegin();
    Serial_begin(115200, SERIAL_8N1);

    Serial_write("Init\n");
    Serial_flush();
    delay(1);

    addEntropy();

    for(;;)
    {
        static uint32_t i = 0;
        uint16_t val = rng.rand<uint16_t>();
        ++i;

        PERIODIC_RUN(1000,
            Serial_print("%lu 0x%04X\n", i, val);
        );
    }
}
```

Serial output

```
Init
1 0xC328
9024 0xF17D
18046 0xF66E
27068 0x879C
36090 0xCA4C
45112 0x96B9
54134 0x4818
63156 0x9164
72178 0x0747
81200 0x3C37
```

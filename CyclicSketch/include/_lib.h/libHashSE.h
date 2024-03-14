#ifndef LIB_HASH_SE_H
#define LIB_HASH_SE_H

#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include <string>

class ISketchCount
{
public:
    virtual ~ISketchCount(){}

    virtual void clear() = 0;

    virtual uint32_t getHashPos(int k) = 0;

    virtual bool calHashKey(const uint8_t *element, unsigned length) = 0;

    virtual bool calHashKeyB(const uint8_t *element, unsigned length) = 0;

    virtual bool checkBuf() = 0;

    virtual int getKeyValue() = 0;

    virtual int getUnitSize() = 0;

    virtual int getArrayLen() = 0;

    virtual uint32_t getMin() = 0;

    virtual uint32_t countMin() = 0;

    virtual uint32_t countMinThreshold(uint32_t thre) = 0;

    virtual bool count2Cut(uint32_t thre) = 0;
    
    virtual int getOverflow() = 0;

    virtual uint8_t getUCBits() = 0;

    virtual bool setUCBits(uint8_t bits) = 0;

    virtual uint16_t getUSBits() = 0;

    virtual bool setUSBits(uint16_t bits) = 0;

    virtual uint32_t getULBits() = 0;

    virtual bool setULBits(uint32_t bits) = 0;
};

class ISketchMulCount
{
public:
    virtual ~ISketchMulCount(){}

    virtual void clear() = 0;

    virtual uint32_t getHashPos(int k) = 0;

    virtual bool calHashKey(const uint8_t *element, unsigned length) = 0;

    virtual bool calHashKeyB(const uint8_t *element, unsigned length) = 0;

    virtual bool checkBuf() = 0;

    virtual int getKeyValue() = 0;

    virtual int getUnitSize() = 0;

    virtual int getArrayLen() = 0;

    virtual bool getData(unsigned char* data) = 0;

    virtual bool setData(unsigned char* data) = 0;

    virtual uint8_t* getBuffer(int index) = 0;
};

class CHashTools
{
public:
    static uint32_t two_to_power_of(int n);

    static uint32_t HashBuffer(const uint8_t*element, uint32_t length, uint32_t bit_num);

    static ISketchCount* create_SKC(int bit_num, int unit_size, int type=0);

    static ISketchMulCount* create_SKMC(int bit_num, int unit_size, int type=0);

    static ISketchMulCount* create_user_SKMC(int num_hash, int bit_num, int unit_size);
};

#endif

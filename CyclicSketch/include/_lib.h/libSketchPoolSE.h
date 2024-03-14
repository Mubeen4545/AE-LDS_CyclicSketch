#ifndef LIB_SKETCH_POOL_SE_H
#define LIB_SKETCH_POOL_SE_H

#include <vector>
#include <stdint.h>
#include "IEVStorage.h"
#include "libBaseSE.h"
#include "libPacketSE.h"

class ISketchPool
{
public:
    virtual ~ISketchPool(){}
public:
    virtual bool addSketch(packet_statistics_object_type skType, int bit, int pck_thre,
                           uint64_t skFeature, std::vector<int>* lpVctTCPRange, std::vector<int>* lpVctUDPRange) = 0;
    virtual void setStorage(IEigenvectorStorage* lpStor) = 0;
public:
    virtual bool procPacket(CPacket* lppck) = 0;
};

class CSketchPoolCreator
{
public:
    static ISketchPool* create_sketch_pool(int ratio);
};

#endif
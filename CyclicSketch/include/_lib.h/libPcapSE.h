#ifndef LIB_PCAP_SE_H
#define LIB_PCAP_SE_H

#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include <string>
#include "libPacketSE.h"

class CReader
{
public:
    virtual ~CReader(){}
public:
    virtual bool openPcap() = 0;
    virtual int readPacket() = 0;
    virtual int nextPacket() = 0;
    virtual CPacket* getPacket() = 0;
public:
    virtual bool haveMACData() = 0;
    virtual int getError() = 0;
    virtual double getBaseTM() = 0;
    virtual double getReadTime() = 0;
};

class CWriter
{
public:
    virtual ~CWriter(){}
public:
    virtual int getPckNum() = 0;
    virtual bool openWriter() = 0;
    virtual void closeWriter() = 0;
    virtual bool addPacket(CPacket* pck) = 0;
};

class CSquReader
{
public:
    virtual ~CSquReader(){}
public:
    virtual bool openPcap() = 0;
    virtual int readPacket() = 0;
    virtual CPacket* getPacket() = 0;
public:
    virtual bool findSquPcap(double time) = 0;
    virtual bool checkSquPcap() = 0;
public:
    virtual void setMsg(bool bm) = 0;
};

extern "C"
{
CReader* create_pcap_reader(const char* filename);
CWriter* create_pcap_writer(const char* filename, int type, int linktype=1);
CSquReader* create_squ_pcap_reader(const char* filename);
}

#endif

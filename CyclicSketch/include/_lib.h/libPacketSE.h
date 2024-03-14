#ifndef LIB_PACKET_SE_H
#define LIB_PACKET_SE_H

#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include <string>
#include <sys/time.h>

struct timeVS
{
    uint32_t            tv_sec;
    uint32_t            tv_usec;
};

class CPacket
{
public:
    static const int lenIPV6 = 16;    
    static const int lenMAC = 6;
    static const int MAX_PACKET_LEN = 65536;
public:
    virtual ~CPacket(){}
public:                 
    virtual double      getPckGMTTime() = 0;
    virtual timeVS*     getPckGMTTimeval() = 0;
    virtual double      getPckOffTime() = 0;
    virtual double      getBeginTime() = 0;
public:                 
    virtual uint8_t     getProtocol() = 0;
    virtual uint8_t     getIPVer() = 0;
    virtual uint32_t    getSrcIP4() = 0;
    virtual uint32_t    getDstIP4() = 0;
    virtual uint8_t*    getSrcIP6() = 0;
    virtual uint8_t*    getDstIP6() = 0;
    virtual uint16_t    getSrcPort() = 0;
    virtual uint16_t    getDstPort() = 0;
public:                 
    virtual int         getLenFrameHeader() = 0;
    virtual int         getLenIPHeader() = 0;
    virtual int         getLenTCPHeader() = 0;
    virtual int         getLenUDPHeader() = 0;
    virtual int         getLenHeader() = 0;
    virtual int         getLen_IP_P_Header() = 0;
    virtual int         getLenTotal() = 0;
    virtual int         getLenCap() = 0;
    virtual int         getLenPayload() = 0;
    virtual int         getLenPck() = 0;
public:                 
    virtual uint8_t*    getSrcMAC() = 0;
    virtual uint8_t*    getDstMAC() = 0;
public:                 
    virtual uint8_t     getTCPFlag() = 0;
    virtual uint32_t    getACKSACK() = 0;
    virtual int         getSACKCount() = 0;
    virtual bool        haveTimeStamp() = 0;
    virtual uint8_t*    getTCPOptions(int& len) = 0;
    virtual uint8_t*    getTCPHeader(int& len) = 0;
    virtual uint32_t getSelfSeq() = 0;
    virtual uint32_t getAckSeq() = 0;
public:                 
    virtual uint8_t*    getPacketPayload(int& len) = 0;
    virtual uint8_t*    getPacketBuffer(int &len, int type) = 0;
    virtual uint8_t*    getIPPckBuffer(int &len, int type) = 0;
    virtual uint8_t*    getEthernetHead() = 0;
    virtual uint8_t*    getBuffer() = 0;
public:
    virtual void        set_pcap_pkthdr(const struct timeVS* lpVal, int caplen, int pcklen) = 0;
    virtual void        set_pcap_pkthdr(const struct timeval* lpVal, int caplen, int pcklen) = 0;
    virtual void        set_pcapng_pkthdr(double time, int caplen, int pcklen) = 0;
    virtual void        setPckGMTTime(double tm) = 0;
    virtual void       setBeginTime(double tm) = 0;
    virtual void        setSrcMAC(uint8_t* buf) = 0;
    virtual void        setDstMAC(uint8_t* buf) = 0;
    virtual void        modifyFrameLayerData() = 0;
    virtual void        setSrcIP4(uint32_t ip) = 0;
    virtual void        setDstIP4(uint32_t ip) = 0;
    virtual void        modifyIPLayerData() = 0;
    virtual void        setSrcPort(uint16_t port) = 0;
    virtual void        setDstPort(uint16_t port) = 0;
    virtual void        modifyTransportLayerData() = 0;
public:                 
    virtual uint64_t    getPckNum() = 0;
    virtual void        setPckNum(uint64_t num) = 0;
    virtual int         getCapType() = 0;
public:
    virtual int         getPckDetailFromPcapFile(int pos, int len, uint32_t linktype) = 0;
    virtual int         getPckDetailFromBuffer(const uint8_t* lpbuf, int len, uint32_t linktype) = 0;
};

class CPacketTools
{
public:                 
    static uint16_t     invUint16(uint16_t dd);
    static uint32_t     invUint32(uint32_t dd);
public:                 
    static void         IPV4_toString(uint32_t ip, char* str);
    static void         IPV6_toString(uint8_t* ip6, char* str, int bfile=0);
    static void         MAC_toString(uint8_t* mac, char* str, int bfile=0);
    static void         IPV4Subnet_toStr(uint32_t ip, char* str, int ibc = 0, int type=0);
    static void         IPV6Subnet_toStr(uint8_t* ip6, char* str, int bfile=0, int type=0);
    static void         IPV6_toFilename(uint8_t* ip6, char* str);
public:                 
    static bool         getHashBuf_IPMAC(CPacket* lpPck, uint8_t* buf, int &len, bool bSou);
    static bool         getHashBuf_MACSubnet(CPacket* lpPck, uint8_t* buf, int &len, bool bSou);
    static bool         getHashBuf_MACSubnetB(CPacket* lpPck, uint8_t* buf, int &len, bool bSou);
    static bool         getHashBuf_MACSubnetpair(CPacket* lpPck, uint8_t* buf, int &len, bool bSou);
    static bool         getHashBuf_MACSubnetBpair(CPacket* lpPck, uint8_t* buf, int &len, bool bSou);
    static bool         getHashBuf_MAC(CPacket* lpPck, uint8_t* buf, int &len, bool bSou);
    static bool         getHashBuf_MACpair(CPacket* lpPck, uint8_t* buf, int &len, bool bSou);
    static bool         getHashBuf_IP(CPacket* lpPck, uint8_t* buf, int &len, bool bSou);
    static bool         getHashBuf_IP_noprot(CPacket* lpPck, uint8_t* buf, int &len, bool bSou);
    static bool         getHashBuf_IPpair(CPacket* lpPck, uint8_t* buf, int &len, bool bSou);
    static bool         getHashBuf_IPpair_noprot(CPacket* lpPck, uint8_t* buf, int &len, bool bSou);
    static bool         getHashBuf_IPport(CPacket* lpPck, uint8_t* buf, int &len, bool bSou);
    static bool         getHashBuf_IPportpair(CPacket* lpPck, uint8_t* buf, int &len, bool bSou);
    static bool         getHashBuf_port(CPacket* lpPck, uint8_t* buf, int &len, bool bSou);
    static bool         getPacketHashBuf(CPacket* lpPck, uint8_t* buf, int &len,  int type, bool bSou);
    static bool         getIppHashBuf(CPacket* lppck, uint8_t* buf, int &len,  bool bSou);
    static bool         getHashBuf_flow_unique(CPacket* lpPck, uint8_t* buf, int &len, bool &bSou);
    static bool         getHashBuf_IPflow_unique(CPacket* lpPck, uint8_t* buf, int &len, bool &bSou);
    static bool         getHashBuf_IPflow_noprot_unique(CPacket* lpPck, uint8_t* buf, int &len, bool &bSou);
    static bool         getHashBuf_MACsubnet_unique(CPacket* lpPck, uint8_t* buf, int &len, bool &bSou);
    static bool         getHashBuf_MAC_unique(CPacket* lpPck, uint8_t* buf, int &len, bool &bSou);
public:                 
    static bool         getStr_IPportpair_from_hashbuf(uint8_t* buf, int len, char *strbuf, int bfile=0);
    static bool         getStr_IPport_from_hashbuf(uint8_t* buf, int len, char *strbuf, int bfile=0);
    static bool         getStr_IPpair_from_hashbuf(uint8_t* buf, int len, char *strbuf, int bfile=0);
    static bool         getStr_IPpair_noprot_from_hashbuf(uint8_t* buf, int len, char *strbuf, int bfile=0);
    static bool         getStr_IP_from_hashbuf(uint8_t* buf, int len, char *strbuf, int bfile=0);
    static bool         getStr_IP_noprot_from_hashbuf(uint8_t* buf, int len, char *strbuf, int bfile=0);
    static bool         getStr_MACSubnetpair_from_hashbuf(uint8_t* buf, int len, char *strbuf, int bfile=0);
    static bool         getStr_MACSubnet_from_hashbuf(uint8_t* buf, int len, char *strbuf, int bfile=0);
    static bool         getStr_MACpair_from_hashbuf(uint8_t* buf, int len, char *strbuf, int bfile=0);
    static bool         getStr_MAC_from_hashbuf(uint8_t* buf, int len, char *strbuf, int bfile=0);
    static bool         getStr_IPMAC_from_hashbuf(uint8_t* buf, int len, char *strbuf, int bfile=0);
public:
    static uint32_t     trans2IP(const char* buf);
    static int          trans2IP6(const char* buf, uint8_t *ip6buf);
    static bool         trans2MAC(const char* buf, unsigned char* mac);
    static uint8_t      str2IP(const char* buf, uint32_t &ip4, uint8_t *ip6buf);
public:                 
    static bool         checkPcapName(std::string filename);
    static uint16_t     hash16Bit(uint32_t pos);
    static uint8_t     hash8Bit(uint32_t pos);
private:
    static bool         B4toUS(const char* buf, uint16_t &us);
    static void         IPV6str(unsigned char* ip6, char* str, const char* cr);
};

extern "C"
{
CPacket* create_packet();
}

#endif

#include <stdio.h>
#include <pcap.h>
#include <iostream>
#include "_lib.h/libPacketSE.h"
#include "PckCap/CPckCap.h"
#include "_lib.h/libSketchPoolSE.h"
#include "_lib.h/libCsvStorage.h"
#include "_lib.h/libconfig.h++"
#include "other/bit_conversion.h"
#include <sys/time.h>

using namespace std;
using namespace libconfig;

timeval timde;

bool add_SDS_SK(Config* lpCfg, ISketchPool* lpSKP)
{
    bool bout = false;
    if(lpCfg && lpSKP)
    {
        int type, bit, thre;
        lpCfg->lookupValue("CyclicSketch_key_type", type);
        cout << "CyclicSketch sketch hash key type:" << type << endl;

        lpCfg->lookupValue("CyclicSketch_sketch_hash_bit", bit);
        cout << "CyclicSketch sketch length:2^" << bit << endl;

        lpCfg->lookupValue("CyclicSketch_sketch_threshold", thre);
        cout << "sketch statistics threshold:" << thre << endl;

        string strFea = lpCfg->lookup("CyclicSketch_sketch_feature");
        cout << "sketch features:" << strFea << endl;
        uint64_t ufea = convStringValue(strFea);

        uint32_t uflag = 12;
        vector<int> vctTCP;
        if(ufea & uflag) 
        {
            vctTCP.clear();
            int cntRange;
            lpCfg->lookupValue("CyclicSketch_sketch_range_count", cntRange);
            cout << "CyclicSketch sketch range count:" << cntRange << endl;
            for(int j=1; j<=cntRange; j++)
            {
                int value;
                if(lpCfg->lookupValue("CyclicSketch_sketch_range_TCP_" + to_string(j), value))
                {
                    vctTCP.push_back(value);
                    cout << "CyclicSketch sketch TCP payload range threshold " + to_string(j) << ":" << value << endl;
                }
            }
        }
        if(lpSKP->addSketch((packet_statistics_object_type)type, bit, thre, ufea, &vctTCP))
            bout = true;
        else
            cout << "CyclicSketch sketch setting error!" << endl;
    }
    return true;
}

int main(int argc, char *argv[])
{

    char buf[UINT8_MAX] = "data.cfg";

    if(argc==2)
        strcpy(buf, argv[1]);

    std::cerr << "capture begin" << std::endl;        

    Config cfg;
    try
    {
        cfg.readFile(buf);
    }
    catch(...)
    {
        std::cerr << "I/O error while reading file." << std::endl;
        return(EXIT_FAILURE); 
    }    

    try
    {
        string dev = cfg.lookup("CyclicSketch_dev");    
        cout << "Capture dev name:" << dev << endl;

        string file = cfg.lookup("CyclicSketch_out_pcap_file");    
        cout << "Capture output file name:" << file << endl;

        int dumptype;
        cfg.lookupValue("CyclicSketch_dump_type", dumptype);
        cout << "dump type(0--not capture, 1--capture sample, 2--capture all):" << dumptype << endl;

        int rationo, ratio;
        cfg.lookupValue("CyclicSketch_ratio", rationo);
        cout << "Sample ratio No.:" << rationo << endl;
        ratio = calSampleRate(rationo);
        cout << "Sample ratio:1/" << ratio << endl;

        uint32_t cntPck;
        cfg.lookupValue("CyclicSketch_max_packet", cntPck);
        cout << "Maximum packet capture:" << cntPck << endl;
        int tmCap;
        cfg.lookupValue("CyclicSketch_capture_time", tmCap);
        cout << "Maximum packet capture time:" << tmCap << endl;

        if(dev.length()>0)
        {
            IEigenvectorStorage* lpStorage = CCsvStorageCreator::createCsvStorage();
            lpStorage->initialPara(file, 0, "");
            ISketchPool* lpSKPool = CSketchPoolCreator::create_sketch_pool(ratio);
            lpSKPool->setStorage(lpStorage);

            if(add_SDS_SK(&cfg, lpSKPool))
            {

                CPckCap* lpPC = new CPckCap(dev, file);
                lpPC->setPcapType(dumptype);
                lpPC->setSketchPool(lpSKPool);

                lpPC->starCapture(cntPck, tmCap, ratio);
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return(EXIT_FAILURE);
    }
    std::cerr << "capture end" << std::endl;        

    return 0;    
}

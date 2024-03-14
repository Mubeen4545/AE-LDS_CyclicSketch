#include <iostream>
#include <cstring>
#include <time.h>
#include <vector>
#include "_lib.h/libconfig.h++"
#include "_lib.h/libSketchPoolSE.h"
#include "_lib.h/libPcapSE.h"
#include "_lib.h/libCsvStorage.h"
#include "other/bit_conversion.h"

using namespace std;
using namespace libconfig;

bool iterPcapPacket(string name, ISketchPool* lpSKP, int ratio)
{
    bool bout = false;
    CPacket *lppck;
    uint64_t cntPck = 0, cntPckSample = 0;
    uint64_t lenPck = 0, lenPckSample = 0;
    clock_t beginC, endC;

    int iCheckNum = 0;
    //random
    if(ratio>1)
        iCheckNum = rand() % ratio;
    cout << "begin number:" << iCheckNum << " ratio:" << ratio << endl;

    CReader* pr = create_pcap_reader(name.c_str());
    bool bret = pr->openPcap();
    if(bret){
        beginC = clock();

        int iret = 0;
        while(iret>=0){
            if(ratio == 1 || cntPck % ratio == iCheckNum)
            {
                iret = pr->readPacket();
                if(iret>=0){
                    lppck = pr->getPacket();
                    if(lppck && (lppck->getProtocol()==6 )     //tcp
                    {         
                        lpSKP->procPacket(lppck);
                    }
                    cntPck ++;
                    lenPck += lppck->getLenPck();
                }
            }
            else
            {
                iret = pr->nextPacket();
            }
            cntPck ++;
            if(!(cntPck & 0xfffff))
                 cout << "count:" << cntPck << endl;
        }
        cout << "================================" << endl;
        cout << "Pck. count:" << cntPck << ", Pck. count:" << cntPck<< endl;
        cout << "Pck. length:" << lenPck << endl;
        endC = clock();
        double tmLen = (endC - beginC);
        double tmSk = tmLen - pr->getReadTime();
        cout << "--------Time to read  + sketch(ms):" << tmLen*1000.0/(double)CLOCKS_PER_SEC << endl;
        cout << "--------Time to read file(ms):" << pr->getReadTime()*1000.0/(double)CLOCKS_PER_SEC <<endl;
        cout << "--------Time to sketch(ms):" << tmSk*1000.0/(double)CLOCKS_PER_SEC <<endl;
    }
    else
        cout << "open pcap file " << name << " error." << endl;

    delete pr;    
    return bout;
}

bool add_CyclicSketch_multi_sketch(Config* lpCfg, ISketchPool* lpSKP)
{
    bool bout = false;
    if(lpCfg && lpSKP)
    {
        int type, bit, thre;
        //type
        lpCfg->lookupValue("CyclicSketch_key_type", type);
        cout << "CyclicSketch key type:" << type << endl;
        //bit
        lpCfg->lookupValue("CyclicSketch_sketch_hash_bit", bit);
        cout << "CyclicSketch sketch length:2^" << bit << endl;
        //threshold
        lpCfg->lookupValue("CyclicSketch_sketch_threshold", thre);
        cout << "sketch statistics threshold:" << thre << endl;
        //features
        string strFea = lpCfg->lookup("CyclicSketch_sketch_feature");
        cout << "sketch features:" << strFea << endl;
        uint64_t ufea = convStringValue(strFea);

        vector<int> vctTCP;
        if(ufea) 
        {
            //have range flag
            vctTCP.clear();
     

            int cntRange;
            lpCfg->lookupValue("CyclicSketch_sketch_range_count", cntRange);
            cout << "CyclicSketch sketch range count:" << cntRange+1 << endl;
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

    std::cerr << "CyclicSketch begin" << std::endl;        

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
        //name
        string name = cfg.lookup("CyclicSketch_in_pcap_file");    
        cout << "CyclicSketch read pcap file:" << name << endl;

        int seed, rationo, ratio;
        //random seed
        cfg.lookupValue("CyclicSketch_random_seed", seed);
        cout << "Random seed:" << seed << endl;
        srand(seed);
        //ratio
        cfg.lookupValue("CyclicSketch_ratio", rationo);
        cout << "Packet ratio No.:" << rationo << endl;
        ratio = calPacket(rationo);
        cout << "Packet ratio:1/" << ratio << endl;

        if(name.length()>0)
        {
            IEigenvectorStorage* lpStorage = CCsvStorageCreator::createCsvStorage();
            lpStorage->initialPara(name, 0, "");
            ISketchPool* lpSKPool = CSketchPoolCreator::create_sketch_pool(ratio);
            lpSKPool->setStorage(lpStorage);

            if(add_CyclicSketch_multi_sketch(&cfg, lpSKPool))
                iterPcapPacket(name, lpSKPool, ratio);

            delete lpSKPool;
        }
        else
            cout << "parameter error." << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return(EXIT_FAILURE);
    }
    std::cerr << "CyclicSketch end" << std::endl;        

    return 0;
}
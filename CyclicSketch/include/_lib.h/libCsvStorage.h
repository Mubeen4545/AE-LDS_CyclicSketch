#ifndef LIB_CSV_STORAGE_H
#define LIB_CSV_STORAGE_H

#include "IEVStorage.h"
#include "libBaseSE.h"

class CCsvStorageCreator
{
public:
    static IEigenvectorStorage* createCsvStorage();
};

class CCsvStorage: public IEigenvectorStorage
{
public:    
    CCsvStorage();
    ~CCsvStorage();
public:    
    void initialPara(std::string strPara, int iPara, std::string resrPara);
    void insertEV(std::string strKey, std::string strValue);
private:
    static const int iLevelCount = pso_SelfDefine; 
private:
    std::string strPcapFN;
    std::string arrFileName[iLevelCount];  
private:
    void saveTitle(std::string strKey, std::string strValue);
    void saveValue(std::string strKey, std::string strValue);
    int getNextBit(uint32_t value, int beginPos);
};

#endif

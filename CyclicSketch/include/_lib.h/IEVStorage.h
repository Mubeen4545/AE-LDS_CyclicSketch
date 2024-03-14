
#ifndef EIGENVECTOR_STORAGE_H
#define EIGENVECTOR_STORAGE_H

#include <cstdlib>
#include <cstring>
#include <string>

class IEigenvectorStorage
{
public:
    virtual ~IEigenvectorStorage(){}
public:
    virtual void initialPara(std::string strPara, int iPara, std::string resrPara) = 0;
    virtual void insertEV(std::string strKey, std::string strValue) = 0;
};

#endif

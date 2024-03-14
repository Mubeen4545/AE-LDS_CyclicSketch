#ifndef LIB_REDIS_STORAGE_H
#define LIB_REDIS_STORAGE_H

#include "_lib.h/IEVStorage.h"
#include "redis/myredis.h"

class CRedisStorageCreator
{
public:
    static IEigenvectorStorage* createRedisStorage();
};

class CRedisStorage: public IEigenvectorStorage, public myredis
{
public:    
    CRedisStorage();
    ~CRedisStorage();
public:    
    void initialPara(std::string strPara, int iPara, std::string resrPara);
    void insertEV(std::string strKey, std::string strValue);
    void saveValue(std::string strKey, std::string strValue);

};

#endif

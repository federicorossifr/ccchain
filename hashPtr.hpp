#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/crypto.h>
#include <functional>
#include <string>



template <class T>
class HashPointer {
    T* ptr;
    std::string hash;

public:
    HashPointer() {
        ptr = nullptr;
    }

    HashPointer(T* obj,std::function<bool(T* obj,std::string& hashDst)>& hashFun ): ptr(obj) {
        hashFun(obj,hash);
    }

    HashPointer(HashPointer&& hptr): ptr(hptr.ptr),hash(std::move(hptr.hash))  {}

    HashPointer(T* obj,std::string&& _hash): ptr(obj),hash(std::move(_hash)) {}

    HashPointer& operator=(HashPointer&& hptr) {
        ptr = hptr.ptr;
        hash = std::move(hptr.hash);
        return *this;
    }

    bool isTampered(std::function<bool(T* obj,std::string& hashDst)>& hashFun) {
        std::string currHash;
        hashFun(ptr,currHash);
        auto tampered = CRYPTO_memcmp(hash.c_str(),currHash.c_str(),sizeof(hash));
        return tampered;
    }
    
};



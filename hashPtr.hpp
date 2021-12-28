#include "hash.hpp"

template <class T>
class HashPointer {
    T* ptr;
    std::string hash;

public:
    HashPointer() {
        ptr = nullptr;
    }

    template <class HashFunction>
    HashPointer(T* obj,HashFunction hashFun ): ptr(obj) {
        hashFun(obj,hash);
    }

    HashPointer(HashPointer&& hptr): ptr(hptr.ptr),hash(std::move(hptr.hash))  {}

    HashPointer(T* obj,std::string&& _hash): ptr(obj),hash(std::move(_hash)) {}

    HashPointer& operator=(HashPointer<T>&& hptr) {
        ptr = hptr.ptr;
        hash = std::move(hptr.hash);
        return *this;
    }

    template <class HashFunction>
    bool isTampered(HashFunction hashFun) {
        std::string currHash;
        hashFun(ptr,currHash);
        auto tampered = CRYPTO_memcmp(hash.c_str(),currHash.c_str(),sizeof(hash));
        return tampered;
    }

    T& operator*() {
        return *ptr;
    }
    
};



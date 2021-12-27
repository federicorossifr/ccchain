#include "hashPtr.hpp"
#include <iostream>
#include <memory>
#include <iostream>
#include <iomanip>
#include <sstream>

using byte = unsigned char;

template <class T>
std::function<bool(T*,std::string&)> computeHash = [](T* clearObj, std::string& hashDst)
{
    bool success = false;
    byte* rawData = reinterpret_cast<byte*>(clearObj);
    constexpr auto objectSize = sizeof(T);
    EVP_MD_CTX* context = EVP_MD_CTX_new();

    if(context != NULL)
    {
        if(EVP_DigestInit_ex(context, EVP_sha256(), NULL))
        {
            if(EVP_DigestUpdate(context, rawData, objectSize))
            {
                unsigned char hash[EVP_MAX_MD_SIZE];
                unsigned int lengthOfHash = 0;

                if(EVP_DigestFinal_ex(context, hash, &lengthOfHash))
                {
                    std::stringstream ss;
                    for(unsigned int i = 0; i < lengthOfHash; ++i)
                    {
                        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
                    }
                    hashDst = ss.str();
                    success = true;
                }
            }
        }

        EVP_MD_CTX_free(context);
    }

    return success;
};


int main() {
    int a = 5;
    HashPointer<int> ptr(&a,computeHash<int>);
    std::cout << ptr.isTampered(computeHash<int>) << std::endl;
    a = 6;
    std::cout << ptr.isTampered(computeHash<int>) << std::endl;
}
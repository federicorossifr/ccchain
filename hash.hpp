#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/crypto.h>
#include <functional>
#include <string>
#include <memory>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <concepts>

template <class TH>
using HashFunctionBase = std::function<bool(TH*,std::string&)>;

using byte = unsigned char;

bool computeHash(byte* rawData, std::string& hashDst, const auto objectSize)
{
    bool success = false;
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


template <class ContentType>
requires std::movable<ContentType>
class ChainBlock;

template <class T, class Object>
concept ChainableBlock = requires(T o) 
{
    std::derived_from<T,ChainBlock<Object>> == true;
};

template <class ChainableBlock>
HashFunctionBase<ChainableBlock> computeChainBlockHash = [](ChainableBlock* clearObj, std::string& hashDst)
{
    bool success = false;
    byte* rawData = reinterpret_cast<byte*>(&clearObj->blockContent);
    return computeHash(rawData,hashDst, sizeof(ChainableBlock));
};

template <class T>
HashFunctionBase<T> computeBasicHash = [](T* clearObj, std::string& hashDst)
{
    bool success = false;
    byte* rawData = reinterpret_cast<byte*>(clearObj);
    return computeHash(rawData,hashDst,sizeof(T));
};

#include "hashPtr.hpp"


template <class ContentType>
requires std::movable<ContentType>
class ChainBlock {
public:
    HashPointer<ChainBlock> nextBlock;
    ContentType blockContent;

    ChainBlock() = default;

    ChainBlock(ContentType&& obj): blockContent(std::move(obj)) {}
    ChainBlock(ContentType&& obj, HashPointer<ContentType>&& ptr): nextBlock(std::move(ptr)),blockContent(std::move(obj)) {}

    void setNext(HashPointer<ChainBlock>&& ptr) {
        nextBlock = std::move(ptr);
    }

    HashPointer<ChainBlock> getHashPointer() {
        HashPointer<ChainBlock> ptr(this,computeChainBlockHash<ChainBlock<ContentType>>);
        return std::move(ptr);
    }

    ChainBlock& next() {
        return *nextBlock;
    }

};
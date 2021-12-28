#include "chainBlock.hpp"
#include <iostream>




int main() {
    int a = 6, b = 7;
    ChainBlock<int> cblock_a(std::move(a));
    ChainBlock<int> cblock_b(std::move(b));
    auto ptr = std::move(cblock_a.getHashPointer());
    cblock_b.setNext(std::move(ptr));
    auto& nextB = cblock_b.next();
    std::cout << &nextB << " " << &cblock_a << std::endl;
}
#include <iostream>
#include <assert.h>
#include "MemoryManager.h"

using namespace std;

int main()
{
    auto buffer = std::make_unique<char[]>(100);
    MemoryManager* manager = new MemoryManager (buffer.get(), 10);

    //std::cout << "free memory in Manager " << manager->get_free_size() <<std::endl;

    //we cannot allocate more than there is in the buffer
    assert(manager->Alloc(15) == nullptr);

    //let's allocate in various fragments to fully use pre-allocated buffer
    char *p1 = manager->Alloc(5);
    //std::cout << "free memory in Manager " << manager->get_free_size() <<std::endl;

    char *p2 = manager->Alloc(4);
    assert(p1 != nullptr && p2 != nullptr);

    //std::cout << "free memory in Manager " << manager->get_free_size() <<std::endl;
    assert(manager->get_free_size() == 1);

    //over provisioning returns null_ptr
    char *p3 = manager->Alloc(4);
    assert(p3 == nullptr);

    // we even do succeed to allocate 1 byte
    char *p4 = manager->Alloc(1);
    assert(manager->get_free_size() == 0);
    assert(p4 != nullptr);
    //std::cout << "free memory in Manager " << manager->get_free_size() <<std::endl;

    //next smallest allocation will fail -- there's no memory available:
    char *p5 = manager->Alloc(1);
    assert(p5 == nullptr);

    manager->Free(p2);
    manager->Free(p4);

    char* p6 = manager->Alloc(4);
    assert(p6!= nullptr);

    //following allocation will succeed
    // if and only if we can recover fragmented memory blocks
    manager->Free(p6);
    p6 = manager->Alloc(5);
    assert(p6 != nullptr);

    return 0;
}

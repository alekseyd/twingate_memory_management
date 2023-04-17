//
// Created by Aleksey Dashevsky on 17/04/2023.
//

#ifndef TWINGATE_MEMORY_MANAGEMENT_MEMORYMANAGER_H
#define TWINGATE_MEMORY_MANAGEMENT_MEMORYMANAGER_H

#include <list>
#include <unordered_map>
#include "BlockDescriptor.h"

class MemoryManager {
public:
    // buffer is a large chunk of contiguous memory.
    // num_bytes is the size of the buffer.
    MemoryManager(char * buffer, int num_bytes);

    // Allocate memory of size 'size'. Use malloc() like semantics.
    char* Alloc(int size);

    // Free up previously allocated memory.  Use free() like semantics.
    void Free(char*);

    int get_free_size() const { return free_size;}

private:
    const int buffer_size;

    int free_size = buffer_size;
    std::unordered_map<char*, int> allocated_blocks;
    std::list<BlockDescriptor> free_blocks;

    bool optimize_memory(int required_len);
};
#endif //TWINGATE_MEMORY_MANAGEMENT_MEMORYMANAGER_H
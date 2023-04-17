//
// Created by Aleksey Dashevsky on 17/04/2023.
//

#include <algorithm>
#include "MemoryManager.h"
using namespace std;

MemoryManager::MemoryManager(char * buffer, int num_bytes)
    :  buffer_size(num_bytes)
{
    free_blocks.push_front(BlockDescriptor(buffer, num_bytes));
}

char *MemoryManager::Alloc(int size) {
    if (size <=0 ){
        return nullptr;
    }

    // Find a place to allocate
    // Return nullptr whenever there's not enough continues space
    if ( free_size < size) {
        return nullptr;
    }

    list<BlockDescriptor>::iterator it = find_if (free_blocks.begin(), free_blocks.end(),
                    [&size] (BlockDescriptor &bd) { return bd.get_len() >= size; });
    if (it == free_blocks.end()) {
        //try to optimize memory and then look once again for a block of size `size`
        if (!optimize_memory(size))
            return nullptr;

        it = find_if (free_blocks.begin(), free_blocks.end(),
                      [&size] (BlockDescriptor &bd) { return bd.get_len() >= size; });
        if (it == free_blocks.end())
            return nullptr;
    }
    char* allocated_start = it->get_start();
    allocated_blocks.insert(pair<char*, int>(allocated_start, size));
    if(it->get_len() == size) {
        //remove fully used block from list of free block
        free_blocks.erase(it);
    } else {
        //otherwise shorten it from the start
        it->decrease_len(size);
    }
    free_size -= size;
    return allocated_start;
}

void MemoryManager::Free(char *start) {
    // Find block to be freed, otherwise throw
    auto it = allocated_blocks.find(start);
    if (it == allocated_blocks.end()){
        //TODO: throw, we have been given a wrong address?!
        return;
    }

    //update counters and allocated blocks
    BlockDescriptor free_bd(start, it->second);
    free_size += it->second;
    allocated_blocks.erase(it); //

    //return freshly freed block into the list
    // keep order by the block start addresses
    auto prev_jt = free_blocks.begin();
    for (auto jt = free_blocks.begin(); jt != free_blocks.end(); jt++) {
        if (jt->get_start() < start) {
            prev_jt = jt;
            continue;
        }
    }

    if (prev_jt == free_blocks.begin()){
        free_blocks.push_front(free_bd);
    }else {
        free_blocks.insert(prev_jt, free_bd);
    }
}

bool MemoryManager::optimize_memory(int required_len) {
    auto prev_jt = free_blocks.begin();
    auto jt = free_blocks.begin();
    jt++;
    //run with two pointers and join together adjacent segments
    for ( ;jt != free_blocks.end(); prev_jt = jt, jt++) {
        if(prev_jt->get_start() + prev_jt->get_len() == jt->get_start()) {
            //join the blocks
            jt->set_start(prev_jt->get_start());
            jt->increase_len(prev_jt->get_len());
            free_blocks.erase(prev_jt);
            if (jt->get_len() >= required_len) {
                return true;
            }
        }
    }
    return false;
}

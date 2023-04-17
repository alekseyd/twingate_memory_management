//
// Created by Aleksey Dashevsky on 17/04/2023.
//

#ifndef TWINGATE_MEMORY_MANAGEMENT_BLOCKDESCRIPTOR_H
#define TWINGATE_MEMORY_MANAGEMENT_BLOCKDESCRIPTOR_H


class BlockDescriptor {
    char* start;
    int len;

public:
    BlockDescriptor(int len=0)
        : start(nullptr), len(len) {}
    BlockDescriptor(char* start, int len)
        : start(start), len(len) {}
    BlockDescriptor(const BlockDescriptor &other)
            : start(other.start), len(other.len) {}

    char* get_start() const {return start;}
    void set_start(char* ptr) {start = ptr;}

    int get_len() const {return len;}
    void increase_len(int size) {len+=size;}
    void decrease_len(int size) {len -= size; start += size;}

};

#endif //TWINGATE_MEMORY_MANAGEMENT_BLOCKDESCRIPTOR_H

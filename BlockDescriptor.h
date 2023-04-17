//
// Created by Aleksey Dashevsky on 17/04/2023.
//

#ifndef TWINGATE_MEMORY_MANAGEMENT_BLOCKDESCRIPTOR_H
#define TWINGATE_MEMORY_MANAGEMENT_BLOCKDESCRIPTOR_H


class BlockDescriptor {
    char* start;
    int len;

public:
    BlockDescriptor(char* start=nullptr, int len=0)
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

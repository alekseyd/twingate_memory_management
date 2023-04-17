# Memory manager implementation by Aleksey Dashevsky

### Memory manager API
Fully based on suggested API with addition of one const function only, `int get_free_size() const` — the function returns overall amount of memory still available for allocation
  ```c++
  class MemoryManager {
  public:
  // buffer is a large chunk of contiguous memory.
  // num_bytes is the size of the buffer.
  MemoryManager(char * buffer, int num_bytes);

  // Allocate memory of size 'size'. Use malloc() like semantics.
  char* Alloc(int size);

  // Free up previously allocated memory.  Use free() like semantics.
  void Free(char*);
  };
```

### Implementation
C++ with basic STL containers and allocators, build with CMake, CLion IDE.

### Testing
You will find one inside `main()`, I used only native `assert` call

### Internally used data structures:
Along with memory buffer I keep two more structure:
- Ordered list of all free memory blocks. The ordering is based on a start address of every block
- Unordered map of all allocated blocks currently in use

Every block is represented by pair of start address and its length accompanied by constructor and necessary getters and setters
```c++

class BlockDescriptor {
    char* start;
    int len;
...     
}
```

###  Algorithm:
1) MemoryManager is initialized with empty map of used `Blocks` and list of free memory blocks, filled with only one block that equals initial buffer
2) Every time customer requests new mem block with `Alloc(size)`, we scan the list of free memory blocks, and when we find the first unused block large enough to contain keep memory segment of required size, we split aforementioned block into two:
   * lower part of the block of **size** will be returned to user, and we also mark the allocated block with relevant `BlockDescriptor` in the map of allocated blocks
   * the rest of the free block will be reduced by **size**, of course we keep this `BlockDescriptor` in the list, unless its size gets to zero 
3) If during execution of `Alloc(size)` we are unable to find block of appropriate **size**, we try to defragment free memory by rejoining adjacent unused memory blocks with call `optimize_memory()`
4) Upon call to `Free(addr*)` we do the reverse to 2: we throw away `BlockDescriptor` representing the block to be released from map of used blocks and instead place the `BlockDescriptor` into the list of unused blocks
5) `optimize_memory()` implements the simple algorithm where we scan the list of free blocks (remember, the blocks in the list are ordered by their start address), and whenever we find two adjacent blocks, we replace them with one block that cover them both. In practice, we throw the first `BlockDescriptor`, and the second is enlarged to the combine size while the start address of the first block becomes start address of new combined block. Algorithm stops whenever we find block large enough to cover the requested block size
6) If the tricks described in 2.-3.-5 fail to find and produce a requested  block, we return `nullptr` to the user

### What is missing/can be improved
Our defragmenation policy is very limited. To improve it we might want to move around the allocated and the free blocks (say, by collecting all the allocated blocks at the beginning of the buffer, thus ultimately moving all unused spaces into one contiguous free block).
The drawback of this solution is that all the pointer known to the user will render invalid. To overcome this problem we can:
1) make return a handle to memory block instead of pointer:
    ```c++
    Handle Alloc(size)
    ```
    the `Handle` value can be then used to get access to the memory block address by calling something like
    ```c++
    char* HandleToPtr(Handle)
    ```
    (handle can be a unique number used as a key in a map to get actual pointer). Obviously, user cannot store the actual pointer for a later use 
2) alternative solution would be to return kind of smart pointer, which envelopes actual pointer. Such smart pointer would be safe to store on the user side given that user performs dereference of such smart pointer

### Multithreading
Alas, the solution is not multithreading friendly, and would require adding locking mechanisms on top the described earlier additional level of indirection. For coarse grained granularity, we would want to serialize access to the functions `Alloc`, `Free` and `HandleToPtr`


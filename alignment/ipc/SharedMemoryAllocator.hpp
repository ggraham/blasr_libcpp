#ifndef _BLASR_SHARED_MEMORY_ALLOCATOR_HPP_
#define _BLASR_SHARED_MEMORY_ALLOCATOR_HPP_

#include <cerrno>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <string>

template <typename T_Data>
int AllocateMappedShare(std::string &handle, int dataLength, T_Data *&dataPtr, int &shmId)
{
    std::cout << "opening shm" << std::endl;
    shmId = shm_open(handle.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (ftruncate(shmId, sizeof(T_Data) * dataLength) == -1) {
        std::cout << " ftruncate error: " << errno << std::endl;
    }
    std::cout << "done truncating." << std::endl;
    dataPtr = (T_Data *)mmap(NULL, sizeof(T_Data) * dataLength, PROT_READ | PROT_WRITE, MAP_SHARED,
                             shmId, 0);
    if (dataPtr == MAP_FAILED) {
        //
        // Handle this better later on.
        //
        std::cout << "ERROR, MEMORY MAP FAILED." << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "done mapping." << std::endl;
    return dataLength;
}

#endif  // _BLASR_SHARED_MEMORY_ALLOCATOR_HPP_

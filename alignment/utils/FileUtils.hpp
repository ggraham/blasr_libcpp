#ifndef _BLASR_FILE_UTILS_HPP_
#define _BLASR_FILE_UTILS_HPP_

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>  // for lseek
#include <unistd.h>     // for lseek
#include <cstdio>

#include <fstream>
#include <iostream>
#include <string>

bool FileExists(std::string &fileName);

void CriticalOpenRead(std::string &fileName, std::ifstream &file,
                      std::ios::openmode mode = std::ios::in);

int OpenRead(std::string &fileName, std::ifstream &file, std::ios::openmode mode = std::ios::in);

void CriticalOpenWrite(std::string &fileName, std::ofstream &file,
                       std::ios::openmode mode = std::ios::out);

int OpenWrite(std::string &fileName, std::ofstream &file, std::ios::openmode mode = std::ios::out);

int CountLinesInFile(std::string fileName);

#endif

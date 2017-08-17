#ifndef SIMULATOR_OUTPUT_SAMPLE_LIST_SET_H_
#define SIMULATOR_OUTPUT_SAMPLE_LIST_SET_H_

#include <iostream>
#include <map>
#include <string>

#include "../../pbdata/utils.hpp"
#include "OutputSampleList.hpp"

typedef std::map<std::string, OutputSampleList> OutputSampleListMap;
class OutputSampleListSet
{
public:
    OutputSampleListMap listMap;
    std::vector<std::string> keys;
    int keyLength;
    int nSufficient;
    int sampleSpaceSize;
    int keySize;
    int minSamples;
    int maxSamples;
    std::vector<int> lengths;
    OutputSampleListSet(int keySizeP)
    {
        minSamples = 500;
        maxSamples = 2000;
        nSufficient = 0;
        keySize = keySizeP;
        sampleSpaceSize = 1 << (2 * keySize);
    }

    void Write(std::ofstream &out)
    {
        // Say how many elements to write.
        OutputSampleListMap::iterator mapIt;
        int setSize = listMap.size();
        out.write((char *)&setSize, sizeof(int));
        int keySize = 0;
        // Say how large each element is.
        if (listMap.size() > 0) {
            keySize = listMap.begin()->first.size();
        }
        out.write((char *)&keySize, sizeof(int));

        for (mapIt = listMap.begin(); mapIt != listMap.end(); ++mapIt) {
            std::string mapItKey = mapIt->first;
            out.write((char *)mapItKey.c_str(), sizeof(char) * mapItKey.size());
            mapIt->second.Write(out);
        }
        int numLengths = lengths.size();
        out.write((char *)&numLengths, sizeof(int));
        for (size_t i = 0; i < lengths.size(); i++) {
            out.write((char *)&lengths[i], sizeof(int));
        }
    }

    void Read(std::string &inName)
    {
        std::ifstream in;
        CrucialOpen(inName, in, std::ios::in | std::ios::binary);
        Read(in);
        in.close();
    }

    void Read(std::ifstream &in)
    {
        int setSize;
        in.read((char *)&setSize, sizeof(int));
        in.read((char *)&keyLength, sizeof(int));

        if (keyLength == 0 or setSize == 0) {
            return;
        }
        char *key = ProtectedNew<char>(keyLength + 1);
        key[keyLength] = '\0';
        int i;
        for (i = 0; i < setSize; i++) {
            in.read(key, sizeof(char) * keyLength);
            listMap[key].Read(in);
        }
        int numLengths;
        in.read((char *)&numLengths, sizeof(int));
        if (numLengths > 0) {
            lengths.resize(numLengths);
        }
        for (i = 0; i < numLengths; i++) {
            in.read((char *)&lengths[i], sizeof(int));
        }
        if (key) {
            delete[] key;
            key = NULL;
        }
    }

    void AppendOutputSample(std::string key, OutputSample &sample)
    {
        if (static_cast<int>(listMap[key].size()) < minSamples) {
            if (static_cast<int>(listMap[key].size()) < maxSamples) {
                listMap[key].push_back(sample);
            }
            if (static_cast<int>(listMap[key].size()) == minSamples) {
                nSufficient++;
                std::cout << nSufficient << " / " << sampleSpaceSize << std::endl;
            }
        }
    }

    bool Sufficient() { return nSufficient == sampleSpaceSize; }

    void SampleRandomSample(std::string key, OutputSample &sample)
    {
        if (listMap.find(key) == listMap.end()) {
            std::cout << listMap.size() << std::endl;
            std::cout << "ERROR, " << key << " is not a sampled context." << std::endl;
            for (size_t i = 0; i < key.size(); i++) {
                char c = toupper(key[i]);
                if (c != 'A' and c != 'C' and c != 'G' and c != 'T') {
                    std::cout << "The nucleotide " << c << " is not supported." << std::endl;
                }
            }
            std::exit(EXIT_FAILURE);
        }
        sample = listMap[key][RandomInt(listMap[key].size())];
    }
};

#endif

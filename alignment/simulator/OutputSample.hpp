#ifndef _SIMULATOR_OUTPUT_SAMPLE_HPP_
#define _SIMULATOR_OUTPUT_SAMPLE_HPP_

#include "../../pbdata/SMRTSequence.hpp"
#include "QualitySample.hpp"

class OutputSample
{
public:
    enum Type
    {
        Match,
        Insertion,
        Deletion,
        Substitution
    };

    std::vector<QualitySample> qualities;
    std::vector<Nucleotide> nucleotides;
    Type type;

    void Resize(int size)
    {
        qualities.resize(size);
        nucleotides.resize(size);
    }

    void CopyFromSeq(SMRTSequence &seq, int pos, int length = 1)
    {
        Resize(length);
        int i;
        for (i = 0; i < length; i++) {
            qualities[i].CopyFromSequence(seq, pos + i);
            nucleotides[i] = seq.seq[pos + i];
        }
    }

    void Write(std::ofstream &out)
    {

        out.write((char *)&type, sizeof(type));
        unsigned nNuc = nucleotides.size();

        out.write((char *)&nNuc, sizeof(unsigned));
        for (size_t i = 0; i < qualities.size(); i++) {
            qualities[i].Write(out);
        }
        assert(nNuc == qualities.size());
        out.write((char *)&nucleotides[0], sizeof(Nucleotide) * nucleotides.size());
    }

    void Read(std::ifstream &in)
    {
        in.read((char *)&type, sizeof(Type));
        int nNuc;
        in.read((char *)&nNuc, sizeof(int));
        qualities.resize(nNuc);
        int i;
        for (i = 0; i < nNuc; i++) {
            qualities[i].Read(in);
        }
        nucleotides.resize(nNuc);
        in.read((char *)&nucleotides[0], sizeof(Nucleotide) * nNuc);
    }
};

#endif

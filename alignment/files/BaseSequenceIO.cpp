#include "BaseSequenceIO.hpp"
#include <cstdlib>

void BaseSequenceIO::SetFiles(FileType &pFileType, std::string &pFileName)
{
    fileType = pFileType;
    fileName = pFileName;
}

FileType BaseSequenceIO::GetFileType() { return fileType; }

int BaseSequenceIO::DetermineFileTypeByExtension(std::string &fileName, FileType &type,
                                                 bool exitOnFailure)
{

    std::string::size_type dotPos = fileName.rfind(".");
    if (dotPos != std::string::npos) {
        std::string extension;
        extension.assign(fileName, dotPos + 1, fileName.size() - (dotPos + 1));
        if (extension == "fasta" || extension == "fa" || extension == "fas" ||
            extension == "fsta" || extension == "screen") {
            type = FileType::Fasta;
            return 1;
        } else if (extension == "h5") {
            dotPos = fileName.rfind(".", dotPos - 1);
            extension.assign(fileName, dotPos + 1, fileName.size() - (dotPos + 1));
            if (extension == "pls.h5" || extension == "plx.h5") {
                type = FileType::HDFPulse;
                return 1;
            } else if (extension == "bas.h5" || extension == "bax.h5") {
                type = FileType::HDFBase;
                return 1;
            } else if (extension == "ccs.h5") {
                type = FileType::HDFCCSONLY;
                return 1;
            } else {
                type = FileType::None;
                return 0;
            }
        } else if (extension == "fastq" || extension == "fq") {
            type = FileType::Fastq;
            return 1;
        } else if (extension == "4bit" || extension == "fourbit") {
            type = FileType::Fourbit;
            assert("Four bit reading is not yet implemented for the reader agglomerate!" == 0);
            return 1;
        } else if (extension == "bam") {
            type = FileType::PBBAM;
            return 1;
        } else if (extension == "xml") {
            type = FileType::PBDATASET;
            return 1;
        } else {
            type = FileType::None;
            if (exitOnFailure) {
                std::cout << "ERROR, file type '." << extension
                     << "' is not understood to be one of pls.h5, fasta, fastq, nor bam. " << std::endl;
                std::exit(EXIT_FAILURE);
            }
            return 0;
        }
        return 0;
    }
    return 0;
}

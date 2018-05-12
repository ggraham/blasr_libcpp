#include <pbdata/Types.h>
#include <pbdata/MD5Utils.hpp>
#include <pbdata/StringUtils.hpp>

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

int ExactPatternMatch(std::string orig, std::string pattern)
{
    std::string::size_type pos = orig.find(pattern);
    if (pos == orig.npos) {
        return 0;
    } else {
        return 1;
    }
}

void MakeMD5(const char *data, unsigned int dataLength, std::string &md5Str, int nChars)
{

    MD5 md5engine;
    md5engine.update((unsigned char *)data, dataLength);
    md5engine.finalize();

    char *md5c_str = md5engine.hex_digest();
    assert(md5c_str != NULL);
    if (nChars == 0) {
        nChars = 32;
    }
    md5Str.assign(md5c_str, nChars);
    delete[] md5c_str;
}

void MakeMD5(std::string &data, std::string &md5Str, int nChars)
{
    MakeMD5(data.c_str(), data.size(), md5Str, nChars);
}

int IsWhitespace(char c) { return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\0'); }

int IsSpace(char c) { return (c == ' ' || c == '\t'); }

size_t ToWords(std::string &orig, std::vector<std::string> &words)
{
    size_t curWordStart, curWordEnd;
    curWordStart = 0;
    while (curWordStart < orig.size()) {
        while (curWordStart < orig.size() and IsSpace(orig[curWordStart])) {
            curWordStart++;
        }
        curWordEnd = curWordStart;
        while (curWordEnd < orig.size() and !IsSpace(orig[curWordEnd])) {
            curWordEnd++;
        }
        std::string word;
        if (curWordEnd != curWordStart) {
            word.assign(orig, curWordStart, curWordEnd - curWordStart);
            words.push_back(word);
        }
        curWordStart = curWordEnd;
    }
    return words.size();
}

// Splice a string by pattern and save to a vector of token strings.
int Splice(const std::string &orig, const std::string &pattern, std::vector<std::string> &tokens)
{
    assert(pattern.size() > 0);

    tokens.clear();
    size_t search_start = 0;
    size_t find_pos = orig.find(pattern, search_start);
    while (find_pos != std::string::npos) {
        std::string x = orig.substr(search_start, find_pos - search_start);
        tokens.push_back(x);
        search_start = find_pos + pattern.size();
        find_pos = orig.find(pattern, search_start);
    }
    tokens.push_back(orig.substr(search_start));
    return tokens.size();
}

void ParseSeparatedList(const std::string &csl, std::vector<std::string> &values, char delim)
{
    std::stringstream cslStrm(csl);
    std::string valString;
    std::string next;
    do {
        if (std::getline(cslStrm, valString, delim)) {
            if (valString.size() > 0) {
                values.push_back(valString);
            }
        }
    } while (cslStrm);
}

int AssignUntilFirstSpace(char *orig, int origLength, std::string &result)
{
    int i;
    for (i = 0; i < origLength; i++) {
        if (orig[i] == ' ' || orig[i] == '\t' || orig[i] == '\n' || orig[i] == '\r' ||
            orig[i] == '\0') {
            break;
        }
    }
    result.assign(orig, i);
    return i;
}

std::string RStrip(std::string &fileName)
{
    // Remove right-ended spaces
    int i = fileName.size();
    if (i == 0) {
        return "";
    }
    while (i >= 1) {
        i--;
        if (not IsWhitespace(fileName[i])) {
            break;
        }
    }
    return fileName.substr(0, i + 1);
}

std::string MakeReadGroupId(const std::string &movieName, const ReadType::ReadTypeEnum &readType)
{
    // PBBAM spec 3.0b5:
    // Read Group Id is computed as MD5(${movieName}//${readType})[0:8], where
    // movieName is PacBio platform unit id, e.g., (m140905_042...77_s1_X0),
    // readtype is SUBREAD, CCS or UNKNOWN,
    // CCS reads for a movie named "movie32" would have
    // RGID STRING = "f5b4ffb6"
    std::string seed = movieName + "//" + ReadType::ToString(readType);
    std::string readGroupId;
    MakeMD5(seed, readGroupId, 8);
    return readGroupId;
}

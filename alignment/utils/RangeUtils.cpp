#include <alignment/utils/RangeUtils.hpp>

#include <stdexcept>

Range::Range(UInt pStart) { start = end = pStart; }
Range::Range(UInt pStart, UInt pEnd)
{
    start = pStart;
    end = pEnd;
    if (start > end) {
        std::cout << "ERROR: start of a range should be less than the end." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

bool Range::contains(const UInt& query) { return (start <= query && query <= end); }
bool Range::operator<(const Range& pRange) const
{
    if (start == pRange.start) {
        return (end > pRange.end);
    }
    return (start < pRange.start);
}

//
// Input is a comma-delimited string of ranges.
// e.g. 1,2,3,10-20
bool ParseRanges(std::string& rangesStr, std::vector<Range>& ranges)
{
    ranges.clear();
    bool parseSucceed = true;
    std::vector<std::string> strList;
    ParseSeparatedList(rangesStr, strList, ',');
    for (int i = 0; i < int(strList.size()); i++) {
        std::string& str = strList[i];
        if (str.find('-') == std::string::npos) {
            ranges.push_back(Range(std::atoi(str.c_str())));
        } else {
            std::vector<std::string> start_end;
            ParseSeparatedList(str, start_end, '-');
            if (start_end.size() != 2) {
                parseSucceed = false;
                break;
            }
            ranges.push_back(Range(std::atoi(start_end[0].c_str()), atoi(start_end[1].c_str())));
        }
    }
    if (parseSucceed) {
        std::sort(ranges.begin(), ranges.end());
    } else {
        ranges.clear();
    }
    return parseSucceed;
}

Ranges::Ranges(std::string rangesStr)
{
    if (!ParseRanges(rangesStr, ranges)) throw std::invalid_argument("bad range");
}

bool Ranges::setRanges(std::string rangesStr) { return ParseRanges(rangesStr, ranges); }

int Ranges::size() { return ranges.size(); }

UInt Ranges::max()
{
    if (size() == 0) {
        std::cout << "ERROR, could not determine the maximum value "
                  << "of an empty Ranges object." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return ranges.back().end;
}

bool Ranges::contains(const UInt& query)
{
    if (ranges.size() == 0) return false;
    std::vector<Range> searchRanges;
    searchRanges.push_back(Range(0, ranges.size() - 1));
    while (searchRanges.size() > 0) {
        Range searchRange = searchRanges.back();
        searchRanges.pop_back();
        UInt mid = (searchRange.start + searchRange.end) / 2;
        if (ranges[mid].contains(query)) {
            return true;
        }
        if (mid > 0 && searchRange.start <= mid - 1) {
            searchRanges.push_back(Range(searchRange.start, mid - 1));
        }
        if (ranges[mid].start <= query && searchRange.end >= mid + 1) {
            searchRanges.push_back(Range(mid + 1, searchRange.end));
        }
    }
    return false;
}

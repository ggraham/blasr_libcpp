#include <LibBlasrConfig.h>

#ifdef USE_PBBAM

#include <alignment/query/SequentialZmwGroupQuery.h>

#include <pbbam/CompositeBamReader.h>
#include <boost/optional.hpp>

#include <cassert>

using namespace PacBio;
using namespace PacBio::BAM;
using namespace PacBio::BAM::internal;

struct SequentialZmwGroupQuery::SequentialZmwGroupQueryPrivate
{
public:
    SequentialZmwGroupQueryPrivate(const DataSet& dataset)
        : reader_(new SequentialCompositeBamReader(dataset)), nextRecord_(boost::none)
    {
    }

    bool GetNext(std::vector<BamRecord>& records)
    {
        records.clear();

        std::string movieName;
        int32_t holeNumber = -1;

        if (nextRecord_.is_initialized()) {
            BamRecord r = nextRecord_.get();
            movieName = r.MovieName();
            holeNumber = r.HoleNumber();
            records.push_back(std::move(r));
            nextRecord_ = boost::none;
        }

        BamRecord record;
        while (reader_->GetNext(record)) {
            if (records.empty()) {
                movieName = record.MovieName();
                holeNumber = record.HoleNumber();
                records.push_back(record);
            } else {
                assert(!records.empty());
                if (record.MovieName() == movieName and record.HoleNumber() == holeNumber)
                    records.push_back(record);
                else {
                    nextRecord_ = record;
                    return true;
                }
            }
        }
        return !records.empty();
    }

public:
    std::unique_ptr<SequentialCompositeBamReader> reader_;

    boost::optional<BamRecord> nextRecord_;
};

SequentialZmwGroupQuery::SequentialZmwGroupQuery(const DataSet& dataset)
    : internal::IGroupQuery(), d_(new SequentialZmwGroupQueryPrivate(dataset))
{
}

SequentialZmwGroupQuery::~SequentialZmwGroupQuery(void) {}

bool SequentialZmwGroupQuery::GetNext(std::vector<BamRecord>& records)
{
    return d_->GetNext(records);
}

#endif

#include <LibBlasrConfig.h>

#ifdef USE_PBBAM

#include <alignment/query/PbiFilterZmwGroupQuery.h>

#include <pbbam/CompositeBamReader.h>
#include <boost/optional.hpp>

#include <cassert>

using namespace PacBio;
using namespace PacBio::BAM;
using namespace PacBio::BAM::internal;

struct PbiFilterZmwGroupQuery::PbiFilterZmwGroupQueryPrivate
{
    /// TODO: To ensure that BamRecords of a zmw are always stored
    /// sequentially in bam files, such as in movie.subreads.bam.
public:
    PbiFilterZmwGroupQueryPrivate(const PbiFilter& filter, const DataSet& dataset)
        : reader_(new PbiFilterCompositeBamReader<Compare::Zmw>(filter, dataset))
        , nextRecord_(boost::none)
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
    std::unique_ptr<PbiFilterCompositeBamReader<Compare::Zmw>> reader_;

    boost::optional<BamRecord> nextRecord_;
};

PbiFilterZmwGroupQuery::PbiFilterZmwGroupQuery(const DataSet& dataset)
    : internal::IGroupQuery()
    , d_(new PbiFilterZmwGroupQueryPrivate(PbiFilter::FromDataSet(dataset), dataset))
{
}

PbiFilterZmwGroupQuery::PbiFilterZmwGroupQuery(const PbiFilter& filter, const DataSet& dataset)
    : internal::IGroupQuery(), d_(new PbiFilterZmwGroupQueryPrivate(filter, dataset))
{
}

PbiFilterZmwGroupQuery::~PbiFilterZmwGroupQuery(void) {}

bool PbiFilterZmwGroupQuery::GetNext(std::vector<BamRecord>& records)
{
    return d_->GetNext(records);
}
#endif

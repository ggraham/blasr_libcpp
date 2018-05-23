#include <alignment/statistics/LookupAnchorDistribution.hpp>

int LookupAnchorDistribution(int readLength, int minMatchLength, int accuracy, float &mn,
                             float &sdn, float &mnab, float &sdnab)
{

    int kIndex, accIndex, lengthIndex;
    int returnValue = 0;

    // Major index is by accuracy
    if (accuracy < PacBio::AnchorDistributionTable::anchorReadAccuracies[0]) {
        returnValue = -2;
        accuracy = PacBio::AnchorDistributionTable::anchorReadAccuracies[0];
    } else if (accuracy >= PacBio::AnchorDistributionTable::anchorReadAccuracies[1]) {
        returnValue = 2;
        accuracy = PacBio::AnchorDistributionTable::anchorReadAccuracies[1] -
                   PacBio::AnchorDistributionTable::anchorReadAccuracies[2];
    }

    accIndex = (((int)accuracy) - PacBio::AnchorDistributionTable::anchorReadAccuracies[0]) /
               PacBio::AnchorDistributionTable::anchorReadAccuracies[2];

    // middle index is by k
    if (minMatchLength < PacBio::AnchorDistributionTable::anchorMinKValues[0]) {
        returnValue = -1;  // signal too low
        minMatchLength = PacBio::AnchorDistributionTable::anchorMinKValues[0];
    } else if (minMatchLength >= PacBio::AnchorDistributionTable::anchorMinKValues[1]) {
        returnValue = 1;  // signal too high
        minMatchLength = PacBio::AnchorDistributionTable::anchorMinKValues[1] -
                         PacBio::AnchorDistributionTable::anchorMinKValues[2];  // max match length
    }

    kIndex = (minMatchLength - PacBio::AnchorDistributionTable::anchorMinKValues[0]) /
             PacBio::AnchorDistributionTable::anchorMinKValues[2];

    // last index is by read length
    if (readLength < PacBio::AnchorDistributionTable::anchorReadLengths[0]) {
        returnValue = -3;
        readLength = PacBio::AnchorDistributionTable::anchorReadLengths[0];
    } else if (readLength >= PacBio::AnchorDistributionTable::anchorReadLengths[1]) {
        returnValue = 3;
        readLength = PacBio::AnchorDistributionTable::anchorReadLengths[1] -
                     PacBio::AnchorDistributionTable::anchorReadLengths[2];  // max read length
    }

    lengthIndex = (readLength - PacBio::AnchorDistributionTable::anchorReadLengths[0]) /
                  PacBio::AnchorDistributionTable::anchorReadLengths[2];

    int nLengths = (PacBio::AnchorDistributionTable::anchorReadLengths[1] -
                    PacBio::AnchorDistributionTable::anchorReadLengths[0]) /
                   PacBio::AnchorDistributionTable::anchorReadLengths[2];
    int nAnchors = (PacBio::AnchorDistributionTable::anchorMinKValues[1] -
                    PacBio::AnchorDistributionTable::anchorMinKValues[0]) /
                   PacBio::AnchorDistributionTable::anchorMinKValues[2];
    int index = accIndex * (nLengths * nAnchors) + kIndex * nLengths + lengthIndex;

    mn = PacBio::AnchorDistributionTable::meanNumAnchors[index];
    sdn = PacBio::AnchorDistributionTable::sdNumAnchors[index];
    mnab = PacBio::AnchorDistributionTable::meanNumAnchorBases[index];
    sdnab = PacBio::AnchorDistributionTable::sdNumAnchorBases[index];

    return returnValue;
}

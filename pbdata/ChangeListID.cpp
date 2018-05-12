#include <pbdata/ChangeListID.hpp>

#include <sstream>

ChangeListID::ChangeListID() {}
ChangeListID::ChangeListID(std::string &idStringP) { StoreString(idStringP); }

void ChangeListID::StoreString(std::string &idStringP)
{
    idString = idStringP;
    std::stringstream ss(idString);
    std::string part;
    intVer.clear();
    while (std::getline(ss, part, '.')) {
        intVer.push_back(std::atoi(part.c_str()));
    }
}

int ChangeListID::LessThan(ChangeListID &rhs, int depth)
{
    if (depth == 0) {
        depth = std::min(intVer.size(), rhs.intVer.size());
    }
    int i;
    for (i = 0; i < depth; i++) {
        if (intVer[i] != rhs.intVer[i]) {
            return intVer[i] < rhs.intVer[i];
        }
    }
    return 0;  // making it here they are equal
}

QVScale ChangeListID::DetermineQVScaleFromChangeListID()
{
    ChangeListID phredCL;
    phredCL.intVer.resize(3);
    phredCL.intVer[0] = 1;
    phredCL.intVer[1] = 2;
    phredCL.intVer[2] = 2;
    if (LessThan(phredCL)) {
        return POverOneMinusP;
    } else {
        return PHRED;
    }
}

// utility method
void AppendPerforceChangelist(std::string perforceVersionString, std::string &version)
{
    if (perforceVersionString.size() > 12) {
        version.insert(version.size(), ".");
        version.insert(version.size(), perforceVersionString, 9, perforceVersionString.size() - 11);
    }
}

std::string ChangeListID::GetVersion()
{
    std::stringstream ss;
    if (intVer.size() == 1) {
        ss << intVer[0];
    } else if (intVer.size() >= 2) {
        ss << intVer[0] << "." << intVer[1];
    }
    return ss.str();
}

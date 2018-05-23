#include <hdf/HDFAlnGroupGroup.hpp>

bool HDFAlnGroupGroup::Create(HDFGroup &parent)
{
    parent.AddGroup("AlnGroup");
    if (alnGroup.Initialize(parent.group, "AlnGroup") == 0) {
        return 0;
    }
    idArray.Create(alnGroup, "ID");
    pathArray.Create(alnGroup, "Path");
    return true;
}

int HDFAlnGroupGroup::AddPath(std::string path)
{
    pathArray.Write(&path, 1);
    unsigned int id = pathArray.size();
    idArray.Write(&id, 1);
    int size = pathArray.size();
    return size;
}

int HDFAlnGroupGroup::Initialize(HDFGroup &parent)
{
    if (alnGroup.Initialize(parent.group, "AlnGroup") == 0) {
        std::cout << "ERROR, could not open /AlnGroup group." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if (idArray.Initialize(alnGroup, "ID") == 0) {
        std::cout << "ERROR, could not open /AlnGroup/ID." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if (pathArray.Initialize(alnGroup, "Path") == 0) {
        std::cout << "ERROR, could not open /AlnGroup/Path." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return 1;
}

void HDFAlnGroupGroup::Read(AlnGroup &aln)
{
    UInt nRow = idArray.size();
    if (nRow > 0) {
        aln.id.resize(nRow);
        idArray.Read(0, nRow, &aln.id[0]);
        aln.path.resize(nRow);
        unsigned int i;
        for (i = 0; i < nRow; i++) {
            pathArray.Read(i, i + 1, &aln.path[i]);
        }
    }
}

HDFAlnGroupGroup::~HDFAlnGroupGroup() { alnGroup.Close(); }

#ifndef GRAPH_PAPER_HPP_
#define GRAPH_PAPER_HPP_

#include "../../../pbdata/matrix/FlatMatrix.hpp"
#include "../../datastructures/alignment/Path.h"

template <typename T_Point>
bool SetBounds(std::vector<T_Point> &points, DNALength &minPos, DNALength &maxPos, int axis);

inline int GetIndex(DNALength pos, DNALength minPos, DNALength maxPos, int nBins);

template <typename T_Point>
int GraphPaper(std::vector<T_Point> &points, int nRows, int nCols, FlatMatrix2D<int> &bins,
               FlatMatrix2D<int> &scoreMat, FlatMatrix2D<Arrow> &pathMat, std::vector<bool> &onOptPath);

template <typename T_Point>
void RemoveOffOpt(std::vector<T_Point> &points, std::vector<bool> &optPath);

#include "GraphPaperImpl.hpp"

#endif

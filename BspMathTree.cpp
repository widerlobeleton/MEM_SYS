#include "BspMathTree.h"

FBspMathTree::FBspMathTree(int32 InX, int32 InY, int32 InWidth, int32 InHeight)
    : X(InX), Y(InY), Width(InWidth), Height(InHeight), LeftChild(nullptr), RightChild(nullptr)
{
}

FBspMathTree::~FBspMathTree()
{
    // C++ estándar limpiará las ramas hijas por la recursión de los TSharedPtr
}

bool FBspMathTree::IsLeaf() const
{
    return !LeftChild.IsValid() && !RightChild.IsValid();
}

bool FBspMathTree::SplitWithDynamicPivot(int32 MinRange, int32 MaxRange, FRandomStream& Stream)
{
    if (!IsLeaf()) return false;

    int32 TargetPivotSize = Stream.RandRange(MinRange, MaxRange);
    if (TargetPivotSize < MIN_HARD_LIMIT) TargetPivotSize = MIN_HARD_LIMIT;

    bool bCutX = Stream.RandRange(0, 1) == 0;

    if (bCutX && Width < (TargetPivotSize * 2)) return false;
    if (!bCutX && Height < (TargetPivotSize * 2)) return false;

    if (bCutX)
    {
        int32 MaxSplit = Width - TargetPivotSize;
        int32 RelativeSplitPoint = Stream.RandRange(TargetPivotSize, MaxSplit);

        LeftChild = TMakeShared<FBspMathTree>(X, Y, RelativeSplitPoint, Height);
        RightChild = TMakeShared<FBspMathTree>(X + RelativeSplitPoint, Y, Width - RelativeSplitPoint, Height);
    }
    else
    {
        int32 MaxSplit = Height - TargetPivotSize;
        int32 RelativeSplitPoint = Stream.RandRange(TargetPivotSize, MaxSplit);

        LeftChild = TMakeShared<FBspMathTree>(X, Y, Width, RelativeSplitPoint);
        RightChild = TMakeShared<FBspMathTree>(X, Y + RelativeSplitPoint, Width, Height - RelativeSplitPoint);
    }

    return true;
}
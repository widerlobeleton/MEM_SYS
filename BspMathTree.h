#pragma once

#include "CoreMinimal.h"
#include "Math/RandomStream.h"
#include "Templates/SharedPointer.h"
#include "BspInfiniteTypes.h" // Estructuras de paso de datos llanos USTRUCT

/** Clase pura de C++ para el procesamiento matemático del BSP */
class FBspMathTree : public TSharedFromThis<FBspMathTree>
{
public:
    int32 X;
    int32 Y;
    int32 Width;
    int32 Height;

    TSharedPtr<FBspMathTree> LeftChild;
    TSharedPtr<FBspMathTree> RightChild;

    static const int32 MIN_HARD_LIMIT = 15;

    // Constructor estándar de C++
    FBspMathTree(int32 InX, int32 InY, int32 InWidth, int32 InHeight);
    
    // Destructor (Al usar TSharedPtr los hijos se limpian automáticamente)
    ~FBspMathTree();

    bool IsLeaf() const;

    /** Capa 1: Pivote Dinámico en C++ puro */
    bool SplitWithDynamicPivot(int32 MinRange, int32 MaxRange, FRandomStream& Stream);

    /** Capa 2: Cálculos geométricos en memoria */
    FXZRoomDesign GetRoomGeometry(float WallThickness) const;
    FChunkRoomData ProcessRoomLogicAndFlags(float WallThickness, FRandomStream& Stream) const;
};
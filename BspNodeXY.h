#pragma once

#include "CoreMinimal.h"
#include "Math/RandomStream.h"
#include "Templates/SharedPointer.h"
#include "BspInfiniteTypes.h"

class YOURPROJECT_API FBspNodeXY : public TSharedFromThis<FBspNodeXY>
{
public:
    // Coordenadas abstractas en la rejilla XY
    int32 X;
    int32 Y;
    int32 Width;
    int32 Height;

    // Punteros inteligentes del árbol binario
    TSharedPtr<FBspNodeXY> LeftChild;
    TSharedPtr<FBspNodeXY> RightChild;

    static const int32 MIN_HARD_LIMIT = 15;

    FBspNodeXY(int32 InX, int32 InY, int32 InWidth, int32 InHeight);
    bool IsLeaf() const;

    /** Capa 1: Tu lógica de cortes con Pivote Dinámico en el plano XY (Z constante) */
    bool SplitWithDynamicPivotXY(int32 MinRange, int32 MaxRange, FRandomStream& Stream);

    /** Capa 2: Función interna que traduce la rejilla abstracta a centímetros reales de Unreal */
    FXZRoomDesign GetRoomGeometry(float WallThickness) const;

    /** Capa 2: Procesa la geometría y empaqueta las caras y los flags decorativos con cohesión matemática */
    FChunkRoomData ProcessRoomLogicAndFlags(float WallThickness, FRandomStream& Stream) const;
};
#include "BspNodeXY.h"

FBspNodeXY::FBspNodeXY(int32 InX, int32 InY, int32 InWidth, int32 InHeight)
    : X(InX), Y(InY), Width(InWidth), Height(InHeight), LeftChild(nullptr), RightChild(nullptr)
{
}

bool FBspNodeXY::IsLeaf() const
{
    return !LeftChild.IsValid() && !RightChild.IsValid();
}

bool FBspNodeXY::SplitWithDynamicPivotXY(int32 MinRange, int32 MaxRange, FRandomStream& Stream)
{
    if (!IsLeaf()) return false;

    int32 TargetPivotSize = Stream.RandRange(MinRange, MaxRange);
    if (TargetPivotSize < MIN_HARD_LIMIT) TargetPivotSize = MIN_HARD_LIMIT;

    // Decidir aleatoriamente el eje de corte en el plano del suelo: true = X, false = Y
    bool bCutX = Stream.RandRange(0, 1) == 0;

    // Tu condición de parada prematura por tamaño del pivote aleatorio
    if (bCutX && Width < (TargetPivotSize * 2)) return false;
    if (!bCutX && Height < (TargetPivotSize * 2)) return false;

    if (bCutX)
    {
        int32 MaxSplit = Width - TargetPivotSize;
        int32 RelativeSplitPoint = Stream.RandRange(TargetPivotSize, MaxSplit);

        LeftChild = TMakeShared<FBspNodeXY>(X, Y, RelativeSplitPoint, Height);
        RightChild = TMakeShared<FBspNodeXY>(X + RelativeSplitPoint, Y, Width - RelativeSplitPoint, Height);
    }
    else
    {
        int32 MaxSplit = Height - TargetPivotSize;
        int32 RelativeSplitPoint = Stream.RandRange(TargetPivotSize, MaxSplit);

        LeftChild = TMakeShared<FBspNodeXY>(X, Y, Width, RelativeSplitPoint);
        RightChild = TMakeShared<FBspNodeXY>(X, Y + RelativeSplitPoint, Width, Height - RelativeSplitPoint);
    }

    return true;
}

FXZRoomDesign FBspNodeXY::GetRoomGeometry(float WallThickness) const
{
    // 1 unidad BSP equivale a 100 cm (1 metro) en Unreal
    float RealWidth = (Width * 100.0f) - (WallThickness * 2.0f);
    float RealHeight = (Height * 100.0f) - (WallThickness * 2.0f);
    float RealZHeight = 300.0f; // Altura fija de las paredes (3 metros)

    FXZRoomDesign Design;
    Design.Center.X = (X * 100.0f) + (RealWidth / 2.0f) + WallThickness;
    Design.Center.Y = (Y * 100.0f) + (RealHeight / 2.0f) + WallThickness;
    Design.Center.Z = RealZHeight / 2.0f; // Centro del cuboide tridimensional

    Design.Extents = FVector(RealWidth / 2.0f, RealHeight / 2.0f, RealZHeight / 2.0f);
    return Design;
}

FChunkRoomData FBspNodeXY::ProcessRoomLogicAndFlags(float WallThickness, FRandomStream& Stream) const
{
    FChunkRoomData OutData;
    const float AlturaZConstante = 300.0f;

    // ¡AQUÍ OCURRE LA LLAMADA! El nodo calcula sus dimensiones reales en centímetros
    FXZRoomDesign RoomGeo = GetRoomGeometry(WallThickness);

    // 1. ASIGNACIÓN MATEMÁTICA DE CARAS BASE (Suelo y Techo plano)
    OutData.FloorFace.Location = FVector(RoomGeo.Center.X, RoomGeo.Center.Y, 0.0f);
    OutData.FloorFace.Rotation = FRotator::ZeroRotator;
    OutData.FloorFace.Scale    = FVector(RoomGeo.Extents.X * 2.0f, RoomGeo.Extents.Y * 2.0f, 1.0f);

    OutData.CeilingFace.Location = FVector(RoomGeo.Center.X, RoomGeo.Center.Y, AlturaZConstante);
    OutData.CeilingFace.Rotation = FRotator::ZeroRotator;
    OutData.CeilingFace.Scale    = FVector(RoomGeo.Extents.X * 2.0f, RoomGeo.Extents.Y * 2.0f, 1.0f);

    // 2. ASIGNACIÓN MATEMÁTICA DE CARAS LATERALES (Las 4 Paredes)
    // Pared Norte (+X)
    OutData.WallNorth.Location = FVector(RoomGeo.Center.X + RoomGeo.Extents.X + (WallThickness / 2.0f), RoomGeo.Center.Y, AlturaZConstante / 2.0f);
    OutData.WallNorth.Rotation = FRotator(0.0f, 0.0f, 0.0f);
    OutData.WallNorth.Scale    = FVector(WallThickness, RoomGeo.Extents.Y * 2.0f + (WallThickness * 2.0f), AlturaZConstante);

    // Pared Sur (-X)
    OutData.WallSouth.Location = FVector(RoomGeo.Center.X - RoomGeo.Extents.X - (WallThickness / 2.0f), RoomGeo.Center.Y, AlturaZConstante / 2.0f);
    OutData.WallSouth.Rotation = FRotator(0.0f, 0.0f, 0.0f);
    OutData.WallSouth.Scale    = FVector(WallThickness, RoomGeo.Extents.Y * 2.0f + (WallThickness * 2.0f), AlturaZConstante);

    // Pared Este (+Y)
    OutData.WallEast.Location = FVector(RoomGeo.Center.X, RoomGeo.Center.Y + RoomGeo.Extents.Y + (WallThickness / 2.0f), AlturaZConstante / 2.0f);
    OutData.WallEast.Rotation = FRotator(0.0f, 90.0f, 0.0f);
    OutData.WallEast.Scale    = FVector(WallThickness, RoomGeo.Extents.X * 2.0f, AlturaZConstante);

    // Pared Oeste (-Y)
    OutData.WallWest.Location = FVector(RoomGeo.Center.X, RoomGeo.Center.Y - RoomGeo.Extents.Y - (WallThickness / 2.0f), AlturaZConstante / 2.0f);
    OutData.WallWest.Rotation = FRotator(0.0f, 90.0f, 0.0f);
    OutData.WallWest.Scale    = FVector(WallThickness, RoomGeo.Extents.X * 2.0f, AlturaZConstante);

    // 3. PROYECCIÓN DE FLAGS CON COHESIÓN LÓGICA (Evitar solapamientos)
    // A) Luz de Techo (Padding Interno 2D)
    float LightRadius = 30.0f;
    float SafeMaxX = RoomGeo.Extents.X - LightRadius;
    float SafeMaxY = RoomGeo.Extents.Y - LightRadius;
    if (SafeMaxX > 0 && SafeMaxY > 0 && Stream.FRandRange(0.0f, 1.0f) < 0.75f)
    {
        FVector LightLoc = RoomGeo.Center;
        LightLoc.X += Stream.FRandRange(-SafeMaxX, SafeMaxX);
        LightLoc.Y += Stream.FRandRange(-SafeMaxY, SafeMaxY);
        LightLoc.Z = AlturaZConstante - 10.0f;
        OutData.ApprovedDecorations.Add(FDecorFlagData(EDecorationType::CeilingLight, LightLoc, FRotator::ZeroRotator));
    }

    // B) Enchufe Perimetral (Anclaje exacto al borde de la cara interna 1D)
    if (Stream.FRandRange(0.0f, 1.0f) < 0.60f)
    {
        int32 SelectedWall = Stream.RandRange(0, 3);
        FVector OutletLoc = RoomGeo.Center;
        FRotator OutletRot = FRotator::ZeroRotator;
        float CornerSafetyMargin = 40.0f;

        if (SelectedWall == 0) // Pegado al muro Norte
        {
            OutletLoc.X = RoomGeo.Center.X + RoomGeo.Extents.X; 
            float SafeRangeY = RoomGeo.Extents.Y - CornerSafetyMargin;
            if (SafeRangeY > 0)
            {
                OutletLoc.Y += Stream.FRandRange(-SafeRangeY, SafeRangeY);
                OutletRot = FRotator(0.0f, 180.0f, 0.0f); // Rotar hacia el centro de la sala
                OutletLoc.Z = 30.0f; // Altura desde el suelo
                OutData.ApprovedDecorations.Add(FDecorFlagData(EDecorationType::WallOutlet, OutletLoc, OutletRot));
            }
        }
        // ... (Se calcula igual para muros Sur, Este y Oeste modificando sus respectivos ejes límites fijados)
    }

    return OutData;
}
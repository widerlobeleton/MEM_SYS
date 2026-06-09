#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "BspNodeXY.h"
#include "DungeonOrchestrator.generated.h"

UCLASS()
class YOURPROJECT_API ADungeonOrchestrator : public AActor
{
    GENERATED_BODY()
    
public:	
    ADungeonOrchestrator();

protected:
    virtual void BeginPlay() override;

    // Asignación de recursos visuales desde el Editor de Unreal (Mallas modulares de 100x100x100 cm)
    UPROPERTY(EditAnywhere, Category = "Procedural | Meshes")
    TSoftObjectPtr<UStaticMesh> AssetSueloTecho;

    UPROPERTY(EditAnywhere, Category = "Procedural | Meshes")
    TSoftObjectPtr<UStaticMesh> AssetPared;

    UPROPERTY(EditAnywhere, Category = "Procedural | Blueprints")
    TSubclassOf<AActor> BlueprintLuzTecho;

    // Componentes ISM de alto rendimiento
    UPROPERTY()
    UInstancedStaticMeshComponent* FloorCeilingISM;

    UPROPERTY()
    UInstancedStaticMeshComponent* WallsISM;

private:
    void CargarRecursosAsincronos();
    void BuildAndRenderChunk(int32 StartX, int32 StartY, int32 Width, int32 Height, int32 Seed);
    void RenderRoomByFaces(const FChunkRoomData& RoomData);
    void GetLeavesRecursively(TSharedPtr<FBspNodeXY> CurrentNode, TArray<TSharedPtr<FBspNodeXY>>& OutLeaves);
};
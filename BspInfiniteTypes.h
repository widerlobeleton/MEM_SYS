#pragma once

#include "CoreMinimal.h"
#include "BspInfiniteTypes.generated.h"

UENUM(BlueprintType)
enum class EDecorationType : uint8
{
    None,
        CeilingLight,
            WallOutlet
            };

            USTRUCT(BlueprintType)
            struct FDecorFlagData
            {
                GENERATED_BODY()

                    UPROPERTY()
                        EDecorationType Type;

                            UPROPERTY()
                                FVector Location;

                                    UPROPERTY()
                                        FRotator Rotation;

                                            FDecorFlagData() : Type(EDecorationType::None), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator) {}
                                                FDecorFlagData(EDecorationType InType, FVector InLoc, FRotator InRot) : Type(InType), Location(InLoc), Rotation(InRot) {}
                                                };

                                                USTRUCT(BlueprintType)
                                                struct FFaceTransformData
                                                {
                                                    GENERATED_BODY()

                                                        UPROPERTY()
                                                            FVector Location;

                                                                UPROPERTY()
                                                                    FRotator Rotation;

                                                                        UPROPERTY()
                                                                            FVector Scale;

                                                                                FFaceTransformData() : Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), Scale(FVector::OneVector) {}
                                                                                    FFaceTransformData(FVector InLoc, FRotator InRot, FVector InScale) : Location(InLoc), Rotation(InRot), Scale(InScale) {}
                                                                                    };

                                                                                    USTRUCT(BlueprintType)
                                                                                    struct FXZRoomDesign
                                                                                    {
                                                                                        GENERATED_BODY()
                                                                                            FVector Center;
                                                                                                FVector Extents; // Mitad del tamaño total de la sala (Radio X, Radio Y, Radio Z)
                                                                                                };

                                                                                                USTRUCT(BlueprintType)
                                                                                                struct FChunkRoomData
                                                                                                {
                                                                                                    GENERATED_BODY()

                                                                                                        UPROPERTY()
                                                                                                            FFaceTransformData FloorFace;

                                                                                                                UPROPERTY()
                                                                                                                    FFaceTransformData CeilingFace;

                                                                                                                        UPROPERTY()
                                                                                                                            FFaceTransformData WallNorth; // +X

                                                                                                                                UPROPERTY()
                                                                                                                                    FFaceTransformData WallSouth; // -X

                                                                                                                                        UPROPERTY()
                                                                                                                                            FFaceTransformData WallEast;  // +Y

                                                                                                                                                UPROPERTY()
                                                                                                                                                    FFaceTransformData WallWest;  // -Y

                                                                                                                                                        UPROPERTY()
                                                                                                                                                            TArray<FDecorFlagData> ApprovedDecorations;
                                                                                                                                                            };
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "OceanManager.generated.h"


UCLASS()
class MYPROJECT_API AOceanManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOceanManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(Category = HeightMap, EditAnyWhere)
		UTextureRenderTarget2D* RenderTarget;

	UFUNCTION(BlueprintCallable, Category = "HeightMap|Update")
		void UpdateBuffer();

	UFUNCTION(BlueprintCallable, Category = "HeightMap | Texture Helper")
		FColor GetRenderTargetValue(float x, float y);

	UFUNCTION(BlueprintCallable, Category = "Gerstuner Wave")
		FVector CalculateGerstnerWave(float wavelength, float amplitude, FVector2D position, FVector2D direction, float angle, float steepness, float time, float phase);

	UFUNCTION(BlueprintCallable, Category = "Gerstuner Wave")
		FVector CalculateGerstnerCluster(float medianWavelength, float medianAmplitude, FVector2D position, FVector2D medianDirection, float steepness, float time);

	UFUNCTION(BlueprintCallable, Category = "Gerstuner Wave")
		FVector GetWaveHeightValue(float medianWavelength1, float medianWavelength2, float medianAmplitude1, float medianAmplitude2, FVector location, FVector2D direction, float time);
private:
	TArray<FColor> ColorBuffer;
};

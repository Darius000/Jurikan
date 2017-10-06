// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "OceanManager.h"


// Sets default values
AOceanManager::AOceanManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOceanManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOceanManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AOceanManager::UpdateBuffer()
{
	ColorBuffer.Reset();

	if (RenderTarget != NULL)
	{
		FTextureRenderTarget2DResource* textureResource = (FTextureRenderTarget2DResource*)RenderTarget->Resource;
		if (textureResource->ReadPixels(ColorBuffer))
		{

		}
	}
}

FVector AOceanManager::GetWaveHeightValue(float medianWavelength1, float medianWavelength2, float medianAmplitude1, float medianAmplitude2, FVector location, FVector2D direction, float time)
{
	FVector sum = FVector(0, 0, 0);

	sum += CalculateGerstnerCluster(medianWavelength1, medianAmplitude1, FVector2D(location.X, location.Y), direction, .5f, time);
	sum += CalculateGerstnerCluster(medianWavelength2, medianAmplitude2, FVector2D(location.X, location.Y), direction, .5f, time);

	return sum / 2;
}

FVector AOceanManager::CalculateGerstnerWave(float wavelength, float amplitude, FVector2D position, FVector2D direction, float angle, float steepness, float time, float phase)
{
	float lambda = (2 * PI) / wavelength;

	FVector dir = FVector(direction.X, direction.Y, 0.0f);
	dir = dir.RotateAngleAxis(angle * 360, FVector(0, 0, 1));

	FVector2D rotationDirection = FVector2D(dir.X, dir.Y);

	float wavePhase = lambda * FVector2D::DotProduct(rotationDirection, position) + (time + phase);

	float c = FMath::Cos(wavePhase);
	float s = FMath::Sin(wavePhase);

	float QA = steepness * amplitude;

	return FVector(QA * rotationDirection.X * c, QA * rotationDirection.Y * c, amplitude * s);
}

FVector AOceanManager::CalculateGerstnerCluster(float medianWavelength, float medianAmplitude, FVector2D position, FVector2D medianDirection, float steepness, float time)
{
	//Calculate gerstner wave
	FVector sum = FVector(0, 0, 0);

	sum += CalculateGerstnerWave(medianWavelength, medianAmplitude, position, medianDirection, 0, steepness, time, 0);
	sum += CalculateGerstnerWave(medianWavelength * .5f, medianAmplitude * .5f, position, medianDirection, -.1f, steepness, time, 0);
	sum += CalculateGerstnerWave(medianWavelength * 2.0f, medianAmplitude * 2.0f, position, medianDirection, 0.1f, steepness, time, 0);
	sum += CalculateGerstnerWave(medianWavelength * 1.25f, medianAmplitude * 1.25f, position, medianDirection, 0.05f, steepness, time, 0);
	sum += CalculateGerstnerWave(medianWavelength * .75f, medianAmplitude * .75f, position, medianDirection, 0.075f, steepness, time, 0);
	sum += CalculateGerstnerWave(medianWavelength * 1.5f, medianAmplitude * 1.5f, position, medianDirection, -0.125f, steepness, time, 0);
	sum += CalculateGerstnerWave(medianWavelength * .025f, medianAmplitude * .025f, position, medianDirection, 0.063f, steepness, time, 0);
	sum += CalculateGerstnerWave(medianWavelength * .65f, medianAmplitude * .65f, position, medianDirection, -0.11, steepness, time, 0);

	return sum / 8;
}

FColor AOceanManager::GetRenderTargetValue(float x, float y)
{
	float size = 10000;

	if (RenderTarget == NULL || ColorBuffer.Num() == 0)
		return FColor(0);

	float width = RenderTarget->GetSurfaceWidth();
	float height = RenderTarget->GetSurfaceHeight();

	//convert coordinates to texture space
	float normalizedX = (x / size) + .5f;
	float normalizedY = (y / size) + .5f;

	int i = (int)(normalizedX * width);
	int j = (int)(normalizedY * height);

	if (i < 0) i = 0;
	if (i >= width) i = width - 1;
	if (j < 0) j = 0;
	if (j >= height) j = height - 1;

	int index = i + j * width;
	if (index < 0) index = 0;
	if (index >= ColorBuffer.Num()) index = ColorBuffer.Num();

	return ColorBuffer[index];
}


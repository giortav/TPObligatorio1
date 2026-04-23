// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLifeChanged, float, Health, float, MaxHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPPROG1OBLIGATORIO_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float Health = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MinHealth = 0.f;

	UFUNCTION()
	void OnRep_Health();
	
	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float HealthPoints);

	UPROPERTY(BlueprintAssignable)
	FOnLifeChanged OnLifeChanged;
private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Sanctuary.generated.h"

class UBoxComponent;

UCLASS()
class TPPROG1OBLIGATORIO_API ASanctuary : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASanctuary();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float CoolDown = 10.0f;;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerZone;


	void Interact_Implementation(AActor* Interactor) override;
	
	UPROPERTY(EditAnywhere, Category="Health")
	float HealAmount = 10.0f;
	
	UPROPERTY(EditAnywhere, Category="Health")
	float Interval = 1.0f;

	FTimerHandle CooldownTimer;

	void ResetCooldownTimer();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* AvailableMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* CooldownMaterial;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySanctuaryEffect();

	UPROPERTY(EditAnywhere)
	USoundBase* ActivateSound;

	UPROPERTY(ReplicatedUsing=OnRep_IsAvailable)
	bool bIsAvailable = true;
	
private:
	
	UFUNCTION()
	void OnRep_IsAvailable();

	void UpdateMaterial();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};

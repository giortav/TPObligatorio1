

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
	
	UHealthComponent();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// La variable que quiero replicar tiene que tener este especificador
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float Health = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MinHealth = 0.f;
// Se ejecuta cuando cambia la variable de health (actualiza)
	UFUNCTION()
	void OnRep_Health();
	
	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float HealthPoints);

	UPROPERTY(BlueprintAssignable)
	FOnLifeChanged OnLifeChanged;
private:
	// Sobreescribir la funcion
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};

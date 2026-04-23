


#include "HealthComponent.h"

#include "Net/UnrealNetwork.h"


UHealthComponent::UHealthComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	// Setear en true
	SetIsReplicatedByDefault(true);

	
}



void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
// Agregar la variable con esta macro
	DOREPLIFETIME(UHealthComponent, Health);
}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UHealthComponent::UpdateHealth(float HealthPoints)
{
	if (!GetOwner()->HasAuthority()) return;

	Health = FMath::Clamp(Health + HealthPoints, MinHealth, MaxHealth);

	OnLifeChanged.Broadcast(Health, MaxHealth);
}

// Implementacion de la funcion
void UHealthComponent::OnRep_Health()
{
	OnLifeChanged.Broadcast(Health, MaxHealth);
}

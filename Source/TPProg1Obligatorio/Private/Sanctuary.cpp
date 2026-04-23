#include "Sanctuary.h"
#include "HealthComponent.h"
#include "HealNotifierInterface.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"  // DOREPLIFETIME

ASanctuary::ASanctuary()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true; // CRÍTICO — sin esto nada replica

    TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
    RootComponent = TriggerZone;
    TriggerZone->SetCollisionProfileName(TEXT("Trigger"));

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASanctuary::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
// Agregar la variable con esta macro
    DOREPLIFETIME(ASanctuary, bIsAvailable);
}

void ASanctuary::BeginPlay()
{
    Super::BeginPlay();
    UpdateMaterial(); 
}

void ASanctuary::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASanctuary::Interact_Implementation(AActor* Interactor)
{
    if (!bIsAvailable) return;
    if (!Interactor) return;

    APawn* Pawn = Cast<APawn>(Interactor);
    if (!Pawn) return;
// Se obtiene el componente de health
    UHealthComponent* HealthComp = Pawn->FindComponentByClass<UHealthComponent>();
    if (!HealthComp) return;
// Se actualiza health
    HealthComp->UpdateHealth(HealAmount);
// Se notifica la actualizacion de vida
    if (Pawn->Implements<UHealNotifierInterface>())
    {
        IHealNotifierInterface::Execute_ShowHealMessage(Pawn, HealAmount);
    }
    // Se actualiza el estado y el material
    bIsAvailable = false;
    UpdateMaterial(); 

// Se inicializa el timer
    GetWorldTimerManager().SetTimer(
        CooldownTimer,
        this,
        &ASanctuary::ResetCooldownTimer,
        CoolDown,
        false
    );
}

void ASanctuary::ResetCooldownTimer()
{
    if (!HasAuthority()) return;
    bIsAvailable = true;
    UpdateMaterial(); 
    Multicast_PlaySanctuaryEffect();
}
// Implementacion de la funcion
void ASanctuary::OnRep_IsAvailable()
{
    UpdateMaterial();
}

void ASanctuary::UpdateMaterial()
{
    if (!Mesh) return;
    Mesh->SetMaterial(0, bIsAvailable ? AvailableMaterial : CooldownMaterial);
}
// NetMulticast RPC. Se ejecuta en todos los clientes
void ASanctuary::Multicast_PlaySanctuaryEffect_Implementation()
{
    if (ActivateSound)
    {
        // Se reproduce el sonido
        UGameplayStatics::PlaySoundAtLocation(this, ActivateSound, GetActorLocation());
    }
}
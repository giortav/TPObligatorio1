// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HealNotifierInterface.h"
#include "InteractableInterface.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TPProg1ObligatorioCharacter.generated.h"

class UHealthWidget;
class UHealthComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class ATPProg1ObligatorioCharacter : public ACharacter, public IHealNotifierInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* InteractAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

public:

	/** Constructor */
	ATPProg1ObligatorioCharacter();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	virtual void BeginPlay() override;

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();


	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoInteract();

	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void NotifyActorEndOverlap(AActor* OtherActor) override;

	AActor* CurrentInteractable;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UHealthWidget> HealthWidgetClass;

	UPROPERTY()
	UHealthWidget* HealthWidget;

	UFUNCTION(BlueprintCallable)
	void HandleLifeChanged(float Health, float MaxHealth);

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UHealNotificationWidget> HealNotificationClass;

	UPROPERTY()
	UHealNotificationWidget* HealNotificationWidget;

	UFUNCTION(Client, Reliable)
	void Client_ShowHealMessage(float HealAmount);

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowHealMessage(float HealAmount);

	UFUNCTION(Server, Reliable)
	void Server_Interact(AActor* Interactable);
};


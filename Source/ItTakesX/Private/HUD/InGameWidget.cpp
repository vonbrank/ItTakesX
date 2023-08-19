// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InGameWidget.h"

#include "Ability/DrivingComponent.h"
#include "Ability/HealthComponent.h"
#include "Ability/InventoryComponent.h"
#include "Ability/GrabberComponent.h"
#include "BuildingSystem/VehicleControllerActor.h"
#include "Character/ItTakesXCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


bool UInGameWidget::Initialize()
{
	Super::Initialize();

	Character = Cast<AItTakesXCharacter>(GetOwningPlayerPawn());

	if (HealthBar)
	{
		HealthBar->PercentDelegate.BindDynamic(this, &ThisClass::GetHealth);
	}

	if (HealthContainText)
	{
		HealthContainText->TextDelegate.BindDynamic(this, &ThisClass::GetHealthContainText);
	}

	if (CarHealthBar)
	{
		CarHealthBar->PercentDelegate.BindDynamic(this, &ThisClass::GetCarHealth);
	}
	if (CarHealthContainText)
	{
		CarHealthContainText->TextDelegate.BindDynamic(this, &ThisClass::GetCarContainText);
	}

	if (CarArmourBar)
	{
		CarArmourBar->PercentDelegate.BindDynamic(this, &ThisClass::GetCarArmourBar);
	}
	if (CarArmourContainText)
	{
		CarArmourContainText->TextDelegate.BindDynamic(this, &ThisClass::GetCarArmourContainText);
	}

	if (KeyOneTextBlock)
	{
		KeyOneTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyOneText);
	}
	if (KeyTwoTextBlock)
	{
		KeyTwoTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyTwoText);
	}
	if (KeyThreeTextBlock)
	{
		KeyThreeTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyThreeText);
	}
	if (KeyVTextBlock)
	{
		KeyVTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyVText);
	}

	if (KeyETextBlock)
	{
		KeyETextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyEText);
	}
	if (KeyFTextBlock)
	{
		KeyFTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyFText);
	}
	if (KeyXTextBlock)
	{
		KeyXTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyXText);
	}
	if (KeyLShiftTextBlock)
	{
		KeyLShiftTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyLShiftText);
	}
	if (KeyPageUpTextBlock)
	{
		KeyPageUpTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyPageUpText);
	}
	if (KeyPageDownTextBlock)
	{
		KeyPageDownTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyPageDownText);
	}
	if (KeyLTextBlock)
	{
		KeyLTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyLText);
	}
	if (KeyKTextBlock)
	{
		KeyKTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyKText);
	}

	if (KeyWASDTextBlock)
	{
		KeyWASDTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyWASDText);
	}
	if (KeyLRTextBlock)
	{
		KeyLRTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyLRText);
	}
	if (KeyUDTextBlock)
	{
		KeyUDTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetKeyUDText);
	}
	if (ThrusterPercentageTextBlock)
	{
		ThrusterPercentageTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetThrusterPercentageText);
	}
	if (VehicleSpeedTextBlock)
	{
		VehicleSpeedTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetVehicleSpeedText);
	}

	return true;
}


float UInGameWidget::GetHealth()
{
	if (Character)
	{
		return Character->Health->GetHealthPercentage();
	}
	return 0;
}

FText UInGameWidget::GetHealthContainText()
{
	if (Character)
	{
		return FText::FromString(
			FString::Printf(TEXT("%.f/%.f"), Character->Health->GetHealth(), Character->Health->GetMaxHealth()));
	}
	return FText::FromString(FString::Printf(TEXT("N/A")));
}

float UInGameWidget::GetCarHealth()
{
	if (Character)
	{
		return Character->Driving->GetDrivingVehicleHealthPercentage();
	}
	return 0.5;
}

FText UInGameWidget::GetCarContainText()
{
	if (Character)
	{
		return FText::FromString(
			FString::Printf(TEXT("%.f/%.f"), Character->Driving->GetDrivingVehicleHealth(),
			                Character->Driving->GetDrivingVehicleMaxHealth()));
	}
	return FText::FromString(FString::Printf(TEXT("N/A")));
}

float UInGameWidget::GetCarArmourBar()
{
	if (Character)
	{
		return Character->Driving->GetDrivingVehicleArmourHealthPercentage();
	}
	return 0.5;
}

FText UInGameWidget::GetCarArmourContainText()
{
	if (Character)
	{
		return FText::FromString(
			FString::Printf(TEXT("%.f/%.f"), Character->Driving->GetDrivingVehicleArmourHealth(),
			                Character->Driving->GetDrivingVehicleArmourMaxHealth()));
	}
	return FText::FromString(FString::Printf(TEXT("N/A")));
}

FText UInGameWidget::GetKeyOneText()
{
	return FText::FromString(FString::Printf(TEXT("取消装备 - 1")));
}

FText UInGameWidget::GetKeyTwoText()
{
	return FText::FromString(FString::Printf(TEXT("装备磁铁 - 2")));
}

FText UInGameWidget::GetKeyThreeText()
{
	return FText::FromString(FString::Printf(TEXT("装备枪 - 3")));
}

FText UInGameWidget::GetKeyVText()
{
	return FText::FromString(FString::Printf(TEXT("启用/停止滑翔 - V")));
}

FText UInGameWidget::GetKeyEText()
{
	if (Character->InventoryComponent->HasMagnetEquipped())
	{
		return FText::FromString(FString::Printf(TEXT("吸起/放下零件 - E")));
	}
	if (Character->InventoryComponent->HasWeaponEquipped())
	{
		return FText::FromString(FString::Printf(TEXT("断开零件的连接 - E")));
	}
	if (Character->GetVehicleControllerFromCharacter())
	{
		return FText::FromString(FString::Printf(TEXT("开火 - E/LMB")));
	}
	return FText::FromString(FString::Printf(TEXT("")));
}

FText UInGameWidget::GetKeyFText()
{
	if (Character->InventoryComponent->HasMagnetEquipped())
	{
		return FText::FromString(FString::Printf(TEXT("连接零件 - F")));
	}
	if (Character->Driving->IsOverlappingVehicle() || Character->GetVehicleControllerFromCharacter() != nullptr)
	{
		return FText::FromString(FString::Printf(TEXT("登上/离开载具 - F")));
	}
	return FText::FromString(FString::Printf(TEXT("")));
}

FText UInGameWidget::GetKeyXText()
{
	if (Character->Grabber->IsHoisting())
	{
		return FText::FromString(FString::Printf(TEXT("水平旋转零件 - Z/X")));
	}
	if (Character->GetVehicleControllerFromCharacter() != nullptr && !Character->GetVehicleControllerFromCharacter()->
		IsAimingOpenFireMode())
	{
		return FText::FromString(FString::Printf(TEXT("水平旋转炮塔 - Z/X")));
	}
	return FText::FromString(FString::Printf(TEXT("")));
}

// FText UInGameWidget::GetKeyZText()
// {
// }

FText UInGameWidget::GetKeyLShiftText()
{
	if (Character->Grabber->IsHoisting())
	{
		return FText::FromString(FString::Printf(TEXT("竖直旋转零件 - LCtrl/LShift")));
	}
	if (Character->GetVehicleControllerFromCharacter() != nullptr && !Character->GetVehicleControllerFromCharacter()->
		IsAimingOpenFireMode())
	{
		return FText::FromString(FString::Printf(TEXT("竖直旋转炮塔 - Z/X")));
	}
	return FText::FromString(FString::Printf(TEXT("")));
}

// FText UInGameWidget::GetKeyLCtrlText()
// {
// }

FText UInGameWidget::GetKeyPageUpText()
{
	if (Character->GetVehicleControllerFromCharacter() != nullptr)
	{
		return FText::FromString(FString::Printf(TEXT("提高引擎输出 - Page Up")));
	}
	return FText::FromString(FString::Printf(TEXT("")));
}

FText UInGameWidget::GetKeyPageDownText()
{
	if (Character->GetVehicleControllerFromCharacter() != nullptr)
	{
		return FText::FromString(FString::Printf(TEXT("降低引擎输出 - Page Down")));
	}
	return FText::FromString(FString::Printf(TEXT("")));
}

FText UInGameWidget::GetKeyLText()
{
	if (Character->GetVehicleControllerFromCharacter() != nullptr)
	{
		if (Character->GetVehicleControllerFromCharacter()->
		               IsAimingOpenFireMode())
		{
			return FText::FromString(FString::Printf(TEXT("炮塔切换至按键瞄准模式 - L")));
		}

		return FText::FromString(FString::Printf(TEXT("炮塔切换至准星瞄准模式 - L")));
	}
	return FText::FromString(FString::Printf(TEXT("")));
}

FText UInGameWidget::GetKeyKText()
{
	if (Character->GetVehicleControllerFromCharacter() != nullptr)
	{
		return FText::FromString(FString::Printf(TEXT("开启/关闭火焰喷射器 - K")));
	}
	return FText::FromString(FString::Printf(TEXT("")));
}

FText UInGameWidget::GetKeyWASDText()
{
	if (Character->GetVehicleControllerFromCharacter())
	{
		return FText::FromString(FString::Printf(TEXT("车轮驱动/转向 - WASD")));
	}
	return FText::FromString(FString::Printf(TEXT("角色 Locomotion - WASD")));
}

FText UInGameWidget::GetKeyLRText()
{
	if (Character->GetVehicleControllerFromCharacter())
	{
		return FText::FromString(FString::Printf(TEXT("偏航x滚转 - ← →")));
	}
	return FText::FromString(FString::Printf(TEXT("")));
}

FText UInGameWidget::GetKeyUDText()
{
	if (Character->GetVehicleControllerFromCharacter())
	{
		return FText::FromString(FString::Printf(TEXT("俯仰- ↑ ↓")));
	}
	return FText::FromString(FString::Printf(TEXT("")));
}

FText UInGameWidget::GetThrusterPercentageText()
{
	if (Character->GetVehicleControllerFromCharacter())
	{
		return FText::FromString(FString::Printf(
			TEXT("推进器输出百分比: %.f%%"),
			Character->GetVehicleControllerFromCharacter()->GetAircraftThrottlePercentage() * 100));
	}
	return FText::FromString(FString::Printf(TEXT("推进器输出百分比: 0%%")));
}

FText UInGameWidget::GetVehicleSpeedText()
{
	if (Character->GetVehicleControllerFromCharacter())
	{
		if (Character->GetVehicleControllerFromCharacter()->GetCurrentForwardSpeedLength() == -0)
		{
			return FText::FromString(FString::Printf(TEXT("载具前向速度: 0 m/s")));
		}

		return FText::FromString(FString::Printf(
			TEXT("载具前向速度: %.f m/s"),
			Character->GetVehicleControllerFromCharacter()->GetCurrentForwardSpeedLength() / 100));
	}
	return FText::FromString(FString::Printf(TEXT("载具前向速度: 0 m/s")));
}

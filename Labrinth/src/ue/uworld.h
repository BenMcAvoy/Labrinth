#pragma once

namespace Labrinth::UE {
	static inline struct UWorld* GWorld = nullptr;

	struct UWorld : UObject {
		static UWorld* Get() {
			const uintptr_t poUWorld = GDriver.GetBaseAddress() + Offsets::G_WORLD_OFFSET;
			const auto pUWorld = GDriver.Read<UWorld*>(poUWorld);
			GWorld = pUWorld;
			return pUWorld;
		}

		LABRINTH_PROP_OFFSET(AGameStateBase*, GameState, 0x158);
		LABRINTH_PROP_OFFSET(UGameInstance*, OwningGameInstance, 0x1B8);
	};
}

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppUninitializedNonStaticDataMember
// ReSharper disable CppClassNeedsConstructorBecauseOfUninitializedMember

#pragma once

namespace Labrinth::UE {
	struct APlayerState : AActor {
		LABRINTH_PROP_OFFSET(APawn*, PawnPrivate, 0x308)
	};
	
	struct APG_PlayerState_Game : APlayerState {
		LABRINTH_PROP_OFFSET(uint8_t, Team, 0x428)
		LABRINTH_PROP_OFFSET(struct UInventoryComponent_Game*, InventoryComponent, 0x458)
	};
}

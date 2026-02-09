#pragma once

namespace Labrinth::UE {
	struct AGameStateBase : AActor {
		LABRINTH_PROP_OFFSET(TArray<APG_PlayerState_Game*>, PlayerArray, 0x2A8)
	};
} 

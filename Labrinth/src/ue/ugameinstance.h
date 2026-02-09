#pragma once

namespace Labrinth::UE {
	struct UGameInstance : UObject {
		LABRINTH_PROP_OFFSET(TArray<ULocalPlayer*>, LocalPlayers, 0x38);
	};
}

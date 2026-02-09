#pragma once

namespace Labrinth::UE {
	struct UPlayer : UObject {
		LABRINTH_PROP_OFFSET(APlayerController*, PlayerController, 0x30);
	};
}

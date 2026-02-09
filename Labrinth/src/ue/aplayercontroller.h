#pragma once

namespace Labrinth::UE {
	struct APlayerController : AActor {
		LABRINTH_PROP_OFFSET(APawn*, AcknowledgedPawn, 0x338);
		LABRINTH_PROP_OFFSET(APlayerCameraManager*, PlayerCameraManager, 0x348);
	};
}

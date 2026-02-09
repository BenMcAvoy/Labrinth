#pragma once

namespace Labrinth::UE {
	struct AActor : UObject {
		LABRINTH_PROP_OFFSET(struct USceneComponent*, RootComponent, 0x198)
	};
}

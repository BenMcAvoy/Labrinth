#pragma once

namespace Labrinth::UE {
	struct ACharacter : APawn {
		LABRINTH_PROP_OFFSET(struct USkeletalMeshComponent*, Mesh, 0x340);
	};
}

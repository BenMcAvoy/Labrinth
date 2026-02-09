#pragma once

namespace Labrinth::UE {
	class UObject {
	public:
		LABRINTH_PROP_OFFSET(uintptr_t*, VTable, 0x0);
		LABRINTH_PROP_OFFSET(uint32_t, ObjectFlags, 0x8);
		LABRINTH_PROP_OFFSET(uint32_t, ObjectIndex, 0xC);
		LABRINTH_PROP_OFFSET(class UClass*, ClassPrivate, 0x10);
		LABRINTH_PROP_OFFSET(FName, NamePrivate, 0x18);
		LABRINTH_PROP_OFFSET(UObject*, OuterPrivate, 0x20);
	};
}

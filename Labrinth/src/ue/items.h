#pragma once

namespace Labrinth::UE {
	struct AItem_Gun_General {
		LABRINTH_PROP_OFFSET(float, GunUpRecoil, 0x3fc)
		LABRINTH_PROP_OFFSET(float, AccuracyHip, 0x414)
		LABRINTH_PROP_OFFSET(float, CurrentSpread, 0x734)
	};
}

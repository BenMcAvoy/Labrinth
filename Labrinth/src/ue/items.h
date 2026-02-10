#pragma once

namespace Labrinth::UE {
    enum class EGunShootingType : uint8_t {
        BOLT = 0,
        SEMI_AUTO = 1,
        AUTO = 2,
        EGunShootingType_MAX = 3
    };

    struct AItem_Gun_General {
        LABRINTH_PROP_OFFSET(float, TimeBetweenShots, 0x3f8)
        LABRINTH_PROP_OFFSET(float, GunUpRecoil, 0x3fc)
        LABRINTH_PROP_OFFSET(float, AccuracyHip, 0x414)
        LABRINTH_PROP_OFFSET(int32_t, MaxMagazineAmmo, 0x3f0)
        LABRINTH_PROP_OFFSET(int32_t, MaxStockAmmo, 0x3f4)
        LABRINTH_PROP_OFFSET(int32_t, CurrentMagazineAmmo, 0x704)
        LABRINTH_PROP_OFFSET(uint16_t, CurrentStockAmmo, 0x708)
        LABRINTH_PROP_OFFSET(EGunShootingType, GunShootingType, 0x39f)
        LABRINTH_PROP_OFFSET(float, CurrentSpread, 0x734)
    };
}

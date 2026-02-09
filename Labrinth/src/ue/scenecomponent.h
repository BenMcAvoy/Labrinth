// ReSharper disable CppInconsistentNaming
// ReSharper disable CppUninitializedNonStaticDataMember
// ReSharper disable CppClassNeedsConstructorBecauseOfUninitializedMember

#pragma once

namespace Labrinth::UE {
    struct UActorComponent : UObject {};
    struct USceneComponent : UActorComponent {
        LABRINTH_PROP_OFFSET(FVector, RelativeLocation, 0x128)
        LABRINTH_PROP_OFFSET(FTransform, ComponentToWorld, 0x280); // TODO
    };
}

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppUninitializedNonStaticDataMember
// ReSharper disable CppClassNeedsConstructorBecauseOfUninitializedMember

#pragma once

namespace Labrinth::UE {
    struct FMinimalViewInfo {
        /* 0x0000 */ FVector Location;
        /* 0x000C */ FRotator Rotation;
        /* 0x0080 */ float FOV;
        /* 0x0084 */ float DesiredFOV;
        /* 0x0088 */ float FirstPersonFOV;
        /* 0x008C */ float OrthoWidth;
        /* 0x0090 */ float OrthoNearClipPlane;
        /* 0x0094 */ float OrthoFarClipPlane;
        /* 0x0098 */ float AspectRatio;

        std::string ToString() {
            return std::format("FMinimalViewInfo(Location: {}, Rotation: {}, FOV: {}, DesiredFOV: {}, FirstPersonFOV: {}, OrthoWidth: {}, OrthoNearClipPlane: {}, OrthoFarClipPlane: {}, AspectRatio: {})",
                Location.ToString(), Rotation.ToString(), FOV, DesiredFOV, FirstPersonFOV, OrthoWidth, OrthoNearClipPlane, OrthoFarClipPlane, AspectRatio);
        }
    };

    struct FCameraCacheEntry {
        /* 0x0000 */ float Timestamp;
    private:
        /* 0x0004 */ char pad_0x0004[0xC];
    public:
        /* 0x0010 */ FMinimalViewInfo POV;

        std::string ToString() {
            return std::format("FCameraCacheEntry(Timestamp: {}, POV: {})", Timestamp, POV.ToString());
        }
    };

    struct APlayerCameraManager : AActor {
        LABRINTH_PROP_OFFSET(FCameraCacheEntry, CameraCachePrivate, 0x22B0)

            std::optional<FVector2D> WorldToScreen(const FVector& WorldPos, uint32_t Width, uint32_t Height) const
        {
            if (!this) return std::nullopt;

            const auto& POV = CameraCachePrivate.POV;

            // Convert quaternion to matrix
            const FMatrix View(POV.Rotation); // make sure ToMatrixWithScale uses the same math as Rust

            // Extract camera axes
            const FVector AxisX{ View.M[0][0], View.M[0][1], View.M[0][2] }; // forward
            const FVector AxisY{ View.M[1][0], View.M[1][1], View.M[1][2] }; // right
            const FVector AxisZ{ View.M[2][0], View.M[2][1], View.M[2][2] }; // up

            // Transform world to camera space
            const FVector Delta = WorldPos - POV.Location;
            const FVector Transformed{
                Delta.Dot(AxisY),  // X = right
                Delta.Dot(AxisZ),  // Y = up
                Delta.Dot(AxisX)   // Z = forward
            };

            if (Transformed.Z < 1.0f)
                return std::nullopt; // behind camera

            const float ScreenCenterX = Width * 0.5f;
            const float ScreenCenterY = Height * 0.5f;

            // FOV conversion (degrees -> radians / horizontal FOV)
            constexpr float PI = 3.14159265358979323846f;
            const float FovRad = POV.FOV * 0.5f * PI / 180.0f;
            const float TanFov = std::tan(FovRad);

            FVector2D Screen;
            Screen.X = ScreenCenterX + (Transformed.X * (ScreenCenterX / TanFov)) / Transformed.Z;
            Screen.Y = ScreenCenterY - (Transformed.Y * (ScreenCenterX / TanFov)) / Transformed.Z;

            return Screen;
        }
    };
}

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppUninitializedNonStaticDataMember
// ReSharper disable CppClassNeedsConstructorBecauseOfUninitializedMember

#pragma once

namespace Labrinth::UE {
    struct FMeshBoneInfo {
        uint32_t NameIdx;
        int32_t ParentIdx;
        char pad_0x000[0x4];
    };

    struct FReferenceSkeleton {
        /* 0x000 */ TArray<FMeshBoneInfo> RawRefBoneInfo;
        /* 0x010 */ TArray<FTransform> RawRefBonePos;

        /* 0x020 */ TArray<FMeshBoneInfo> FinalRefBoneInfo;
        /* 0x030 */ TArray<FTransform> FinalRefBonePos;

        std::optional<int> GetBoneIndexByName(const std::string_view boneName) {
            try {
                auto& boneInfos = RawRefBoneInfo;

                int index = 0;
                for (auto boneInfo : boneInfos) {
                    try {
                        FName name(boneInfo.NameIdx);
                        std::string& nameStr = name.ToString();

                        if (nameStr == boneName) {
                            return index;
                        }

                        index++;
                    }
                    catch (std::exception) {
                        std::string msg = std::format(
                            "Failed to get bone name at index {}: {}",
                            index,
                            boneInfo.NameIdx
						);
                        index++;
                        continue;
                    }
                }

                return std::nullopt;
            }
            catch (std::exception) {
                return {};
            }
        }
    };

    // Technically inherits from UStreamableRenderAsset
    struct USkeletalMesh : UObject {
        LABRINTH_PROP_OFFSET(FReferenceSkeleton, RefSkeleton, 0x1B8)
    };

    enum class EVisibilityBasedAnimTickOption : uint8_t {
        AlwaysTickPoseAndRefreshBones = 0,
        AlwaysTickPose = 1,
        OnlyTickMontagesWhenNotRendered = 2,
        OnlyTickPoseWhenRendered = 3,
        EVisibilityBasedAnimTickOption_MAX = 4
    };

	struct UPrimitiveComponent : USceneComponent {};
	struct UMeshComponent : UPrimitiveComponent {};
    struct USkinnedMeshComponent : UMeshComponent {
		LABRINTH_PROP_OFFSET(USkeletalMesh*, SkeletalMesh, 0x700)
        LABRINTH_PROP_OFFSET(TArray<FTransform>, CachedComponentSpaceTransforms1, 0x720)
        LABRINTH_PROP_OFFSET(TArray<FTransform>, CachedComponentSpaceTransforms2, 0x730)
		LABRINTH_PROP_OFFSET(EVisibilityBasedAnimTickOption, VisibilityBasedAnimTickOption, 0x874)
		LABRINTH_PROP_OFFSET(uint8_t, Flags, 0x878)

		__declspec(property(get = _getBoneArray)) TArray<FTransform> BoneArray;
        TArray<FTransform> _getBoneArray() {
			TArray<FTransform> transforms1 = CachedComponentSpaceTransforms1;
			return (transforms1.Count == 0) ? CachedComponentSpaceTransforms2 : transforms1;
		}
    };

    struct USkeletalMeshComponent : USkinnedMeshComponent { };
}

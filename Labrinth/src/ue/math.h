// ReSharper disable CppInconsistentNaming
// ReSharper disable CppUninitializedNonStaticDataMember
// ReSharper disable CppClassNeedsConstructorBecauseOfUninitializedMember

#pragma once

namespace Labrinth::UE {
    struct FQuat {
        double X;
        double Y;
        double Z;
        double W;

        FQuat() : X(0), Y(0), Z(0), W(1) {}
        FQuat(const double x, const double y, const double z, const double w) : X(x), Y(y), Z(z), W(w) {}

        std::string ToString() const {
            return std::format("FQuat(X: {}, Y: {}, Z: {}, W: {})", X, Y, Z, W);
        }
    };

    struct FVector {
        double X;
        double Y;
        double Z;

        FVector() : X(0), Y(0), Z(0) {}
        FVector(const double x, const double y, const double z) : X(x), Y(y), Z(z) {}

        FVector operator+(const FVector& other) const {
            return FVector(X + other.X, Y + other.Y, Z + other.Z);
        }
        FVector operator-(const FVector& other) const {
            return FVector(X - other.X, Y - other.Y, Z - other.Z);
        }
        FVector operator*(const double scalar) const {
            return FVector(X * scalar, Y * scalar, Z * scalar);
        }
        FVector operator/(const double scalar) const {
            return FVector(X / scalar, Y / scalar, Z / scalar);
        }

        double Size() const {
            return std::sqrt(X * X + Y * Y + Z * Z);
        }

        double Dot(const FVector Other) const {
            return X * Other.X + Y * Other.Y + Z * Other.Z;
        }

        std::string ToString() const {
            return std::format("FVector(X: {}, Y: {}, Z: {})", X, Y, Z);
        }
    };

    struct FVector2D {
        double X;
        double Y;

        FVector2D() : X(0), Y(0) {}
        FVector2D(const double x, const double y) : X(x), Y(y) {}

        std::string ToString() const {
            return std::format("FVector2D(X: {}, Y: {})", X, Y);
        }

        ImVec2 ToImVec2() {
            return {
                (float)X,
                (float)Y
            };
        }
	};

    struct FVector4 {
        double X;
        double Y;
        double Z;
        double W;
        FVector4() : X(0), Y(0), Z(0), W(0) {}
        FVector4(const double x, const double y, const double z, const double w) : X(x), Y(y), Z(z), W(w) {}
        std::string ToString() const {
            return std::format("FVector4(X: {}, Y: {}, Z: {}, W: {})", X, Y, Z, W);
        }
	};

    struct FRotator {
        double Pitch;
        double Yaw;
        double Roll;

        FRotator() : Pitch(0), Yaw(0), Roll(0) {}
        FRotator(const double pitch, const double yaw, const double roll) : Pitch(pitch), Yaw(yaw), Roll(roll) {}

        std::string ToString() const {
            return std::format("FRotator(Pitch: {}, Yaw: {}, Roll: {})", Pitch, Yaw, Roll);
        }
    };

    struct FMatrix {
        double M[4][4] = {
            { 0.0, 0.0, 0.0, 0.0 },
            { 0.0, 0.0, 0.0, 0.0 },
            { 0.0, 0.0, 0.0, 0.0 },
            { 0.0, 0.0, 0.0, 0.0 },
        };

        FMatrix() {
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    M[i][j] = 0.0f;
                }
            }
        }
        explicit FMatrix(const FRotator& Rot) {
            const double RadPitch = Rot.Pitch * 3.14159265f / 180.0f;
            const double RadYaw = Rot.Yaw * 3.14159265f / 180.0f;
            const double RadRoll = Rot.Roll * 3.14159265f / 180.0f;

            const double SP = std::sin(RadPitch);
            const double CP = std::cos(RadPitch);
            const double SY = std::sin(RadYaw);
            const double CY = std::cos(RadYaw);
            const double SR = std::sin(RadRoll);
            const double CR = std::cos(RadRoll);

            M[0][0] = CP * CY;
            M[0][1] = CP * SY;
            M[0][2] = SP;
            M[0][3] = 0.0f;

            M[1][0] = SR * SP * CY - CR * SY;
            M[1][1] = SR * SP * SY + CR * CY;
            M[1][2] = -SR * CP;
            M[1][3] = 0.0f;

            M[2][0] = -(CR * SP * CY + SR * SY);
            M[2][1] = CY * SR - CR * SP * SY;
            M[2][2] = CR * CP;
            M[2][3] = 0.0f;

            M[3][0] = 0.0f; // origin.Roll
            M[3][1] = 0.0f; // origin.Yaw
            M[3][2] = 0.0f; // origin.Pitch
            M[3][3] = 1.0f;
        }

        FMatrix operator*(const FMatrix& Other) const {
            FMatrix Result;

            for (int row = 0; row < 4; ++row) {
                for (int col = 0; col < 4; ++col) {
                    Result.M[row][col] =
                        M[row][0] * Other.M[0][col] +
                        M[row][1] * Other.M[1][col] +
                        M[row][2] * Other.M[2][col] +
                        M[row][3] * Other.M[3][col];
                }
            }

            return Result;
        }

        std::string ToString() const {
            std::string result = "FMatrix(\n";
            for (int i = 0; i < 4; ++i) {
                result += "  [";
                for (int j = 0; j < 4; ++j) {
                    result += std::format("{:>8.3f}", M[i][j]);
                    if (j < 3) result += ", ";
                }
                result += "]\n";
            }
            result += ")";
            return result;
        }
    };

    struct FTransform {
        FQuat Rotation;
        FVector Translation;
        char pad_0x01C[0x4];
        FVector Scale3D;
        char pad_0x02C[0x4];

        FMatrix ToMatrix() const {
            FMatrix matrix{};

            matrix.M[3][0] = Translation.X;
            matrix.M[3][1] = Translation.Y;
            matrix.M[3][2] = Translation.Z;

            const double x2 = Rotation.X + Rotation.X;
            const double y2 = Rotation.Y + Rotation.Y;
            const double z2 = Rotation.Z + Rotation.Z;

            const double xx2 = Rotation.X * x2;
            const double yy2 = Rotation.Y * y2;
            const double zz2 = Rotation.Z * z2;

            matrix.M[0][0] = (1.0f - (yy2 + zz2)) * Scale3D.X;
            matrix.M[1][1] = (1.0f - (xx2 + zz2)) * Scale3D.Y;
            matrix.M[2][2] = (1.0f - (xx2 + yy2)) * Scale3D.Z;

            const double yz2 = Rotation.Y * z2;
            const double wx2 = Rotation.W * x2;
            matrix.M[2][1] = (yz2 - wx2) * Scale3D.Z;
            matrix.M[1][2] = (yz2 + wx2) * Scale3D.Y;

            const double xy2 = Rotation.X * y2;
            const double wz2 = Rotation.W * z2;
            matrix.M[1][0] = (xy2 - wz2) * Scale3D.X;
            matrix.M[0][1] = (xy2 + wz2) * Scale3D.Y;

            const double xz2 = Rotation.X * z2;
            const double wy2 = Rotation.W * y2;
            matrix.M[2][0] = (xz2 + wy2) * Scale3D.X;
            matrix.M[0][2] = (xz2 - wy2) * Scale3D.Z;

            matrix.M[0][3] = 0.0f;
            matrix.M[1][3] = 0.0f;
            matrix.M[2][3] = 0.0f;
            matrix.M[3][3] = 1.0f;

            return matrix;
        }

        std::string Tostring() const {
            return std::format(
                "FTransform(Rotation: {}, Translation: {}, Scale3D: {})",
                Rotation.ToString(),
                Translation.ToString(),
                Scale3D.ToString()
            );
        }
    };
}

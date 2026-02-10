#define NOMINMAX

#include "overlay.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <unordered_map>

#include <TlHelp32.h>
#include "driver.h"

#include "offsets.h"
#include <Windows.h>
#include "macros.h"
#include "ue/ue.h"

#include <print>

void watermark() {
	ImDrawList* draw = ImGui::GetForegroundDrawList();
	ImGuiIO& io = ImGui::GetIO();

	const char* title = "Labrinth";
	const char* sep = "|";

	char fpsBuf[32];
	snprintf(fpsBuf, sizeof(fpsBuf), "%.0f FPS", io.Framerate);

	ImVec2 padding(8.0f, 6.0f);
	float accentWidth = 3.0f;

	ImVec2 titleSize = ImGui::CalcTextSize(title);
	ImVec2 sepSize = ImGui::CalcTextSize(sep);
	ImVec2 fpsSize = ImGui::CalcTextSize(fpsBuf);

	float textHeight = std::max({
		titleSize.y,
		sepSize.y,
		fpsSize.y
		});

	float height =
		padding.y * 2.0f +
		textHeight;

	float width =
		accentWidth +
		padding.x +
		titleSize.x +
		padding.x * 0.5f +
		sepSize.x +
		padding.x * 0.5f +
		fpsSize.x +
		padding.x;

	ImVec2 pos(10.0f, 10.0f);
	ImVec2 end(pos.x + width, pos.y + height);

	ImU32 bgCol = IM_COL32(20, 20, 20, 220);
	ImU32 accentCol = Labrinth::AccentColour;
	ImU32 textCol = IM_COL32(230, 230, 230, 255);
	ImU32 fpsCol = IM_COL32(160, 160, 160, 255);

	draw->AddRectFilled(pos, end, bgCol, 4.0f);

	draw->AddRectFilled(
		pos,
		ImVec2(pos.x + accentWidth, end.y),
		accentCol,
		4.0f,
		ImDrawFlags_RoundCornersLeft
	);

	float textY = pos.y + (height - textHeight) * 0.5f;
	float x = pos.x + accentWidth + padding.x;

	draw->AddText(ImVec2(x, textY), accentCol, title);
	x += titleSize.x + padding.x * 0.5f;

	draw->AddText(ImVec2(x, textY), textCol, sep);
	x += sepSize.x + padding.x * 0.5f;

	draw->AddText(ImVec2(x, textY), fpsCol, fpsBuf);
}

void windowheader(const char* title) {
	ImDrawList* draw = ImGui::GetWindowDrawList();
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 padding(8.0f, 6.0f);
	float accentWidth = 3.0f;
	ImVec2 titleSize = ImGui::CalcTextSize(title);
	float textHeight = titleSize.y;
	float height =
		padding.y * 2.0f +
		textHeight;
	// width is the full window width
	float width = windowSize.x;
	ImVec2 pos(windowPos.x, windowPos.y);
	ImVec2 end(pos.x + width, pos.y + height);
	ImU32 bgCol = IM_COL32(20, 20, 20, 220);
	ImU32 accentCol = Labrinth::AccentColour;
	ImU32 textCol = IM_COL32(230, 230, 230, 255);
	draw->AddRectFilled(pos, end, bgCol, 4.0f);
	draw->AddRectFilled(
		pos,
		ImVec2(pos.x + accentWidth, end.y),
		accentCol,
		4.0f,
		ImDrawFlags_RoundCornersLeft
	);
	float textY = pos.y + (height - textHeight) * 0.5f;
	float x = pos.x + accentWidth + padding.x;
	draw->AddText(ImVec2(x, textY), accentCol, title);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + height);
}

bool checkbox(const char* label, bool& v) {
	ImDrawList* draw = ImGui::GetWindowDrawList();
	ImGuiIO& io = ImGui::GetIO();

	// Per-label animation progress (0.0 = unchecked, 1.0 = checked)
	static std::unordered_map<std::string, float> s_progress;

	// Invisible button to capture clicks
	ImGui::InvisibleButton(label, ImVec2(20.0f, 20.0f));
	if (ImGui::IsItemClicked()) {
		v = !v;
	}

	float& prog = s_progress[std::string(label)];
	float target = v ? 1.0f : 0.0f;
	float dt = io.DeltaTime;
	const float speed = 12.0f; // higher = snappier
	float step = std::min(dt * speed, 1.0f);
	prog += (target - prog) * step;

	ImVec2 pos = ImGui::GetItemRectMin();
	ImVec2 end = ImGui::GetItemRectMax();

	// Lerp between frame background and accent colour
	ImVec4 bgCol = ImGui::GetStyleColorVec4(ImGuiCol_FrameBg);
	ImVec4 accCol = ImGui::ColorConvertU32ToFloat4(Labrinth::AccentColour);
	ImVec4 mixed = ImVec4(
		bgCol.x + (accCol.x - bgCol.x) * prog,
		bgCol.y + (accCol.y - bgCol.y) * prog,
		bgCol.z + (accCol.z - bgCol.z) * prog,
		bgCol.w + (accCol.w - bgCol.w) * prog
	);

	ImU32 boxCol = ImGui::GetColorU32(mixed);
	draw->AddRectFilled(pos, end, boxCol, 4.0f);

	// Animated check: draw partial lines based on progress
	if (prog > 0.001f) {
		ImU32 checkCol = IM_COL32(230, 230, 230, (int)(255 * prog));

		ImVec2 p1 = ImVec2(pos.x + 4.0f, pos.y + 10.0f);
		ImVec2 p2 = ImVec2(pos.x + 8.0f, pos.y + 14.0f);
		ImVec2 p3 = ImVec2(pos.x + 16.0f, pos.y + 6.0f);

		ImVec2 mid1 = ImVec2(
			p1.x + (p2.x - p1.x) * prog,
			p1.y + (p2.y - p1.y) * prog
		);
		ImVec2 mid2 = ImVec2(
			p2.x + (p3.x - p2.x) * prog,
			p2.y + (p3.y - p2.y) * prog
		);

		float thickness = 2.0f * (0.6f + 0.4f * prog);
		draw->AddLine(p1, mid1, checkCol, thickness);
		draw->AddLine(p2, mid2, checkCol, thickness);
	}

	ImGui::SameLine();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
	ImGui::TextUnformatted(label);
	return v;
}

void windowdropshadow() {
	// draw a subtle frame around the window
	ImDrawList* draw = ImGui::GetWindowDrawList();
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 pos = ImVec2(windowPos.x - 1.0f, windowPos.y - 1.0f);
	ImVec2 end = ImVec2(windowPos.x + windowSize.x + 1.0f, windowPos.y + windowSize.y + 1.0f);
	ImU32 shadowCol = IM_COL32(0, 0, 0, 100);
	draw->AddRect(pos, end, shadowCol, 4.0f);
}

int main() {
	Labrinth::Render.Init();

	Labrinth::UE::FName test(0);
	std::println("Test FName: {}", test.ToString());

	bool showSkeleton = false;

	Labrinth::UE::Init();

	auto pUWorld = Labrinth::UE::GWorld;
	std::println("GWorld: {:p}", (void*)pUWorld);

	while (true) {
		Labrinth::Render.BeginFrame();

		watermark();

		try {
			Labrinth::UE::Init();
			
			auto players = Labrinth::UE::GWorld
				->GameState
				->PlayerArray;

			auto localPlayer = Labrinth::UE::GWorld
				->OwningGameInstance
				->LocalPlayers[0];

			auto cam = localPlayer
				->PlayerController
				->PlayerCameraManager;

			static Labrinth::UE::APG_PlayerState_Game* localPlayerState = nullptr;
			if (!localPlayerState) {
				for (auto player : players) {
					auto pawn = player->PawnPrivate;
					if (pawn == localPlayer->PlayerController->AcknowledgedPawn) {
						localPlayerState = player;
						break;
					}
				}
			}

			const auto gun = localPlayerState->InventoryComponent->CurrentGun;

			gun->TimeBetweenShots = 0.01f;
			gun->AccuracyHip = 0.0f;
			gun->CurrentSpread = 0.0f;
			gun->GunUpRecoil = 0.0f;
			
			gun->CurrentMagazineAmmo = gun->MaxMagazineAmmo;
			gun->CurrentStockAmmo = gun->MaxStockAmmo;
			
			gun->GunShootingType = Labrinth::UE::EGunShootingType::AUTO;

			for (auto player : players) {
				auto pawn = player->PawnPrivate;

				if (pawn == localPlayer->PlayerController->AcknowledgedPawn) {
					localPlayerState = player;
					continue;
				}

				if (player->Team == localPlayerState->Team)
					continue;

				auto loc = pawn->RootComponent->RelativeLocation;
				auto res = cam->WorldToScreen(loc, 1920, 1080);

				if (res) {
					ImGui::GetBackgroundDrawList()->AddCircleFilled(
						res->ToImVec2(),
						2.0f,
						IM_COL32(255, 64, 64, 255)
					);

					ImGui::GetBackgroundDrawList()->AddLine(
						ImVec2(960.0f, 0.0f),
						res->ToImVec2(),
						IM_COL32(255, 64, 64, 255),
						1.0f
					);
				}
			}
		}
		catch (const std::exception& e) {
			//ImGui::Text("Error reading player data: %s", e.what());
			ImGui::SetNextWindowPos(ImVec2(10.0f, 50.0f));
			ImGui::SetNextWindowSize(ImVec2(400.0f, 0.0f));
			ImGui::Begin("Error", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoResize
			);
			windowheader("Error");
			windowdropshadow();
			std::string msg = "Error reading player data: ";
			msg += e.what();
			ImGui::TextWrapped("%s", msg.c_str());
			ImGui::End();
		}

		if (Labrinth::Render.running) {
			ImGui::Begin("Labrinth Menu", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoResize
			);

			windowheader("Labrinth");
			windowdropshadow();

			//checkbox("Show Skeleton", showSkeleton);

			ImGui::Text("Owning Game Instance: 0x%p", (void*)pUWorld->OwningGameInstance);

			//Labrinth::UE::FVector playerLocation = pUWorld->OwningGameInstance->LocalPlayers[0]->PlayerController->AcknowledgedPawn->RootComponent->RelativeLocation;
			//ImGui::Text("Player Location: X=%.2f Y=%.2f Z=%.2f", playerLocation.X, playerLocation.Y, playerLocation.Z);


			ImGui::End();
		}

		Labrinth::Render.EndFrame();
	}

	return 0;
}
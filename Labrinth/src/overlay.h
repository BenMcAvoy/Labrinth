#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#pragma comment(lib, "d3d11.lib")

#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

#include <Windows.h>
#include <dwmapi.h>
#include <d3d11.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_freetype.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "config.h"

namespace Labrinth {

struct RenderData {
    HWND window = nullptr;
    WNDCLASSEX wndclass{};
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* deviceCtx = nullptr;
    ID3D11RenderTargetView* renderTarget = nullptr;
    IDXGISwapChain* swapchain = nullptr;
};

class Render_t {
public:
    Render_t() = default;
    ~Render_t();

    bool running = false;

    void Init();
    void BeginFrame();
    void EndFrame();

    RenderData renderData{};

private:
    bool InitDevice();
    bool InitWindow();
    bool InitImGui();

    void DeinitDevice();
    void DeinitWindow();
    void DeinitImGui();
};

static inline Render_t Render;

} // namespace Vermilion

// ------------------------------------------------------------
// Implementation
// ------------------------------------------------------------

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

static LRESULT CALLBACK Vermilion_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;

    case WM_SYSKEYDOWN:
        if (wParam == VK_F4) {
            DestroyWindow(hwnd);
            return 0;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_CLOSE:
        return 0;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

static void ApplyAccentColor(const ImColor& accent) {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4 a = accent.Value;

    style.Colors[ImGuiCol_CheckMark] = a;
    style.Colors[ImGuiCol_SliderGrab] = a;
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(a.x*1.1f, a.y*1.1f, a.z*1.1f, a.w);

    style.Colors[ImGuiCol_Button] = ImVec4(a.x*0.85f, a.y*0.85f, a.z*0.85f, a.w);
    style.Colors[ImGuiCol_ButtonHovered] = a;
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(a.x*1.15f, a.y*1.15f, a.z*1.15f, a.w);

    style.Colors[ImGuiCol_Header] = ImVec4(a.x*0.8f, a.y*0.8f, a.z*0.8f, a.w);
    style.Colors[ImGuiCol_HeaderHovered] = a;
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(a.x*1.1f, a.y*1.1f, a.z*1.1f, a.w);

    style.Colors[ImGuiCol_Tab] = ImVec4(a.x*0.75f, a.y*0.75f, a.z*0.75f, a.w);
    style.Colors[ImGuiCol_TabHovered] = a;
    style.Colors[ImGuiCol_TabActive] = ImVec4(a.x*1.1f, a.y*1.1f, a.z*1.1f, a.w);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(a.x*0.5f, a.y*0.5f, a.z*0.5f, a.w);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(a.x*0.85f, a.y*0.85f, a.z*0.85f, a.w);

    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(a.x*0.6f, a.y*0.6f, a.z*0.6f, a.w);
    style.Colors[ImGuiCol_ResizeGripHovered] = a;
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(a.x*1.2f, a.y*1.2f, a.z*1.2f, a.w);

    style.Colors[ImGuiCol_TitleBg] = ImVec4(a.x*0.6f, a.y*0.6f, a.z*0.6f, a.w);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(a.x*0.9f, a.y*0.9f, a.z*0.9f, a.w);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(a.x*0.4f, a.y*0.4f, a.z*0.4f, a.w);

    style.WindowBorderSize = 0.0f;
}

// ------------------------------------------------------------
// Render_t
// ------------------------------------------------------------

namespace Labrinth {
    Render_t::~Render_t() {
        DeinitImGui();
        DeinitWindow();
        DeinitDevice();
    }

    bool Render_t::InitWindow() {
        renderData.wndclass.cbSize = sizeof(renderData.wndclass);
        renderData.wndclass.style = CS_CLASSDC;
        renderData.wndclass.lpszClassName = L"Vermilion";
        renderData.wndclass.hInstance = GetModuleHandleA(nullptr);
        renderData.wndclass.lpfnWndProc = Vermilion_WndProc;

        RegisterClassExW(&renderData.wndclass);

        renderData.window = CreateWindowExW(
            WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
            L"Vermilion",
            L"Vermilion",
            WS_POPUP,
            0, 0,
            GetSystemMetrics(SM_CXSCREEN),
            GetSystemMetrics(SM_CYSCREEN),
            nullptr, nullptr,
            renderData.wndclass.hInstance,
            nullptr
        );

        if (!renderData.window)
            return false;

        SetLayeredWindowAttributes(renderData.window, RGB(0,0,0), 255, LWA_ALPHA);

        RECT client{}, window{};
        GetClientRect(renderData.window, &client);
        GetWindowRect(renderData.window, &window);

        POINT diff{};
        ClientToScreen(renderData.window, &diff);

        MARGINS margins{
            window.left + (diff.x - window.left),
            window.top + (diff.y - window.top),
            window.right,
            window.bottom
        };

        DwmExtendFrameIntoClientArea(renderData.window, &margins);

        ShowWindow(renderData.window, SW_SHOW);
        UpdateWindow(renderData.window);

        return true;
    }

    bool Render_t::InitDevice() {
        DXGI_SWAP_CHAIN_DESC scd{};
        scd.BufferCount = 1;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.OutputWindow = renderData.window;
        scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        scd.Windowed = TRUE;
        scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        scd.SampleDesc.Count = 2;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

        D3D_FEATURE_LEVEL featureLevel;
        D3D_FEATURE_LEVEL levels[] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_0
        };

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            levels, 2,
            D3D11_SDK_VERSION,
            &scd,
            &renderData.swapchain,
            &renderData.device,
            &featureLevel,
            &renderData.deviceCtx
        );

        if (hr == DXGI_ERROR_UNSUPPORTED) {
            hr = D3D11CreateDeviceAndSwapChain(
                nullptr,
                D3D_DRIVER_TYPE_WARP,
                nullptr,
                0,
                levels, 2,
                D3D11_SDK_VERSION,
                &scd,
                &renderData.swapchain,
                &renderData.device,
                &featureLevel,
                &renderData.deviceCtx
            );
        }

        if (FAILED(hr))
            MessageBoxA(nullptr, "This software can not run on your computer.", "Critical Problem", MB_ICONERROR);

        ID3D11Texture2D* backbuffer{};
        renderData.swapchain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));

        if (!backbuffer)
            return false;

        renderData.device->CreateRenderTargetView(backbuffer, nullptr, &renderData.renderTarget);
        backbuffer->Release();

        return true;
    }

    bool Render_t::InitImGui() {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        float scale =
            ImGui_ImplWin32_GetDpiScaleForMonitor(
                MonitorFromPoint(POINT{0,0}, MONITOR_DEFAULTTOPRIMARY));

        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;

        ImGui::GetStyle().ScaleAllSizes(scale);
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 14.0f * scale);
        io.Fonts->Build();

        ApplyAccentColor(Labrinth::AccentColour);

        if (!ImGui_ImplWin32_Init(renderData.window))
            return false;

        if (!ImGui_ImplDX11_Init(renderData.device, renderData.deviceCtx))
            return false;

        return true;
    }

    void Render_t::DeinitDevice() {
        if (renderData.renderTarget) renderData.renderTarget->Release();
        if (renderData.swapchain) renderData.swapchain->Release();
        if (renderData.deviceCtx) renderData.deviceCtx->Release();
        if (renderData.device) renderData.device->Release();
    }

    void Render_t::DeinitWindow() {
        DestroyWindow(renderData.window);
        UnregisterClassW(L"Vermilion", renderData.wndclass.hInstance);
    }

    void Render_t::DeinitImGui() {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void Render_t::Init() {
        InitWindow();
        InitDevice();
        InitImGui();
    }

    void Render_t::BeginFrame() {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (GetAsyncKeyState(VK_HOME) & 1) {
            running = !running;

            if (running)
                SetWindowLong(renderData.window, GWL_EXSTYLE, WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT);
            else
                SetWindowLong(renderData.window, GWL_EXSTYLE, WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_LAYERED);
        }
    }

    void Render_t::EndFrame() {
        ImGui::Render();

        float clearColour[4]{0,0,0,0};

        renderData.deviceCtx->OMSetRenderTargets(1, &renderData.renderTarget, nullptr);
        renderData.deviceCtx->ClearRenderTargetView(renderData.renderTarget, clearColour);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        //renderData.swapchain->Present(1, 0);
        renderData.swapchain->Present(0, 0);
    }
}
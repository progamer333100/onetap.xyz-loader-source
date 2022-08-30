#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")
#include "../imgui/imgui.h"
#include "../auth.h"

inline const std::string name = "[REDACTED]";
inline const std::string ownerid = "[REDACTED]";
inline const std::string secret = "[REDACTED]";
inline const std::string version = "[REDACTED]";
inline const std::string url = "[REDACTED]";
inline const std::string sslPin = "[REDACTED]";

namespace ui {
	void init(LPDIRECT3DDEVICE9);
	void render();
}

namespace ui {
	inline LPDIRECT3DDEVICE9 dev;
	inline const char* window_title = ":)";
}

namespace ui {
	inline auto* logo_tex = LPDIRECT3DTEXTURE9();
	inline auto* arrow_tex = LPDIRECT3DTEXTURE9();
	inline ImVec2 screen_res{ 000, 000 };
	inline ImVec2 window_pos{ 0, 0 };
	inline ImVec2 window_size{ 500, 320 };
	inline DWORD  window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;
	inline KeyAuth::api auth(name, ownerid, secret, version, url, sslPin);
}
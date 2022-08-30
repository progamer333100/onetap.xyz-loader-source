#include "ui.hh"
#include "../globals.hh"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imspinner.h"
#include "../logo.h"
#include "../arrow.h"
#include "../injection/injection.h"

#include <fstream>

void AlignForWidth(float width, float alignment = 0.5f)
{
    ImGuiStyle& style = ImGui::GetStyle();
    float avail = ImGui::GetContentRegionAvail().x;
    float off = (avail - width) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
}

std::string tm_to_readable_time(tm ctx) {
    char buffer[80];

    strftime(buffer, sizeof(buffer), "%m/%d/%y", &ctx);

    return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
    auto cv = strtol(timestamp.c_str(), NULL, 10); // long

    return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
    std::tm context;

    localtime_s(&context, &timestamp);

    return context;
}

void injection_thread()
{


    if (!FindWindowA("Valve001", nullptr))
    {

        globals.info_text = "Opening csgo";

        if (globals.cur_selec == 0) // ot.xyz release
            system("cd C://Program Files (x86)//Steam && steam.exe -applaunch 730 -insecure");
        else if (globals.cur_selec == 1 && !globals.user_is_alpha) // lunarware
            system("cd C://Program Files (x86)//Steam && steam.exe -applaunch 730");
        else if (globals.cur_selec == 1 && globals.user_is_alpha) // ot.xyz alpha
            system("cd C://Program Files (x86)//Steam && steam.exe -applaunch 730 -insecure");
        else if (globals.cur_selec == 2)
            system("cd C://Program Files (x86)//Steam && steam.exe -applaunch 730");

    }

    globals.info_text = "Waiting...";

    while (!FindWindowA("Valve001", NULL))
    {
        Sleep(100);
    }

    ManualMap();

    globals.info_text = "Done!";

    Sleep(1200);

    globals.info_text = "Exiting...";

    Sleep(2500);

    exit(0);

}

void login_thread()
{
   
    ui::auth.login(globals.user_name, globals.pass_word);

    if (ui::auth.data.success & 1)
    {

        globals.show_new_ui = true;
        globals.sucessful_login = true;
        globals.done_logging_in = true;
        globals.logging_in = false;
        globals.show_new_ui = true;

        if (globals.checkbox)
        {

            CreateDirectoryA("C:\\onetap\\Loader\\", NULL);
            std::ofstream f("C:\\onetap\\Loader\\login");
            f << globals.pass_word << "|||" << globals.user_name;

        }

        auto yes = [&](const char* v1) {
            bool v55 = strcmp(globals.user_name, v1) == 0;
            bool v56 = strcmp(globals.autolog_username.c_str(), v1) == 0;
            if (v55)
                return true;
            if (v56)
                return true;
            return false;
        };

        if (yes("nerdie") || yes("zinc") || yes("Wixyy") || yes("mitsu") || yes("Pancake") || yes("genocide2") || yes("zNq") || yes("zenquil") || yes("PiXGG") || yes("Nexii"))
        {
            globals.user_is_alpha = true;
        }
        else
        {
            globals.user_is_alpha = false;
        }


    } // good login, go ahead

    else
    {

        globals.sucessful_login = false;
        globals.done_logging_in = true;
        globals.logging_in = false;
        globals.show_new_ui = false;

    }

}

namespace ImGui
{

    bool ArrowButton(std::string label)
    {

        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label.c_str());
        const ImVec2 label_size = CalcTextSize(label.c_str(), NULL, true);
        ImVec2 size_arg = ImVec2(180, 36);
        ImGuiButtonFlags flags = ImGuiButtonFlags_None;

        ImVec2 pos = window->DC.CursorPos;
        if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
           pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
        ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

        const ImRect bb(pos, pos + size);
        ItemSize(size, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;

        if (g.CurrentItemFlags & ImGuiItemFlags_ButtonRepeat)
            flags |= ImGuiButtonFlags_Repeat;
        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

        // Render
        const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        RenderNavHighlight(bb, id);
        RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

        if (g.LogEnabled)
            LogSetNextTextDecoration("[", "]");
        //RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label.c_str(), NULL, &label_size, style.ButtonTextAlign, &bb);
        //ImGui::RenderArrow(bb.Min + style.FramePadding, ImGuiDir_Right);
        ImVec4 vol = ImVec4(59 / 255.f, 47 / 255.f, 71 / 255.f, 255.f / 255.f);
        ImVec2 button_middle = ImVec2((bb.Min.x + bb.Max.x) / 2,( bb.Min.y + bb.Max.y) / 2);
        window->DrawList->AddLine(button_middle, button_middle + ImVec2(15, 0), ImGui::ColorConvertFloat4ToU32(vol));
        window->DrawList->AddLine(button_middle + ImVec2(0, 1), button_middle + ImVec2(15, 1), ImGui::ColorConvertFloat4ToU32(vol));

        window->DrawList->AddLine(button_middle + ImVec2(15, 0), button_middle + ImVec2(6, 4), ImGui::ColorConvertFloat4ToU32(vol));

        // Automatically close popups
        //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
        //    CloseCurrentPopup();

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
        return pressed;

    }

    void Spinner(std::string label)
    {

        ImSpinner::Spinner(label.c_str(), 8, 4, ImColor(189, 150, 220), 8);

    }

    void InfoBox(std::string text)
    {
        
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(24 / 255.f, 23 / 255.f, 35 / 255.f, 255.f / 255.f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(128 / 255.f, 124 / 255.f, 156 / 255.f, 255.f / 255.f));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.f);
        ImGui::BeginChild("##infobox", ImVec2(180, 40));
        ImVec2 text_sz = ImGui::CalcTextSize(text.c_str());
        ImVec2 box_sz = ImGui::GetWindowSize();
        ImGui::SetCursorPos(ImVec2((box_sz.x - text_sz.x) / 6 - 14, (box_sz.y - text_sz.y) / 2));
        ImGui::Text(text.c_str());
        ImGui::EndChild();
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();

    }

    void TextWithHeader(std::string header, std::string text, ImVec2 pos)
    {

        ImVec2 text_sz = ImGui::CalcTextSize(text.c_str());
        ImVec2 header_sz = ImGui::CalcTextSize(header.c_str());
        ImVec2 combined_sz = ImVec2(text_sz.x + header_sz.x, text_sz.y + text_sz.y);
        ImVec2 pad = ImVec2(10, 10);
        ImVec2 window_sz = combined_sz + pad;
        ImGui::SetCursorPos(pos);
        std::string v1 = std::string("##" + header);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.f, 1.f, 1.f, 0.f));
        ImGui::BeginChild(v1.c_str(), window_sz);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(110 / 255.f, 110 / 255.f, 110 / 255.f, 255 / 255.f));
        ImGui::Text(header.c_str());
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(129 / 255.f, 124 / 255.f, 157 / 255.f, 255 / 255.f));
        ImGui::Text(text.c_str());
        ImGui::PopStyleColor(3);
        ImGui::EndChild();

    }

}

void ui::render() {
    if (!globals.active) return;

    if (globals.auto_login)
    {

        auth.login(globals.autolog_username, globals.autolog_password);

        if (ui::auth.data.success & 1)
        {

            globals.show_new_ui = true;
            globals.sucessful_login = true;
            globals.done_logging_in = true;
            globals.logging_in = false;
            globals.show_new_ui = true;

            auto yes = [&](const char* v1) {
                bool v55 = strcmp(globals.user_name, v1) == 0;
                bool v56 = strcmp(globals.autolog_username.c_str(), v1) == 0;
                if (v55)
                    return true;
                if (v56)
                    return true;
                return false;
            };

            if (yes("nerdie") || yes("zinc") || yes("Wixyy") || yes("mitsu") || yes("Pancake") || yes("genocide2") || yes("zNq") || yes("zenquil") || yes("PiXGG") || yes("Nexii"))
            {
                globals.user_is_alpha = true;
            }
            else
            {
                globals.user_is_alpha = false;
            }

        } // good login, go ahead

        else
        {

            globals.sucessful_login = false;
            globals.done_logging_in = true;
            globals.logging_in = false;
            globals.show_new_ui = false;

        }

        globals.auto_login = false;

    }

    static std::string info_text = "";

    ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));

    ImGui::Begin("##window", &globals.active, window_flags);

    if (globals.show_new_ui & 1)
    {

        ImGui::SetCursorPos(ImVec2(window_size.x - 34, 10));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(44 / 255.f, 42 / 255.f, 62 / 255.f, 255.f / 255.f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(18 / 255.f, 17 / 255.f, 27 / 255.f, 255.f / 255.f));
        if (ImGui::Button("x", ImVec2(24, 24)))
            globals.active = false;
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

        ImGui::SetCursorPosX(95);
        ImGui::SetCursorPosY(203);

        ImGui::InfoBox(globals.info_text);
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(59 / 255.f, 47 / 255.f, 71 / 255.f, 255.f / 255.f));
        if (ImGui::ArrowButtonEx("", ImGuiDir_Right, ImVec2(180, 40)))
        {

            if (!globals.done_injecting && !globals.is_injecting)
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)injection_thread, NULL, NULL, NULL);

        }
        ImGui::PopStyleColor();

        if (globals.user_is_alpha)
        {

            ImGui::SetCursorPosX(95);
            ImGui::SetNextItemWidth(366);
            ImGui::Combo("##lol", &globals.cur_selec, "Onetap.xyz\0Onetap.xyz alpha\0Lunarware");

        }
        else
        {

            ImGui::SetCursorPosX(95);
            ImGui::SetNextItemWidth(366);
            ImGui::Combo("##lol", &globals.cur_selec, "Onetap.xyz\0Lunarware");

        }

        // 370

        ImGui::TextWithHeader("Update", "8/26/2022", ImVec2(95, 151));
        ImGui::TextWithHeader("Sub expires", tm_to_readable_time(timet_to_tm(string_to_timet(auth.data.expiry))), ImVec2(250, 151));
        ImGui::TextWithHeader("Status", "Undetected", ImVec2(370, 151));

        ImGui::SetCursorPosY(97);
        ImGui::SetCursorPosX(95);

        if (globals.user_is_alpha)
            ImGui::Text("Welcome back, %s [alpha]", auth.data.username.c_str());
        else
            ImGui::Text("Welcome back, %s", auth.data.username.c_str());

    }
    else if (!globals.needs_to_register && !globals.needs_to_upgrade)
    {

        ImGui::SetCursorPos(ImVec2(window_size.x / 4, window_size.y / 8)); // magic numbers
        ImGui::Image(reinterpret_cast<void*>(logo_tex), ImVec2(300, 300)); // remove it so you can add your own pastes' branding

        ImGui::SetCursorPos(ImVec2(window_size.x / 2, 0));
        ImGui::SetNextWindowBgAlpha(0.99f);
        ImGui::BeginChild("##controls", ImVec2(400, window_size.y), false, window_flags);

        ImVec2 child_size = ImGui::GetWindowSize();

        ImGui::SetCursorPosY(81);
        ImGui::PushItemWidth(180.f);
        AlignForWidth(334.f);
        ImGui::InputText("Username", globals.user_name, sizeof(globals.user_name));
        AlignForWidth(334.f);
        ImGui::InputText("Password", globals.pass_word, sizeof(globals.pass_word), ImGuiInputTextFlags_Password);
        AlignForWidth(334.f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));
        ImGui::Checkbox("Save Login", &globals.checkbox);
        ImGui::PopStyleVar();
        AlignForWidth(334.f);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(59 / 255.f, 47 / 255.f, 71 / 255.f, 255.f / 255.f));
        if (ImGui::ArrowButtonEx("", ImGuiDir_Right, ImVec2(180, 40)) && !globals.logging_in)
        {

            globals.logging_in = true;
            globals.done_logging_in = false;
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)login_thread, NULL, NULL, NULL);

        }

        AlignForWidth(334.f);
        if (ImGui::Button("Register", ImVec2(180 / 2, 40)))
        {

            globals.needs_to_register = true;
            globals.done_registering = false;

        }
        ImGui::SameLine();
        if (ImGui::Button("Upgrade", ImVec2((180 / 2)-6, 40)))
        {

            globals.needs_to_upgrade = true;
            globals.done_upgrading = false;

        }

        if (!globals.sucessful_login && !auth.data.success)
            info_text = auth.data.message;

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.2f, 0.3f, 1.f));
        AlignForWidth(334.f);
        ImGui::Text(info_text.c_str());
        ImGui::PopStyleColor();

        //ImGui::Combo("##none", &globals.cur_selec, globals.combo);
        //ImGui::SliderInt("##none", &globals.cur_selec, 0, 10);

        ImGui::PopStyleColor();
        ImGui::PopItemWidth();

        ImGui::SetCursorPos(ImVec2(244 - 34, 10));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(44 / 255.f, 42 / 255.f, 62 / 255.f, 255.f / 255.f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(18 / 255.f, 17 / 255.f, 27 / 255.f, 255.f / 255.f));
        if (ImGui::Button("x", ImVec2(24, 24)))
            globals.active = false;

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);
        ImGui::EndChild();

    }

    if (!globals.done_logging_in && globals.logging_in)
    {

        ImGui::OpenPopup("##bb");

        if (ImGui::BeginPopupModal("##bb"))
        {

            ImGui::Spinner("#lol");
            ImGui::SameLine();
            ImGui::Text("Logging in");
            ImGui::EndPopup();

        }

    }

    if (!globals.done_upgrading && globals.needs_to_upgrade)
    {

        ImGui::OpenPopup("##vvs");

        if (ImGui::BeginPopupModal("##vvs"))
        {

            ImGui::PushItemWidth(180.f);
            AlignForWidth(334.f);
            ImGui::Text("Use your new key");
            AlignForWidth(334.f);
            ImGui::InputText("Username", globals.user_name, sizeof(globals.user_name));
            AlignForWidth(334.f);
            ImGui::InputText("Key", globals.user_key, sizeof(globals.user_key));
            AlignForWidth(334.f);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(59 / 255.f, 47 / 255.f, 71 / 255.f, 255.f / 255.f));
            if (ImGui::ArrowButtonEx("", ImGuiDir_Right, ImVec2(180, 40)))
            {
                auth.upgrade(globals.user_name, globals.user_key);
                if (auth.data.success)
                {
                    globals.needs_to_upgrade = false;
                    globals.done_upgrading = true;
                    globals.show_new_ui = true;
                }
                else
                {
                    info_text = auth.data.message;
                    globals.needs_to_upgrade = false;
                    globals.done_upgrading = false;
                }
            }
            if (ImGui::Button("Exit", ImVec2(180, 40)))
            {
                globals.needs_to_upgrade = false;
            }
            ImGui::PopItemWidth();
            ImGui::PopStyleColor();
            ImGui::EndPopup();

        }

    }

    if (!globals.done_registering && globals.needs_to_register)
    {

        ImGui::OpenPopup("##vv");

        if (ImGui::BeginPopupModal("##vv"))
        {

            ImGui::PushItemWidth(180.f);
            AlignForWidth(334.f);
            ImGui::Text("Please Register");
            AlignForWidth(334.f);
            ImGui::InputText("Username", globals.user_name, sizeof(globals.user_name));
            AlignForWidth(334.f);
            ImGui::InputText("Password", globals.pass_word, sizeof(globals.pass_word));
            AlignForWidth(334.f);
            ImGui::InputText("Key", globals.user_key, sizeof(globals.user_key));
            AlignForWidth(334.f);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(59 / 255.f, 47 / 255.f, 71 / 255.f, 255.f / 255.f));
            if (ImGui::ArrowButtonEx("", ImGuiDir_Right, ImVec2(180, 40)))
            {
                auth.regstr(globals.user_name, globals.pass_word, globals.user_key);
                if (auth.data.success)
                {
                    globals.needs_to_register = false;
                    globals.done_registering = true;
                    globals.show_new_ui = true;
                }
                else
                {
                    info_text = auth.data.message;
                    globals.needs_to_register = false;
                    globals.done_registering = false;
                }
            }
            if (ImGui::Button("Exit", ImVec2(180, 40)))
            {
                globals.needs_to_register = false;
            }
            ImGui::PopItemWidth();
            ImGui::PopStyleColor();
            ImGui::EndPopup();

        }

    }

    ImGui::End();

}

void ui::init(LPDIRECT3DDEVICE9 device) {
    dev = device;
	
    // colors
    ImGui::StyleColorsDark();

	if (window_pos.x == 0) {
		RECT screen_rect{};
		GetWindowRect(GetDesktopWindow(), &screen_rect);
		screen_res = ImVec2(float(screen_rect.right), float(screen_rect.bottom));
		window_pos = (screen_res - window_size) * 0.5f;

        D3DXCreateTextureFromFileInMemory(device, logo_bin, sizeof(logo_bin), &logo_tex);
        D3DXCreateTextureFromFileInMemory(device, arrow_bin, sizeof(arrow_bin), &arrow_tex);

	}
}
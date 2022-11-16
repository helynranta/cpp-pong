#include "menu.hpp"

#include "game.hpp"

namespace lerppana::pong
{
    void menu::initialise() { }

    void menu::start() { }

    void menu::update(core::dt_t dt) { }

    void menu::on_gui()
    {
        const auto score_font = font_manager->get_font("scorefont");

        const auto padding = 4;

        ImGui::SetWindowFontScale(1.0f);

        {
            ImGui::FontScope font_scope(score_font);

            ImGui::StyleColorScope button_color_scope(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::StyleColorScope button_hover_color_scope(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 0.6f));
            ImGui::StyleColorScope button_active_color_scope(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.8f, 0.8f, 0.8f));

            const auto font_height = ImGui::GetFontSize();
            static auto width = 100.0f;

            ImGui::SetCursorPos(
                ImVec2(ImGui::GetWindowWidth() / 2 - width / 2, ImGui::GetWindowHeight() / 2 - font_height - padding));

            ImGui::GroupScope group;
            if (ImGui::Button("Start Game"))
            {
                scene_orchestrator_state->request_scene_transition("fs1://scenes/game.scene");
                audio_manager->play_clip("fs1://audio/Minimalist2.wav");
            }

            static auto was_button_hovered = false;
            auto is_hovered = ImGui::IsItemHovered();
            if (!was_button_hovered && is_hovered)
            {
                audio_manager->play_clip("fs1://audio/Minimalist7.wav");
                was_button_hovered = true;
            }
            else if (was_button_hovered && !is_hovered)
            {
                was_button_hovered = false;
            }

            width = ImGui::GetItemRectSize().x;
        }
        {
            const auto font_height = ImGui::GetFontSize();
            static auto width = 100.0f;

            ImGui::SetCursorPos(
                ImVec2(ImGui::GetWindowWidth() / 2 - width / 2, ImGui::GetWindowHeight() / 2 + font_height + padding));

            ImGui::FontScope font_scope(score_font);

            ImGui::StyleColorScope button_color_scope(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::StyleColorScope button_hover_color_scope(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 0.6f));
            ImGui::StyleColorScope button_active_color_scope(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.8f, 0.8f, 0.8f));

            ImGui::GroupScope group;
            if (ImGui::Button("Exit"))
            {
                scene_orchestrator_state->request_application_quit();
            }

            static auto was_button_hovered = false;
            auto is_hovered = ImGui::IsItemHovered();
            if (!was_button_hovered && is_hovered)
            {
                audio_manager->play_clip("fs1://audio/Minimalist7.wav");
                was_button_hovered = true;
            }
            else if (was_button_hovered && !is_hovered)
            {
                was_button_hovered = false;
            }

            width = ImGui::GetItemRectSize().x;
        }
    }
}

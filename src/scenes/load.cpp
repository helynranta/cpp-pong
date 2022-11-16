#include "load.hpp"

namespace lerppana::pong
{
    void load::on_gui()
    {
        const auto score_font = font_manager->get_font("scorefont");
        ImGui::FontScope font_scope(score_font);

        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - 200, ImGui::GetWindowHeight() / 2 - 5));

        ImGui::SetWindowFontScale(1.0f);

        ImGui::GroupScope group;

        ImGui::ProgressBar(load_progress, ImVec2(400, 20), "");
    }

    void load::start()
    {
        renderer->skybox_enabled(false);
        renderer->set_background_color({0.1f, 0.1f, 0.1f, 1.0f});

        const auto window_height = window->get_height();

        resource_loader->request_load<resource::texture_resource>(core::texture_grid_256);

        resource_loader->request_load<resource::vk_mesh_resource>(core::mesh_primitive_cube);
        resource_loader->request_load<resource::vk_mesh_resource>(core::mesh_primitive_quad);
        resource_loader->request_load<resource::vk_mesh_resource>(core::mesh_primitive_sphere);
        resource_loader->request_load<resource::texture_resource>("fs0://textures/circle.png");
        resource_loader->request_load<resource::audio_resource>("fs1://audio/Minimalist2.wav");
        resource_loader->request_load<resource::audio_resource>("fs1://audio/Minimalist7.wav");
    }

    void load::update(core::dt_t dt)
    {
        auto count = 0;
        if (resource_loader->is_ready<resource::vk_mesh_resource>(core::mesh_primitive_quad))
            count++;
        if (resource_loader->is_ready<resource::vk_mesh_resource>(core::mesh_primitive_cube))
            count++;
        if (resource_loader->is_ready<resource::vk_mesh_resource>(core::mesh_primitive_sphere))
            count++;
        if (resource_loader->is_ready<resource::texture_resource>(core::texture_grid_256))
            count++;

        load_progress = float(count) / 4.f;

        if (count == 4)
        {
            scene_orchestrator_state->request_scene_transition("fs1://scenes/menu.scene");
        }
    }
}

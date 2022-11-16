#pragma once

#include "engine.hxx"

namespace lerppana::pong
{
    struct [[le::scene]]  load : core::scene
    {
        explicit load(
            std::shared_ptr<engine::framework::window> window,
            std::shared_ptr<engine::vk::renderer> renderer,
            std::shared_ptr<engine::resource::resource_loader> resource_loader,
            std::shared_ptr<engine::core::scene_orchestrator_state> scene_orchestrator_state,
            std::shared_ptr<vk::font_manager> font_manager) :
            scene("fs1://scenes/load.scene"),
            window(std::move(window)),
            renderer(std::move(renderer)),
            resource_loader(std::move(resource_loader)), 
            scene_orchestrator_state(std::move(scene_orchestrator_state)),
            font_manager(std::move(font_manager))
        {
        }

        void on_gui() override;

        void start() override;

        void update(core::dt_t dt) override;

        float load_progress = 0.0f;

      private:
        std::shared_ptr<engine::framework::window> window;
        std::shared_ptr<engine::vk::renderer> renderer;
        std::shared_ptr<engine::resource::resource_loader> resource_loader;
        std::shared_ptr<engine::core::scene_orchestrator_state> scene_orchestrator_state;
        std::shared_ptr<vk::font_manager> font_manager;
    };
}

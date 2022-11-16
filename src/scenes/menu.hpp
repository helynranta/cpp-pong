#pragma once

#include "engine.hxx"

namespace lerppana::pong
{
    struct [[le::scene]] menu : core::scene
    {
        explicit menu(
            std::shared_ptr<engine::vk::renderer> renderer,
            std::shared_ptr<engine::core::scene_orchestrator_state> scene_orchestrator_state,
            std::shared_ptr<engine::vk::font_manager> font_manager,
            std::shared_ptr<engine::framework::audio_manager> audio_manager) :
            scene("fs1://scenes/menu.scene"),
            renderer(std::move(renderer)), 
            scene_orchestrator_state(std::move(scene_orchestrator_state)),
            font_manager(std::move(font_manager)),
            audio_manager(std::move(audio_manager))
        {
        }

        void initialise();

        void start() override;

        void update(core::dt_t dt) override;

        void on_gui() override;

      private:
        std::shared_ptr<vk::renderer> renderer;
        std::shared_ptr<engine::core::scene_orchestrator_state> scene_orchestrator_state;
        std::shared_ptr<engine::vk::font_manager> font_manager;
        std::shared_ptr<engine::framework::audio_manager> audio_manager;
    };
}

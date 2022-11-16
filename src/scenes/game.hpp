#pragma once

#include "engine.hxx"
#include "tags.hpp"

namespace lerppana::pong
{
    enum game_state
    {
        starting,
        playing,
        point,
        ended
    };

    struct [[le::scene]] game : engine::core::scene
    {
        float ball_speed = 12.f;
        float paddle_speed = 500.0f;
        game_state state = game_state::starting;
        std::chrono::high_resolution_clock::time_point round_start_time;
        std::chrono::milliseconds start_delay{2000};
        uint8_t player_1_score = 0u;
        uint8_t player_2_score = 0u;

        const float paddle_shrink_speed = 0.1f;
        const glm::vec3 paddle_original_scale = { 0.5f, 1.5f, 0.5f };
        const glm::vec3 paddle_original_size = {1.0f, 1.5f, 1.0f};
        const glm::vec3 paddle_min_size = {1.0f, 0.8f, 1.0f};
        glm::vec3 paddle_size = paddle_original_size;

#ifdef ENGINE_ENABLE_EDITOR
        inline static bool pong_debug_window_open = true;

        void on_editor(core::dt_t dt) final;
#endif

        explicit game(
            std::shared_ptr<engine::vk::renderer> renderer,
            std::shared_ptr<engine::system::sdl_input_manager> input_manager,
            std::shared_ptr<engine::core::scene_orchestrator_state> scene_orchestrator_state,
            std::shared_ptr<engine::vk::font_manager> font_manager,
            std::shared_ptr<engine::framework::audio_manager> audio_manager) :
            scene("fs1://scenes/game.scene"),
            renderer(std::move(renderer)),
            input_manager(std::move(input_manager)), 
            scene_orchestrator_state(std::move(scene_orchestrator_state)),
            font_manager(std::move(font_manager)),
            audio_manager(std::move(audio_manager))
        {
        }

        void end() override;

        void hide_stage();

        void show_stage();

        void start() override;

        void push_ball();

        void reset_ball();

        void reset_game();

        void update(core::dt_t dt) override;

        void fixed_update(core::dt_t dt) override;

        void on_gui() override;

      private:
        std::shared_ptr<engine::vk::renderer> renderer;
        std::shared_ptr<engine::system::sdl_input_manager> input_manager;
        std::shared_ptr<engine::core::scene_orchestrator_state> scene_orchestrator_state;
        std::shared_ptr<engine::vk::font_manager> font_manager;
        std::shared_ptr<engine::framework::audio_manager> audio_manager;
    };
}

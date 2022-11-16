#pragma once

#include <engine.hxx>

#include <system/simple_particle_system.hpp>

namespace lerppana::pong::systems
{
    struct [[le::system]] ball_particle_system : core::system
    {
        std::shared_ptr<engine::system::simple_particle_system> particle_system;

        std::chrono::high_resolution_clock::time_point last_particle_spawn_time;
        std::chrono::milliseconds particle_spawn_delay = std::chrono::milliseconds(50);
        float particle_speed = 100.f;
        float particle_shrink_speed = 100.f;
        float particle_fade_speed = 100.f;

        const size_t max_spawn_count = 3;

        ball_particle_system(std::shared_ptr<engine::system::simple_particle_system> particle_system);

        void update(core::scene& scene, core::dt_t dt) override;

        void fixed_update(core::scene& scene, core::dt_t dt) override;

        void on_gui(core::scene& scene) override;

#ifdef ENGINE_ENABLE_EDITOR
        void on_editor(core::scene& scene, core::dt_t dt) override;
#endif
    };
}

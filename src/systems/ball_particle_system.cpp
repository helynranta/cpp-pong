#include "systems/ball_particle_system.hpp"

#include "tags.hpp"

namespace lerppana::pong::systems
{
    ball_particle_system::ball_particle_system(std::shared_ptr<engine::system::simple_particle_system> particle_system) :
        particle_system(std::move(particle_system))
    {
        last_particle_spawn_time = std::chrono::high_resolution_clock::now();
        particle_spawn_delay = std::chrono::milliseconds(10);
        particle_shrink_speed = 800.f;
        particle_fade_speed = 60.f;
    }

    void ball_particle_system::update(core::scene& scene, core::dt_t dt) {

    }

    void ball_particle_system::fixed_update(core::scene& scene, core::dt_t dt)
    {
        if (scene.objects->is_tagged(tag::ball) &&
            scene.objects->has_component<engine::component::simple_particle_emitter>(
                scene.objects->get_tagged(tag::ball)))
        {
            auto ball = scene.objects->get_tagged(pong::tag::ball);
            auto& transform = scene.objects->get_tagged_component<engine::component::transform>(tag::ball);
            auto& physics_3d = scene.objects->get_tagged_component<engine::component::physics_3d>(tag::ball);
            auto& emitter = scene.objects->get_tagged_component<engine::component::simple_particle_emitter>(tag::ball);

            while (last_particle_spawn_time + particle_spawn_delay < std::chrono::high_resolution_clock::now())
            {
                last_particle_spawn_time += particle_spawn_delay;
                auto particle = particle_system->spawn(scene.objects, ball, std::chrono::milliseconds(500));
                particle.set_position(transform.get_position());
                if (physics_3d.bt_rigid_body != nullptr)
                {
                    particle.linear_velocity =
                        util::bt_vector3_to_glm(physics_3d.bt_rigid_body->getLinearVelocity()) / particle_speed;
                }
            }

            for (auto i = 0u; i < emitter.alive_particle_count; i++)
            {
                auto& particle = emitter.particles.at(i);

                auto alpha = (float)(glm::mix(particle.data.color.a, 0.0f, dt.count() / particle_fade_speed));
                particle.data.color.a = std::clamp(alpha, 0.0f, 1.0f);
                particle.linear_velocity *= (1 - dt.count() / 1000.0f);

                auto scale = (float)(glm::mix(particle.get_scale(), 0.1f, dt.count() / particle_shrink_speed));
                particle.set_scale(glm::clamp(scale, 0.f, 1.f));
            }
        }
    }

    void ball_particle_system::on_gui(core::scene& scene) { }

#ifdef ENGINE_ENABLE_EDITOR
    void ball_particle_system::on_editor(core::scene& scene, core::dt_t dt)
    { 
        auto window = ImGui::WindowScope("Pong");

        static auto particle_spawn_delay_ms = (int)particle_spawn_delay.count();
        ImGui::InputInt("particle spawn delay ms", &particle_spawn_delay_ms);
        particle_spawn_delay = std::chrono::milliseconds(particle_spawn_delay_ms);
        ImGui::InputFloat("particle speed", &particle_speed);
        ImGui::InputFloat("particle shrink", &particle_shrink_speed);
        ImGui::InputFloat("particle fade", &particle_fade_speed);
    }
#endif
}

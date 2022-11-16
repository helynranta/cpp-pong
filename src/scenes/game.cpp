#include "game.hpp"

#include "menu.hpp"

namespace lerppana::pong
{
    void game::end() { objects->destroy(); }

    void game::hide_stage()
    {
        core::view<component::basic_material>(objects).for_each(
            [](auto entity, auto& material)
            {
                if (entity < 5)
                {
                    return;
                }
                material.uniform_buffer.data.albedo_color.a = 0.0f;
            });
    }

    void game::show_stage()
    {
        core::view<component::basic_material>(objects).for_each(
            [](auto entity, auto& material)
            {
                material.uniform_buffer.data.albedo_color.a = 1.0f;
            });
    }

    void game::start()
    {
        physics_world->set_gravity({0.0f, 0.0f, 0.0f});

        // create ball
        {
            auto ball =
                objects->add_gameobject_with_components<component::transform>();
            objects->tag(ball, tag::ball);
            objects->get_component<component::transform>(ball);
            auto& material =
                objects->add_component(ball, component::basic_material{.texture_ref = core::texture_grid_256});
            material.set_texture_factor(0.0f);
            material.set_albedo_color(glm::vec4(glm::vec3(0.9f), 1.0f));

            objects->add_component(ball, component::mesh_filter{.mesh = core::mesh_primitive_sphere});
            auto& rigid_body = objects->add_component<component::physics_3d>(
                ball,
                component::physics_3d{
                    .bt_collision_shape = new btSphereShape(1.0f),
                    .bt_mass = 0.01f});

            rigid_body.bt_rigid_body->setActivationState(DISABLE_DEACTIVATION);
            rigid_body.bt_rigid_body->setAngularFactor(btVector3(1, 1, 1));
            rigid_body.bt_rigid_body->setLinearFactor(btVector3(0, 1, 1));
            rigid_body.bt_rigid_body->setRestitution(1.0f);
            rigid_body.bt_rigid_body->setFriction(0.0f);

            component::simple_particle_emitter emitter;
            emitter.textures = std::vector<core::resource_key_t>{ "fs0://textures/circle.png" };
            objects->add_component<engine::component::particle_emitter<component::simple_particle>>(ball, emitter);
        }

        // create left paddle
        {
            auto paddle =
                objects->add_gameobject_with_components<component::transform>();

            objects->get_component<component::transform>(paddle).set_scale(paddle_original_scale);

            objects->tag(paddle, core::tag::player_1);
            auto& material =
                objects->add_component(paddle, component::basic_material{.texture_ref = core::texture_grid_256});
            material.set_texture_factor(0.0f);
            material.set_albedo_color(glm::vec4(glm::vec3(0.9f), 1.0f));

            objects->add_component(paddle, component::mesh_filter{.mesh = core::mesh_primitive_cube});
            auto& rigid_body = objects->add_component(
                paddle,
                component::physics_3d{
                    .bt_collision_shape = new btBoxShape(btVector3{0.5f, 1.5f, 0.5f}),
                    .bt_mass = 100.0f});
            rigid_body.bt_rigid_body->getWorldTransform().setOrigin({.0f, .0f, 18.0f});
            rigid_body.bt_rigid_body->setActivationState(DISABLE_DEACTIVATION);
            rigid_body.bt_rigid_body->setAngularFactor(btVector3(0, 0, 0));
            rigid_body.bt_rigid_body->setLinearFactor(btVector3(0, 0, 1));
            rigid_body.bt_rigid_body->setRestitution(1.f);
            rigid_body.bt_rigid_body->setFriction(0.0f);
        }

        // create right paddle
        {
            auto paddle =
                objects->add_gameobject_with_components<component::transform>();

            objects->get_component<component::transform>(paddle).set_scale(paddle_original_scale);

            objects->tag(paddle, core::tag::player_2);

            auto& material =
                objects->add_component(paddle, component::basic_material{.texture_ref = core::texture_grid_256});

            material.set_texture_factor(0.0f);
            material.set_albedo_color(glm::vec4(glm::vec3(0.9f), 1.0f));

            objects->add_component(paddle, component::mesh_filter{.mesh = core::mesh_primitive_cube});
            auto& rigid_body = objects->add_component(
                paddle,
                component::physics_3d{
                    .bt_collision_shape = new btBoxShape(btVector3{0.5f, 1.5f, 0.5f}),
                    .bt_mass = 100.0f});
            rigid_body.bt_rigid_body->getWorldTransform().setOrigin({.0f, .0f, -18.0f});
            rigid_body.bt_rigid_body->setActivationState(DISABLE_DEACTIVATION);
            rigid_body.bt_rigid_body->setAngularFactor(btVector3(0, 0, 0));
            rigid_body.bt_rigid_body->setLinearFactor(btVector3(0, 0, 1));
            rigid_body.bt_rigid_body->setRestitution(1.f);
            rigid_body.bt_rigid_body->setFriction(0.0f);
        }

        // lower bound
        {
            auto wall = objects->add_gameobject_with_components<component::transform>();
            auto& rigid_body = objects->add_component(
                wall,
                component::physics_3d{
                    .bt_collision_shape = new btBoxShape(btVector3{50.0f, 1.0f, 50.0f}),
                    .bt_mass = 0.0f});
            rigid_body.bt_rigid_body->getWorldTransform().setOrigin({0.0f, -16.0f, .0f});
            rigid_body.bt_rigid_body->setRestitution(1.0f);
            rigid_body.bt_rigid_body->setFriction(0.0f);
        }

        // upper bound
        {
            auto wall = objects->add_gameobject_with_components<component::transform>();
            auto& rigid_body = objects->add_component(
                wall,
                component::physics_3d{
                    .bt_collision_shape = new btBoxShape(btVector3{50.0f, 1.0f, 50.0f}),
                    .bt_mass = 0.0f});
            rigid_body.bt_rigid_body->getWorldTransform().setOrigin({0.0f, 16.0f, .0f});
            rigid_body.bt_rigid_body->setRestitution(1.0f);
            rigid_body.bt_rigid_body->setFriction(0.0f);
        }

        // draw background
        {
            for (auto i = 0; i < 14; i++)
            {
                auto item = objects->add_gameobject_with_components();
                auto& transform = objects->add_component<component::transform>(item);
                transform.set_position({0.0f, 10.0f - (i * 1.8f), 0.0f});
                transform.set_scale({.4f, 0.5f, 0.2f});
                transform.set_rotation(glm::rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
                objects->add_component(item, component::mesh_filter{.mesh = core::mesh_primitive_quad});

                auto& material = objects->add_component<component::basic_material>(item);
                material.set_texture_factor(0.0f);
                material.set_albedo_color(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
            }
        }

        round_start_time = std::chrono::high_resolution_clock::now();
        state = game_state::starting;

        {
            auto& transform = objects->get_tagged_component<component::transform>(core::tag::camera);
            transform.look_at(glm::vec3{0.0f});
        }
    }

    void game::push_ball()
    {
        std::srand(std::time(nullptr));
        auto& ball = objects->get_tagged_component<component::physics_3d>(tag::ball);
        ball.bt_rigid_body->applyCentralImpulse(
            btVector3(0.0f, (std::rand() % 200 - 100) / 100.0f, (std::rand() % 200 - 100) / 100.0f).normalize() *
            0.05f);
    }

    void game::reset_ball()
    {
        auto& rigid_body = objects->get_tagged_component<component::physics_3d>(tag::ball);
        rigid_body.bt_rigid_body->getWorldTransform().setOrigin(btVector3{0, 0, 0});
        rigid_body.bt_rigid_body->clearForces();
        rigid_body.bt_rigid_body->setLinearVelocity(btVector3{0, 0, 0});
        rigid_body.bt_rigid_body->setAngularVelocity(btVector3{0, 0, 0});
    }

    void game::reset_game()
    {
        player_1_score = 0;
        player_2_score = 0;
        paddle_size = paddle_original_size;
        round_start_time = std::chrono::high_resolution_clock::now();
        state = game_state::starting;
        reset_ball();
        show_stage();
    }

    void game::update(core::dt_t dt) { }

    void game::fixed_update(core::dt_t dt)
    {
        switch (state)
        {
            case pong::game_state::starting:
                {
                    if (round_start_time + start_delay < std::chrono::high_resolution_clock::now())
                    {
                        push_ball();
                        state = game_state::playing;
                    }
                    else
                    {
                        auto& rigid_body = objects->get_tagged_component<component::physics_3d>(tag::ball);
                        reset_ball();
                        /// auto count_down = std::chrono::high_resolution_clock::now() - round_start_time +
                        /// start_delay; common::console::log(common::console_color::red, "\e[A\r\e[0k",
                        /// count_down.count());
                    }
                    break;
                }
            case pong::game_state::playing:
                {
                    auto& transform = objects->get_tagged_component<component::transform>(tag::ball);

                    if (transform.get_position().z > 30.0f)
                    {
                        player_2_score++;
                        state = game_state::point;
                    }

                    if (transform.get_position().z < -30.0f)
                    {
                        player_1_score++;
                        state = game_state::point;
                    }

                    paddle_size = glm::mix(paddle_size, paddle_min_size, paddle_shrink_speed * dt.count() / 1000.f);
                    break;
                }
            case pong::game_state::point:
                {
                    if (player_1_score >= 3 || player_2_score >= 3)
                    {
                        hide_stage();
                        state = game_state::ended;
                        break;
                    }

                    reset_ball();
                    paddle_size = paddle_original_size;
                    round_start_time = std::chrono::high_resolution_clock::now();
                    state = game_state::starting;
                    break;
                }
            case pong::game_state::ended: break;
            default:
                break;
        }

        // update player_1 paddle
        {
            auto& rigid_body = objects->get_tagged_component<component::physics_3d>(core::tag::player_1);
            btVector3 velocity{0.0f, 0.0f, 0.0f};
            if (input_manager->is_key_down(framework::key::forward))
            {
                velocity.setY(velocity.getY() + paddle_speed * dt.count() / 1000.f);
            }
            else if (input_manager->is_key_down(framework::key::back))
            {
                velocity.setY(velocity.getY() - paddle_speed * dt.count() / 1000.f);
            }

            rigid_body.bt_rigid_body->setLinearVelocity(velocity);
            rigid_body.bt_collision_shape->setLocalScaling(btVector3{paddle_size.x, paddle_size.y, paddle_size.z});
            objects->get_tagged_component<component::transform>(core::tag::player_1)
                .set_scale(paddle_original_scale * glm::vec3{ paddle_size.x, paddle_size.y, paddle_size.z });
        }

        // update player_2 paddle
        {
            auto& rigid_body = objects->get_tagged_component<component::physics_3d>(core::tag::player_2);
            auto& ball = objects->get_tagged_component<component::physics_3d>(tag::ball);

            btVector3 velocity{0.0f, 0.0f, 0.0f};

            auto direction = ball.bt_rigid_body->getWorldTransform().getOrigin().getY() -
                             rigid_body.bt_rigid_body->getWorldTransform().getOrigin().getY();

            velocity.setY(glm::clamp(direction * 3.0f, -(paddle_speed * dt.count() / 1000.f), (paddle_speed * dt.count() / 1000.f)));

            rigid_body.bt_rigid_body->setLinearVelocity(velocity);
            rigid_body.bt_collision_shape->setLocalScaling(btVector3{paddle_size.x, paddle_size.y, paddle_size.z});
            objects->get_tagged_component<component::transform>(core::tag::player_2)
                .set_scale(paddle_original_scale * glm::vec3{ paddle_size.x, paddle_size.y, paddle_size.z });
        }

        // ball max speed
        {
            auto& ball = objects->get_tagged_component<component::physics_3d>(tag::ball);
            if (ball.bt_rigid_body->getLinearVelocity().length() != 0.0f)
            {
                auto speed = ball.bt_rigid_body->getLinearVelocity().length();
                auto velocity = ball.bt_rigid_body->getLinearVelocity().normalized();
                if (velocity.getY() < -0.6f)
                {
                    velocity.setY(std::lerp(velocity.getY(), -0.6f, dt.count() / 1000.f));
                    velocity.normalize();
                }
                else if (0.6f < velocity.getY())
                {
                    velocity.setY(std::lerp(velocity.getY(), 0.6f, dt.count() / 1000.f));
                    velocity.normalize();
                }

                speed = std::lerp(speed, ball_speed, 2.0f * dt.count() / 1000.f);
                ball.bt_rigid_body->setLinearVelocity(speed * velocity);
            }
        }
    }

    void game::on_gui()
    {
        const auto score_font = font_manager->get_font("scorefont");

        // render score
        if (player_1_score < 3 && player_2_score < 3)
        {
            ImGui::SetWindowFontScale(0.35f);

            ImGui::FontScope font_scope(score_font);

            static auto size = ImVec2(25, ImGui::GetFontSize());
            ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - size.x / 2, 10));

            ImGui::GroupScope group;
            ImGui::Text("best of three", player_1_score, player_2_score);
            size = ImGui::GetItemRectSize();

            ImGui::SetWindowFontScale(1.0f);

            static auto size2 = ImVec2(100, ImGui::GetFontSize());
            ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - size2.x / 2, 25));

            ImGui::GroupScope group2;
            ImGui::Text("%d - %d", player_1_score, player_2_score);
            size2 = ImGui::GetItemRectSize();
        }
        else
        {
            ImGui::SetWindowFontScale(0.5f);
            ImGui::FontScope font_scope(score_font);

            const auto padding = 4;

            // winner is text
            {
                auto winner = player_1_score >= 3 ? "player 1" : "player 2";

                const auto font_height = ImGui::GetFontSize();
                static auto width = 100.0f;

                const auto padding = 4;
                ImGui::SetCursorPos(
                    ImVec2(ImGui::GetWindowWidth() / 2 - width / 2, ImGui::GetWindowHeight() / 2 - font_height - padding));

                ImGui::GroupScope group;

                auto text = std::string("winner is ") + winner;
                ImGui::Text(text.c_str());

                width = ImGui::GetItemRectSize().x;
            }

            // retry button
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
                if (ImGui::Button("Retry"))
                {
                    reset_game();
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
        }

        // render exit button
        {
            ImGui::FontScope font_scope(score_font);

            ImGui::SetWindowFontScale(0.5f);
            ImGui::StyleColorScope button_color_scope(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::StyleColorScope button_hover_color_scope(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 0.6f));
            ImGui::StyleColorScope button_active_color_scope(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.8f, 0.8f, 0.8f));

            ImGui::SetCursorPos(ImVec2(20, 20));

            ImGui::GroupScope group;
            if (ImGui::Button("X", ImVec2(40.f, 40.f)))
            {
                scene_orchestrator_state->request_scene_transition("fs1://scenes/menu.scene");
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

            ImGui::SetWindowFontScale(1.0f);
        }
    }

#ifdef ENGINE_ENABLE_EDITOR
    void game::on_editor(core::dt_t dt)
    {
        if (pong_debug_window_open)
        {
            auto& ball = objects->get_tagged_component<component::physics_3d>(tag::ball);

            // if (ball.ptr)
            auto velocity = ball.bt_rigid_body->getLinearVelocity();
            auto speed = ball.bt_rigid_body->getLinearVelocity().length();
            if (velocity.length() > 0.0f)
            {
                velocity = velocity.normalized();
            }
            if (ImGui::WindowScope scope{"Pong", &pong_debug_window_open})
            {
                ImGui::Text("ball speed: %.2f", speed);
                ImGui::Text("ball velocity: %.2f, %.2f", velocity.x(), velocity.y());
                ImGui::Text("paddle size: %.2f", paddle_size.y);

                auto score = (int)player_1_score;
                ImGui::InputInt("player 1 score", &score);
                player_1_score = (uint8_t)score;
            }
        }
    }
#endif
}

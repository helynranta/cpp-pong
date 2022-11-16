#pragma once

#include "pong_export.h"

// reload engine
#include <imgui.hpp>
#include <plugin.hpp>
#include <util.hpp>
#include <common/console.hpp>
#include <component/basic_material.hpp>
#include <component/mesh_filter.hpp>
#include <component/transform.hpp>
#include <component/physics_3d.hpp>
#include <component/projection.hpp>
#include <component/simple_particle.hpp>
#include <component/light/directional.hpp>
#include <core/const.hpp>
#include <core/object_manager.hpp>
#include <core/resource.hpp>
#include <core/scene.hpp>
#include <core/scene_orchestrator_state.hpp>
#include <core/system.hpp>
#include <core/tag.hpp>
#include <core/view.hpp>
#include <framework/audio_manager.hpp>
#include <framework/window.hpp>
#include <physics/physics_world.hpp>
#include <resource/resource_loader.hpp>
#include <system/sdl_input_manager.hpp>
#include <vk/font_manager.hpp>
#include <vk/imgui_context.hpp>
#include <vk/renderer.hpp>
#include <vk/pipeline/skybox.hpp>

#ifdef ENGINE_ENABLE_EDITOR
    #include <editor/tag_editor.hpp>
#endif

// 3rd party
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <Hypodermic/Hypodermic.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>

// STL
#include <chrono>
#include <memory>

using namespace engine;

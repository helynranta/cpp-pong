#include "generated/reflection.generated.hxx"
#include "tags.hpp"

#include <engine.hxx>

using namespace engine;
using namespace lerppana::pong;

struct PONG_EXPORT pong_plugin : engine::plugin
{
    void addRegistrations(std::shared_ptr<Hypodermic::ContainerBuilder> builder) final
    {
        builder->addRegistrations(lerppana::generated::generated_builder{});
    }

    void setupContainer(std::shared_ptr<Hypodermic::Container> container) final
    {
        engine::vk::imgui_context::set_shared_imgui_context();

#if ENGINE_ENABLE_EDITOR
        container->resolve<engine::editor::tag_editor>()->register_tags<lerppana::pong::tag>();
#endif
    }
};

extern "C" PONG_EXPORT pong_plugin plugin;
pong_plugin plugin;
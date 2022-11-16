#include "generated/reflection.generated.hxx"

#include <engine.hxx>

using namespace lerppana::pong;

struct PONG_EXPORT pong_plugin : engine::plugin
{
    void addRegistrations(std::shared_ptr<Hypodermic::ContainerBuilder> builder) final
    {
        builder->addRegistrations(lerppana::generated::generated_builder{});
    }

    void setupContainer(std::shared_ptr<Hypodermic::Container> container) final
    {
        lerppana::generated::generated_builder::setup_container(container);
    }
};

extern "C" PONG_EXPORT pong_plugin plugin;
pong_plugin plugin;
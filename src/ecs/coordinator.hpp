/* The coordinator manages all the ascpets of the ECS environment */

#include "ecs/entitymanager.hpp"
#include "ecs/componentmanager.hpp"
#include "ecs/system.hpp"

#pragma once

class Coordinator
{
public:
    void init()
    {
        componentManager = std::make_unique<ComponentManager>();
        entityManager = std::make_unique<EntityManager>();
        systemManager = std::make_unique<SystemManager>();
    }

    /* Entity methods */
    Entity create_entity()
    {
        return entityManager->create_entity();
    }

    void destory_entity(Entity ent)
    {
        entityManager->destroy_entity(ent);
        componentManager->entity_destroyed(ent);
        systemManager->entity_destroyed(ent);
    }

    /* Component methods */
    template<typename T>
    void register_component()
    {
        componentManager->register_component<T>();
    }

    template<typename T>
    void add_component(Entity ent, T component)
    {
        componentManager->add_component<T>(ent, component);

        auto signature = entityManager->get_signature(ent);
        signature.set(componentManager->get_component_type<T>(), true);
        entityManager->set_signature(ent, signature);

        systemManager->entity_signature_changed(ent, signature);
    }

    template<typename T>
    void remove_component(Entity ent)
    {
        componentManager->remove_component<T>(ent);

        auto signature = entityManager->get_signature(ent);
        signature.set(componentManager->get_component_type<T>(), false);
        entityManager->set_signature(ent, signature);

        systemManager->entity_signature_changed(ent, signature);
    }

    template<typename T>
    T& get_component(Entity ent)
    {
        return componentManager->get_component<T>(ent);
    }

    template<typename T>
    ComponentID get_component_type()
    {
        return componentManager->get_component_type<T>();
    }

    /* System methods */
    template<typename T>
    std::shared_ptr<T> register_system()
    {
        return systemManager->register_system<T>();
    }

    template<typename T>
    void set_system_signature(Signature signature)
    {
        systemManager->set_signature<T>(signature);
    }

private:
    std::unique_ptr<ComponentManager> componentManager;
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<SystemManager> systemManager;
};

/* Component manager */

#include <unordered_map>
#include <memory>
#include "ecs/componentpool.hpp"
#include "ecs/types.hpp"

class ComponentManager
{
public:
    template<typename T>
    void register_component()
    {
        const char* typeName = typeid(T).name();

        assert(componentTypes.find(typeName) == componentTypes.end() && 
                "Registering component type more than once.");

        componentTypes.insert({typeName, nextComponentType});

        componentPools.insert({typeName, std::make_shared<ComponentPool<T>>()});

        nextComponentType++;
    }

    template<typename T>
    ComponentID get_component_type()
    {
        const char * typeName = typeid(T).name();

        assert(componentTypes.find(typeName) != componentTypes.end() && 
                "Component not registered before use.");

        return componentTypes[typeName];
    }

    template<typename T>
    void add_component(Entity ent, T component)
    {
        get_component_pool<T>()->insert_data(ent, component);
    }

    template<typename T>
    void remove_component(Entity ent)
    {
        get_component_pool<T>()->remove_data(ent);
    }

    template<typename T>
    T& get_component(Entity ent)
    {
        return get_component_pool<T>()->get_data(ent);
    }

    void entity_destroyed(Entity ent)
    {
        for (auto const& pair : componentPools) {
            auto const& component = pair.second;
            component->entity_destroyed(ent);
        }
    }


private:
    std::unordered_map<const char*, ComponentID> componentTypes{};

    std::unordered_map<const char*, std::shared_ptr<IComponentPool>> componentPools{};

    ComponentID nextComponentType{};

    template<typename T>
    std::shared_ptr<ComponentPool<T>> get_component_pool()
    {
        const char* typeName = typeid(T).name();

        assert(componentTypes.find(typeName) !=  componentTypes.end() &&
                "Component not registered before use.");

        return std::static_pointer_cast<ComponentPool<T>>(componentPools[typeName]);
    }
};

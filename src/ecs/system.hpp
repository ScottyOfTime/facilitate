/* System stuff */

#include <set>
#include <memory>
#include <unordered_map>
#include "ecs/types.hpp"

#pragma once

class System
{
public:
    std::set<Entity> entities;
};

class SystemManager
{
public:
    template<typename T>
    std::shared_ptr<T> register_system()
    {
        const char *typeName = typeid(T).name();

        auto system = std::make_shared<T>();
        systems.insert({typeName, system});
        return system;
    }

    template<typename T>
    void set_signature(Signature signature)
    {
        const char* typeName = typeid(T).name();
        signatures.insert({typeName, signature});
    }

    void entity_destroyed(Entity ent)
    {
        for (auto const& pair : systems) {
            auto const& system = pair.second;
            system->entities.erase(ent);
        }
    }

    void entity_signature_changed(Entity ent, Signature entSignature)
    {
        for (auto const& pair : systems) {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = signatures[type];

            if ((entSignature & systemSignature) == systemSignature) {
                system->entities.insert(ent);
            } else {
                system->entities.erase(ent);
            }
        }
    }

private:
    std::unordered_map<const char*, Signature> signatures{};

    std::unordered_map<const char*, std::shared_ptr<System>> systems{};
};

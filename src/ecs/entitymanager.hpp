/* Entity manager */

#include <queue>
#include <cassert>
#include "ecs/types.hpp"

#pragma once

class EntityManager
{
public:
    EntityManager()
    {
        for (Entity e = 0; e < MAX_ENTITIES; e++) {
            entityid_q.push(e);
        }
    }

    Entity create_entity()
    {
        assert(existingEntities < MAX_ENTITIES && "Hit maximum existing Entities.");

        Entity id = entityid_q.front();
        entityid_q.pop();
        existingEntities++;

        return id;
    }

    void destroy_entity(Entity ent)
    {
        assert(ent < MAX_ENTITIES && "Entity out of range.");

        signatures[ent].reset();
        entityid_q.push(ent);
        existingEntities--;
    }

    void set_signature(Entity ent, Signature sig)
    {
        assert(ent < MAX_ENTITIES && "Entity out of range.");

        signatures[ent] = sig;
    }

    Signature get_signature(Entity ent)
    {
        assert(ent < MAX_ENTITIES && "Entity out of range.");

        return signatures[ent];
    }

private:
    // Data structure (queue) to hold available Entity IDs
    std::queue<Entity> entityid_q;

    // Array of signatures where the index corresponds to Entity ID
    Signature signatures[MAX_ENTITIES];

    // Tracker of existing Entities
    uint32_t existingEntities;
};

/* Data structure implementation of the sparse set ecs
 * PLEASE ACTUALY UNDESTAND THIS!
 * I believe this data structure is called a pool in EnTT
 */

#include <array>
#include <cassert>
#include <ecs/types.hpp>

class IComponentPool
{
public:
    virtual ~IComponentPool() = default;
    virtual void entity_destroyed(Entity ent) = 0;
};

template<typename T>
class ComponentPool : public IComponentPool
{
public:
    void insert_data(Entity ent, T component)
    {
        /*assert(entityIndices[ent] == entityIndices[size] && 
                "Component added to same entity more than once.");*/

        size_t index = size;
        entityIndices[ent] = index;
        entityArray[index] =  ent;
        componentArray[index] = component;
        size++;
    }

    void remove_data(Entity ent)
    {
        /*assert(entityIndices[ent] != entityIndices[size] &&
            "Removing non-existent component.");*/

        size_t indexOfRemovedEntity = entityIndices[ent];
        size_t indexOfLastElement = size - 1;
        componentArray[indexOfRemovedEntity] = componentArray[indexOfLastElement];

        Entity entityOfLastElement = entityArray[indexOfLastElement];
        entityIndices[entityOfLastElement] = indexOfRemovedEntity;
        entityArray[indexOfRemovedEntity] = entityOfLastElement;

        entityIndices[ent] = 0;
        entityArray[indexOfLastElement] = 0;

        size--;
    }

    T& get_data(Entity ent)
    {
        /*assert(entityIndices[ent] != entityIndices[size] && 
                "Retrieving non-existent component");*/

        return componentArray[entityIndices[ent]];
    }

    void entity_destroyed(Entity ent) override
    {
        if (entityIndices[ent] !=  entityIndices[size])
        {
            remove_data(ent);
        }
    }

private:
    // Packed array
    std::array<T, MAX_ENTITIES> componentArray;
    
    // Packed array - accessed with entityArray[entityIndices[Entity ID]]
    std::array<Entity, MAX_ENTITIES> entityArray;

    // Sparse array - entitiyIndices[Entity ID] = index in entityArray
    std::array<Entity, MAX_ENTITIES> entityIndices;

    size_t size;
};

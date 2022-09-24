/* Types and aliases for the ECS */
#include <bitset>

#pragma once

/* Entity is just an id */
using Entity = uint16_t;

const Entity MAX_ENTITIES = 65535;

/* Self explanatory */
using ComponentID = uint8_t;

const ComponentID MAX_COMPONENTS = 32;

/* Bit field for a component signature
 * For example, if Component A has an ID of 1 and Component B
 * has an ID of 2 then an entity with both components A and B
 * would have a signature of 0b11, while an entity with only
 * component B would have a signature of 0b10
 */
using Signature = std::bitset<MAX_COMPONENTS>;

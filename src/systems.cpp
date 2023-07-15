#include "systems.hpp"
#include "ecs/coordinator.hpp"

extern Coordinator coordinator;

void start_systems() {

}

auto system_init_physics() {
	auto physicsSystem = coordinator.register_system<PhysicsSystem>();
	Signature sig;
	sig.set(coordinator.get_component_type<Transform>());
	sig.set(coordinator.set_component_type<Velocity>());
	coordinator.set_system_signature<PhysicsSystem>(signature);
	return physicsSystem;
}

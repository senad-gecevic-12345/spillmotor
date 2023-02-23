#ifndef SYSTEMS_H_
#define SYSTEMS_H_
#include <unordered_map>
#include "Components.h"
#include "entt/fwd.hpp"
#include "World.h"
#include "Utils.h"


namespace Bullet {
	//void create_bullet(entt::registry& registry, const Component::Position pos, const Component::Velocity vel, double fire_rate, entt::entity sender);
	//void create_bullet(entt::registry& registry, const Component::Position pos, const Component::Velocity vel);
	//void update_fire_rate(entt::registry& registry, float dt);
	//void update(entt::registry& registry, float dt);

	inline void step_and_copy_over(BulletScene& scene, float dt){
		scene.step(dt);
        scene.other_copy_over();
		//scene.entt_copy_over();
	}
}


namespace LifetimeSystem {
	inline void update(entt::registry& registry) {
		auto view = registry.view<Component::LifeTime>();
		auto time = World::Clock::get().get_time();
		for (auto e : view) {
			auto& val = registry.get<Component::LifeTime>(e);
			if ((val.spawn_time + val.seconds) < time) {
				registry.destroy(e);
			}
			// if something then registry.destroy(entity)	
		}
	}
}

namespace Model{
    void spheres_circle_animation_update(entt::registry& registry, float sec_rot, glm::vec3 center);
	void rotate_update_system(entt::registry& registry);
}

namespace VelocitySystem {
    void update_velocity(entt::registry& registry, float dt);
}

namespace EnemySystem {
	void spawn_enemies_2(entt::registry& registry, unsigned int no); 
	void spawn_enemies(entt::registry& registry, unsigned int no);

	void ai_event_approach(); 	
	void ai_bullet_update();
    void ai_velocity_update(entt::registry& registry);
}

namespace GUISystem{
    void editor_copy_over();

    // probably want that turned off
    void copy_to_representation(entt::registry& registry);
}

#endif // SYSTEMS_H_


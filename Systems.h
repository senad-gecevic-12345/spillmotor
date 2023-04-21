#ifndef SYSTEMS_H_
#define SYSTEMS_H_
#include <unordered_map>
#include "Components.h"
#include "entt/fwd.hpp"
#include "World.h"
#include "Utils.h"

namespace Bullet {
	inline void step_and_copy_over(BulletScene& scene, float dt){
		scene.step(dt);
        scene.other_copy_over();
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
		}
	}
}
void rotation_copy_over();
namespace GUISystem{
    void editor_copy_over();
    void new_editor_copy_over();
}

#endif // SYSTEMS_H_

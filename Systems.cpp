#include "Systems.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "Math.h"
#include "NewGui.h"
#include "Representation.h"
#include "entt/entt.hpp"

namespace GUISystem{
    void editor_copy_over(){
		auto& registry = EditorRegistry::get().m_registry;
		auto& gui = NewGui::get();
		auto& manager = RepresentationManager<EditorRepresentation>::get();
		for(auto y  : gui.editor_list){
			auto& x = manager.get_representation(y.key);
			auto e = x.get_entity_id();
			auto& position = registry.get<Component::Position>(e);
			position.pos = x.position;
			auto& rotation = registry.get<Component::Rotation>(e);
			rotation.rotation = x.rotation;
			auto& scale = registry.get<Component::Scale>(e);
			scale.scale = x.scale;
		}
	}
}

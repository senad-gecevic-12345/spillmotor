#include "Systems.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "Components.h"
#include "Math.h"
#include "NewGui.h"
#include "Representation.h"
#include "entt/entt.hpp"
#include "EditorGui.h"

namespace GUISystem{

    void new_editor_copy_over(){
        RepresentationModelManagerGui::get().copy_over_data();
    }

    void editor_copy_over(){}



}

void rotation_copy_over(){
    auto& registry = EditorRegistry::get().m_registry;
    auto group = registry.group<>(entt::get<Component::Rotation, Component::OpenGLRotation>);
    for(auto e : group){
        auto [rotation, openglrotation] = registry.get<Component::Rotation, Component::OpenGLRotation>(e);
        openglrotation.rotation = glm::mat4(1.f);
        openglrotation.rotation = glm::rotate(openglrotation.rotation, Math::deg_to_rad(rotation.y), glm::vec3(0, 1, 0));
        openglrotation.rotation = glm::rotate(openglrotation.rotation, Math::deg_to_rad(rotation.x), glm::vec3(1, 0, 0));
        openglrotation.rotation = glm::rotate(openglrotation.rotation, Math::deg_to_rad(rotation.z), glm::vec3(0, 0, 1));
    }

}
#include "Representation.h"

#include "Actor.h"
#include "LinearMath/btDefaultMotionState.h"
#include "Utils.h"
#include "bullet.h"
#include "EditorRegistry.h"

template<>
RepresentationKey RepresentationManager<BulletRepresentation>::create_primitive_representation(int type, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){}

template<>
RepresentationKey RepresentationManager<EditorRepresentation>::create_primitive_representation(int type, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){
    EditorRegistry& editor_registry = EditorRegistry::get();
    //auto& registry = Registry::get().registry;
    entt::entity e = editor_registry.m_registry.create();
    // bool valid = editor_registry.m_registry.valid(e);
    EditorRepresentation rep(type, e, position, rotation, scale);

    editor_registry.m_registry.emplace<Tags::RepresentationRenderNewMeshLoader>(e);
    editor_registry.m_registry.emplace<Component::Position>(e, position.x, position.y, position.z);
    editor_registry.m_registry.emplace<Component::Rotation>(e, rotation.x, rotation.y, rotation.z);
    editor_registry.m_registry.emplace<Component::Scale>(e, scale);

    MeshLoaderNew::MeshRenderId render;
    switch(rep.representation_type){
        case RepresentationType::SPHERE:
            render = MeshLoaderNew::load_mesh(xd::unit_sphere_obj);
            break;
        case RepresentationType::BOX:
            render = MeshLoaderNew::load_mesh(xd::unit_box_obj);
            break;
        default:
            render = MeshLoaderNew::load_mesh(xd::unit_sphere_obj);
            break;
    }

    editor_registry.m_registry.emplace<MeshLoaderNew::MeshRenderId>(e, render);

    return sparce.add(rep, RegistryOwner::EDITORREGISTRY);

};

template<>
RepresentationKey RepresentationManager<RepresentationCharacter>::create_character_representation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){
    auto& registry = Registry::get().registry;
    auto e = registry.create();
    RepresentationCharacter rep(e, position, rotation, scale);

    registry.emplace<Tags::RepresentationRenderNewMeshLoader>(e);
    registry.emplace<Component::Position>(e, position.x, position.y, position.z);
    registry.emplace<Component::Rotation>(e, rotation.x, rotation.y, rotation.z);
    registry.emplace<Component::Scale>(e, scale);

    std::string default_capsule_obj{"/home/solidus/Assets/Models/default_capsule.obj"};

    registry.emplace<MeshLoaderNew::MeshRenderId>
        (e, MeshLoaderNew::load_mesh(default_capsule_obj));

    return sparce.add(std::move(rep), RegistryOwner::EDITORREGISTRY);
}

template<>
RepresentationKey RepresentationManager<BulletRepresentation>::forward_to_bullet(int type, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){
    auto& bullet_registry = Registry::get().registry;
    auto e = bullet_registry.create();
    BulletRepresentation rep(e, type);
    auto rep_key = sparce.add(rep, RegistryOwner::BULLETREGISTRY);
    BulletScene::get().manage_representation(rep_key, position, rotation, scale);

    return rep_key;
}


template<>
RepresentationKey RepresentationManager<EditorRepresentation>::move_to_bullet(RepresentationKey& key){

    Registry& registry = Registry::get();
    EditorRegistry& eregistry = EditorRegistry::get();

    auto& rep = get_representation(key);
    auto e = rep.get_entity_id();

    // this crashes 
    auto new_key = 
        RepresentationManager<BulletRepresentation>::get().forward_to_bullet(rep.representation_type, rep.position, rep.rotation, rep.scale);
    auto new_e = 
        RepresentationManager<BulletRepresentation>::get().get_representation(new_key).get_entity_id();


    Utils::copy_over_to_other_registry(registry.registry, eregistry.m_registry, new_e, e);

    sparce.remove(key);

    eregistry.m_registry.destroy(e);

    return key;
}


template<>
void RepresentationManager<RepresentationCharacter>::delete_representation(const RepresentationKey& key){
    auto& rep = sparce.get(key);
    auto& registry = Registry::get().registry;
    registry.destroy(rep.character.e);
    sparce.remove(key);
}


#include "Representation.h"
#include <iostream>
#include "Actor.h"
#include "MeshFunctions2.h"
#include "Utils.h"
#include "bullet.h"
#include "EditorRegistry.h"
#include "GuiGlobalVariablesSingleton.h"

MeshLoaderNew::MeshMetaData create_meta_data(LoadMeshData data) {
    auto& flags = data.flags;
    auto& mesh = *data.mesh;
    auto* anim = AnimationMeshStorage::get().get_mesh(data.animation_location());
    if(anim)
        return MeshLoaderNew::MeshMetaData(
                flags,
                mesh.meshes.size(),
                mesh.materials.size(),
                mesh.positions.size(),
                mesh.normals.size(),
                mesh.tangents.size(),
                mesh.tex_coords.size(),
                mesh.indices.size(),
                mesh.bones.size(),
                anim->transforms.size(),
                data.no_animations
        );
    return MeshLoaderNew::MeshMetaData(
            flags,
            mesh.meshes.size(),
            mesh.materials.size(),
            mesh.positions.size(),
            mesh.normals.size(),
            mesh.tangents.size(),
            mesh.tex_coords.size(),
            mesh.indices.size(),
            mesh.bones.size(),
            0,
            0
    );
}

MeshLoaderNew::MeshMetaData EditorRepresentation::set_render(const std::string& file_loc_name){
    auto& registry = EditorRegistry::get().m_registry;
    MeshLoaderNew::MeshRenderId render;
    auto load_mesh_data = MeshLoaderNew::load_mesh_with_meta_data(file_loc_name);
    const auto& flags = load_mesh_data.flags;
    auto render_id = create_mesh_render_id(*load_mesh_data.mesh);
    registry.remove<Tags::NormalMap>(e);
    registry.remove<RenderTags::BonesRendering>(e);
    registry.remove<Component::AnimationController>(e);
    registry.remove<MeshLoaderNew::MeshRenderId>(e);

    if(flags[MeshLoaderNew::MESHFLAG::NORMALMAP]){
        registry.emplace<Tags::NormalMap>(e);
    }

    if(flags[MeshLoaderNew::MESHFLAG::BONES]){
        registry.emplace<RenderTags::BonesRendering>(e);
        registry.emplace<Component::AnimationController>(e, load_mesh_data.animation_location(), -1, load_mesh_data.no_animations);
    }

    registry.emplace<MeshLoaderNew::MeshRenderId>(e, render_id);
    return create_meta_data(load_mesh_data);
}

RepresentationKey EditorRepresentationManager::create_representation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){
    entt::registry& registry = EditorRegistry::get().m_registry;
    entt::entity e = registry.create();

    EditorRepresentation rep(e);

    registry.emplace<Tags::RepresentationRenderNewMeshLoader>(e);

    registry.emplace<Component::Position>(e, position.x, position.y, position.z);
    registry.emplace<Component::Rotation>(e, rotation.x, rotation.y, rotation.z);
    registry.emplace<Component::Scale>(e, scale);
    registry.emplace<Component::OpenGLRotation>(e);

    return sparce.add(rep, RegistryOwner::EDITORREGISTRY);
}

RepresentationKey EditorRepresentationManager::move_to_bullet_with_data(RepresentationKey& key, btCollisionShape* collision_shape, BulletInit::BulletConfigData config){
    Registry& registry =        Registry::get();
    EditorRegistry& eregistry = EditorRegistry::get();

    auto& rep = get_representation(key);
    auto e = rep.get_entity_id();

    auto p = eregistry.m_registry.get<Component::Position>(e);
    auto r = eregistry.m_registry.get<Component::Rotation>(e);
    auto s = eregistry.m_registry.get<Component::Scale>(e);

    auto new_key =
        BulletRepresentationManager::get().forward_to_bullet(p.pos, r.rotation, s.scale, config, collision_shape);
    auto new_e =
        BulletRepresentationManager::get().get_representation(new_key).get_entity_id();

    eregistry.m_registry.remove<Component::Rotation>(e);
    Utils::copy_over_to_other_registry(registry.registry, eregistry.m_registry, new_e, e);
    registry.registry.emplace<Tags::BulletPhysics>(new_e);

    sparce.remove(key);
    eregistry.m_registry.destroy(e);

    return new_key;
}


// BulletRepresentationManager

RepresentationKey BulletRepresentationManager::forward_to_bullet(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, BulletInit::BulletConfigData config, btCollisionShape* collision_shape){
    auto& bullet_registry = Registry::get().registry;
    auto e = bullet_registry.create();
    BulletRepresentation rep(e);
    auto rep_key = sparce.add(rep, RegistryOwner::BULLETREGISTRY);
    BulletScene::get().manage_representation(rep_key, position, rotation, scale, config,  collision_shape);

    return rep_key;
}

entt::entity BulletRepresentationManager::create_bullet_object(const std::string& file_loc_name , glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){

    auto& registry = Registry::get().registry;
    auto e = registry.create();
    auto rep = BulletRepresentation(e);


    registry.emplace<Tags::RepresentationRenderNewMeshLoader>(e);
    registry.emplace<Component::Position>(e, position.x, position.y, position.z);
    registry.emplace<Component::OpenGLRotation>(e);
    registry.emplace<Component::Scale>(e, scale);
    registry.emplace<Tags::NormalMap>(e);

    auto[mesh, flags] = MeshLoaderNew::load_mesh(file_loc_name);

    registry.emplace<MeshLoaderNew::MeshRenderId>(e, mesh);

    unmanaged_keys.keys.push_back(sparce.add(rep, RegistryOwner::BULLETREGISTRY));

    return e;
}



// CharacterRepresentationManager

RepresentationKey CharacterRepresentationManager::create_character_representation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){
    auto& registry = Registry::get().registry;
    auto e = registry.create();
    CharacterRepresentation rep(e, position, rotation/*, scale*/);

    registry.emplace<Tags::RepresentationRenderNewMeshLoader>(e);
    registry.emplace<Component::Position>(e, position.x, position.y, position.z);
    registry.emplace<Component::Rotation>(e, rotation.x, rotation.y, rotation.z);
    registry.emplace<Component::Scale>(e, scale);

    std::string default_capsule_obj{"/home/solidus/Assets/Models/default_capsule.obj"};

    auto [mesh, flags] = MeshLoaderNew::load_mesh(default_capsule_obj);
    registry.emplace<MeshLoaderNew::MeshRenderId>
        (e, mesh);

    return sparce.add(std::move(rep), RegistryOwner::EDITORREGISTRY);
}

RepresentationKey CharacterRepresentationManager::create_character_representation(int type, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){
    auto& registry = Registry::get().registry;
    auto e = registry.create();
    Controller::Character* character = nullptr;
    switch(type){
        case CharacterTypes::STANDARD:
              character = new Controller::Character(e, position, rotation);
            break;
        case CharacterTypes::SHOOTER:
            character = new Controller::ShootingCharacter(e, position, rotation);
            break;
    }
    assert(character != nullptr);
    CharacterRepresentation rep(e, character);

    registry.emplace<Tags::RepresentationRenderNewMeshLoader>(e);
    registry.emplace<Component::Position>(e, position.x, position.y, position.z);
    registry.emplace<Component::Rotation>(e, rotation.x, rotation.y, rotation.z);
    registry.emplace<Component::Scale>(e, scale);

    std::string default_capsule_obj{"/home/solidus/Assets/Models/default_capsule.obj"};

    auto [mesh, flags] = MeshLoaderNew::load_mesh(default_capsule_obj);
    registry.emplace<MeshLoaderNew::MeshRenderId>
        (e, mesh);

   return sparce.add(std::move(rep), RegistryOwner::EDITORREGISTRY);
}

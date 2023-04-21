#ifndef REPRESENTATION_H_
#define REPRESENTATION_H_

#include "MeshFunctions2.h"
#include "TextureFunctions.h"
#include "entt/entt.hpp"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "RenderFunctions.h"
#include "Tags.h"
#include "Utils.h"
#include <memory.h>
#include "Actor.h"
#include "EditorRegistry.h"
#include <signal.h>
#include <functional> 
#include <cstdio>

class EditorRepresentation;
class BulletRepresentation;
class CharacterRepresentation;
class ComponentSelectionBase;

namespace MeshLoaderNew{
    struct MeshRenderId;
}

namespace BulletInit{
    struct BulletConfigData;
};

enum CharacterTypes{
    STANDARD = 0,
    SHOOTER = 1
};

namespace Controller{
    struct Character;
}



enum RepresentationType{
    SPHERE_WITH_NORMAL,
    SPHERE,
    BOX,
    CAPSULE,
    BASEBALL
};

enum RegistryOwner{
    EDITORREGISTRY,
    BULLETREGISTRY,
};

class RepresentationKey{
public:
    int key;

    RepresentationKey(int key):key(key){}
    
    bool operator==(const RepresentationKey& other){return key == other.key;}

    RepresentationKey(const RepresentationKey& other):
        key(other.key){}

    RepresentationKey& operator=(const RepresentationKey& other)
    {
        key = other.key;
        return *this;
    }
};

// can probably just return int
template<> 
struct std::hash<RepresentationKey>{
    std::size_t operator()(RepresentationKey const& key)const noexcept{
        return std::hash<int>{}(key.key);
    }
};

template<typename RepresentationType>
class Sparce{
    std::vector<RepresentationType> representations;
    std::vector<int> sparce;
    public:
    RepresentationKey add(const RepresentationType& type, int registry_owner){
        int rs = representations.size();
        int ss = [&, this](){
            for(int i = 0; i < this->sparce.size(); ++i){
                if(this->sparce[i] == -1){
                    this->sparce[i] = rs;
                    return i;
                }
            }
            this->sparce.push_back(rs);
            return (int)this->sparce.size()-1;
        }();
        representations.push_back(type);

        assert(sparce[sparce.size()-1] < (int)representations.size());

        auto key = RepresentationKey(ss);
        assert(key.key < (int)sparce.size());

        bool ss_ok = 0;
        for(int i = 0; i < sparce.size(); ++i){
            if(sparce[i] == rs){
                ss_ok = 1;
                assert(i == key.key);
            }
        }
        assert(ss_ok);

        return key;
    }
    RepresentationKey add(RepresentationType&& type, int registry_owner){
        int rs = representations.size();
        int ss = [&, this](){
            for(int i = 0; i < this->sparce.size(); ++i){
                if(this->sparce[i] == -1){
                    this->sparce[i] = rs;
                    return i;
                }
            }
            this->sparce.push_back(rs);
            return (int)this->sparce.size()-1;
        }();
        representations.push_back(std::forward<RepresentationType>(type));

        assert(sparce[sparce.size()-1] < (int)representations.size());

        auto key = RepresentationKey(ss);

        assert(key.key < (int)sparce.size());
        bool ss_ok = 0;
        for(int i = 0; i < sparce.size(); ++i){
            if(sparce[i] == rs){
                ss_ok = 1;
                assert(i == key.key);
            }
        }
        assert(ss_ok);

        return key;
    }
    void remove(RepresentationKey key){
        int swap_elem = representations.size()-1;
        int rem_elem = sparce[key.key];
        assert(rem_elem < (int)representations.size());
        assert(swap_elem < (int)representations.size());

        if(swap_elem != rem_elem)
            std::swap(representations[swap_elem], representations[rem_elem]);

        for(auto& x : sparce){
            if(x == swap_elem){
                x = (x == rem_elem ? -1 : rem_elem);
            }
        }

        for(int i = 0; i < representations.size()-1; ++i){
            bool has=0;
            for(int j = 0; j < sparce.size(); ++j){
                if(i == sparce[j]){
                    has = 1;
                    break;
                }
            }
            assert(has);
        }

        assert((int)representations.size() > 0);
        representations.pop_back();
    }
    RepresentationType& get(RepresentationKey key){
        assert(key.key >= 0);
        assert(key.key < (int)sparce.size());
        assert(sparce[key.key] < (int)representations.size());
        assert(sparce[key.key] >= 0);
        return representations[sparce[key.key]];
    }
};


class RepresentationBuffer{
public:
    std::vector<RepresentationKey> keys;

};

class EditorRepresentation{
public:
    entt::entity e;

    const entt::entity get_entity_id()const{
        return e;
    }

    MeshLoaderNew::MeshMetaData set_render(const std::string& file_loc_name);

    void remove_render(){
        auto& registry = EditorRegistry::get().m_registry;
        registry.remove<MeshLoaderNew::MeshRenderId>(e);
    }

    EditorRepresentation(entt::entity e):e(e){}

    ~EditorRepresentation()=default;
    EditorRepresentation& operator=(const EditorRepresentation& rep){
        e = rep.e;
        return *this;
    }
    EditorRepresentation(const EditorRepresentation& other):
        e(other.e){}

    friend class BulletScene;
};


class CharacterRepresentation{
public:
    std::unique_ptr<Controller::Character> character;
public:
    CharacterRepresentation(entt::entity e, Controller::Character* character):
        character(character){}

    CharacterRepresentation(entt::entity e, glm::vec3 position, glm::vec3 rotation):
        character(new Controller::Character(e, position, rotation)){}

    CharacterRepresentation& operator=(CharacterRepresentation&& other)noexcept
    {
        character = std::move(other.character);
        return *this;
    }

    CharacterRepresentation& operator=(const CharacterRepresentation& other) = delete;
    CharacterRepresentation(const CharacterRepresentation& other) = delete;
    CharacterRepresentation(CharacterRepresentation&& other)noexcept
    :
        character {std::move(other.character)}{}
};



template<typename DerivedManager, typename Type>
class RepresentationManagerNew{
public:
    Sparce<Type> sparce;
    RepresentationBuffer unmanaged_keys;

    Type& get_representation(const RepresentationKey& key){
        return sparce.get(key);
    }

    void delete_representation(const RepresentationKey& key);

    void change_data_owner(const RepresentationKey& key, Type& type){
        type = std::move(sparce.get(key));
        sparce.remove(key);
    }

    static DerivedManager& get(){
        static DerivedManager singleton;
        return singleton;
    }

};


class EditorRepresentationManager : public RepresentationManagerNew<EditorRepresentationManager, EditorRepresentation>{
    public:

    RepresentationKey create_representation(const std::string& file_loc_name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    RepresentationKey create_representation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    RepresentationKey move_to_bullet_with_data(RepresentationKey& key, btCollisionShape* collision_shape, BulletInit::BulletConfigData config);

    void delete_representation(const RepresentationKey& key){
        auto& rep = sparce.get(key);
        auto& registry = EditorRegistry::get().m_registry;
        registry.destroy(rep.e);
        sparce.remove(key);
    }

    void move_to_bullet_with_data(const RepresentationKey& key, const BulletInit::BulletConfigData& config, btCollisionShape* shape);
};




class BulletRepresentationManager : public RepresentationManagerNew<BulletRepresentationManager, BulletRepresentation>{
public:
    
    entt::entity create_bullet_object(const std::string& file_loc_name , glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    RepresentationKey forward_to_bullet(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, BulletInit::BulletConfigData config, btCollisionShape* collision_shape);
    RepresentationKey create_primitive_representation(MeshLoaderNew::MeshRenderId render_id, btCollisionShape* collision_shape, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    RepresentationKey create_static_primitive_representation(MeshLoaderNew::MeshRenderId render_id, btCollisionShape* collision_shape, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);



    void move_editor_representation_to_bullet(){};
    void move_to_gui();
};

class CharacterRepresentationManager : public RepresentationManagerNew<CharacterRepresentationManager, CharacterRepresentation>{
public:
    RepresentationKey create_character_representation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    RepresentationKey create_character_representation(int type, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    void delete_representation(const RepresentationKey& key){
        auto& rep = sparce.get(key);
        auto& registry = Registry::get().registry;
        registry.destroy(rep.character->e);
        sparce.remove(key);
    }
    CharacterRepresentationManager(){}
};




class EntityMode;

inline void move_character_to_singleton(std::unique_ptr<Controller::Character>& ptr){
     if(Controller::ControllerSingleton::get().character != nullptr){ 
         Controller::ControllerSingleton::get().clear();
     }

     Controller::ControllerSingleton::get().character = std::move(ptr);
}

class BulletRepresentation{
    entt::entity e;
public:
    const entt::entity get_entity_id()const{
        return e;
    }
    BulletRepresentation(entt::entity e):e(e){}
    ~BulletRepresentation()=default;
    BulletRepresentation& operator=(const BulletRepresentation& rep){ 
        e = rep.e; 
        return *this;
    }

    friend class BulletScene;
    friend class BulletRepresentationManager;
};




#endif // REPRESENTATION_H_

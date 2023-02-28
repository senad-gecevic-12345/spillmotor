#ifndef REPRESENTATION_H_
#define REPRESENTATION_H_

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


enum CharacterTypes{
    STANDARD = 0,
    SHOOTER = 1
};

inline int get_id(){
    static int id = 0;
    return ++id;
}

namespace Controller{
    struct Character;
}

namespace xd{
    inline std::string unit_shapes_folder{ "/home/solidus/Assets/UnitShapes/" };
    inline std::string unit_sphere_obj{unit_shapes_folder + "sphere.obj"};
    inline std::string sphere_with_normal{"/home/solidus/Assets/Normal/sphere_with_normal_map.obj"};
    inline std::string unit_sphere_mtl{unit_shapes_folder + "sphere.mtl"};
    inline std::string unit_cone_obj{ unit_shapes_folder + "cone.obj"};
    inline std::string unit_cone_mtl{ unit_shapes_folder + "cone.mtl"};
    inline std::string unit_box_obj{ unit_shapes_folder + "box.obj"};
    inline std::string unit_box_mtl{ unit_shapes_folder + "box.mtl"};
    inline std::string unit_cylinder_obj{ unit_shapes_folder + "cylinder.obj"};
    inline std::string unit_cylinder_mtl{ unit_shapes_folder + "cylinder.mtl"};
    inline std::string capsule_folder{"/home/solidus/Assets/Models/"};
    inline std::string default_capsule_obj{capsule_folder + "default_capsule.obj"};
    inline std::string default_capsule_mtl{capsule_folder + "default_capsule.mtl"};
};

class ComponentSelectionBase;
class ComponentSelectionPosition;
class ComponentSelectionRotation;
class ComponentSelectionScale;

class Representation;

enum RepresentationType{
    SPHERE_WITH_NORMAL,
    SPHERE,
    BOX,
    CAPSULE
};

enum RegistryOwner{
    EDITORREGISTRY,
    BULLETREGISTRY,
};

// check what key is initialized to
class RepresentationKey{
public:
    int key;
    int registry_owner;
    RepresentationKey(int key, int owner):key(key),registry_owner(owner){
    }
    
    bool operator==(const RepresentationKey& other)
    {
        return key == other.key;
    }

    RepresentationKey(const RepresentationKey& other)
    {
        key = other.key;
        registry_owner = other.registry_owner;
    }
   RepresentationKey& operator=(const RepresentationKey& other)
    {
        key = other.key;
        registry_owner = other.registry_owner;
        return *this;
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

        auto key = RepresentationKey(ss, registry_owner);
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

        auto key = RepresentationKey(ss, registry_owner);

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



// create something in some code, and later something else queries this to manage it
// or delegates correctly if, for examle, it has a lifetime, then no need to manage probably
class RepresentationBuffer{
public:
    std::vector<RepresentationKey> keys;

};


template<typename RepresentationType>
class RepresentationManager{
public:
    Sparce<RepresentationType> sparce;
    RepresentationBuffer unmanaged_keys;
    static RepresentationManager& get(){
        static RepresentationManager<RepresentationType> instance;
        return instance;
    }

    RepresentationKey create_representation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    RepresentationKey create_primitive_representation(int type, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    RepresentationKey create_character_representation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    RepresentationKey create_character_representation(int type, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    RepresentationKey create_character_representation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Controller::Character* character);
    RepresentationKey create_editor_representation(int type, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    RepresentationKey forward_to_bullet(int type, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    RepresentationKey forward_to_gui(int type, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    RepresentationKey create_component_representation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    RepresentationType& get_representation(const RepresentationKey& key){
        return sparce.get(key);
        //return representations[sparce[key.key]];
    };

    void delete_representation(const RepresentationKey& key);

    // THIS NEEDS TO CHANGE
    RepresentationKey move_to_bullet(RepresentationKey& key);
    entt::entity create_bullet_object(int type, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    // already by key probably
    bool move_to_gui(int registry_owner, RepresentationKey& key);

};




class EntityMode;


// see move_character_to_singleton
//
// initialize this with a pointer. this should own the pointer
// // why does this store position and such
class RepresentationCharacter{
public:
    std::unique_ptr<Controller::Character> character;
    friend class RepresentationManager<RepresentationCharacter>;
public:
    RepresentationCharacter(entt::entity e, Controller::Character* character):
        character(character)
    {
    }

    RepresentationCharacter(entt::entity e, glm::vec3 position, glm::vec3 rotation):
        character(new Controller::Character(e, position, rotation))
    {
    }

    RepresentationCharacter& operator=(RepresentationCharacter&& other)noexcept
    {
        character = std::move(other.character);
        return *this;
    }

    RepresentationCharacter& operator=(const RepresentationCharacter& other) = delete;
    RepresentationCharacter(const RepresentationCharacter& other) = delete;
    RepresentationCharacter(RepresentationCharacter&& other)noexcept
    :
        //position(std::move(other.position)),
        //rotation(std::move(other.rotation)),
        //scale(std::move(other.scale)),
        character {std::move(other.character)}
    {
    }

};

inline void move_character_to_singleton(std::unique_ptr<Controller::Character>& ptr){
     if(Controller::ControllerSingleton::get().character != nullptr){ 
         Controller::ControllerSingleton::get().clear();
     }

     Controller::ControllerSingleton::get().character = std::move(ptr);
}


class Representation{
public:
    int representation_type;
    entt::entity e;
    glm::vec3 position, rotation, scale;
    const entt::entity get_entity_id()const{
        return e;
    }

private:
    Representation(int type, entt::entity e, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale):
         representation_type(type), e(e), position(position), rotation(rotation), scale(scale)
    {
    }
    friend class BulletScene;
    friend class RepresentationManager<Representation>;
public:
    Representation& operator=(const Representation& rep){
        representation_type=rep.representation_type;
        e = rep.e;
        position = rep.position;
        rotation=rep.rotation;
        scale=rep.scale;
        return *this;
    }
};


class EditorRepresentation{
public:
    entt::entity e;
    glm::vec3 position, rotation, scale;
    int representation_type;
    const entt::entity get_entity_id()const{
        return e;
    }

public:
    EditorRepresentation(int type, entt::entity e, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale):
         representation_type(type), position(position), rotation(rotation), scale(scale), e(e)
    {
    }
    friend class BulletScene;
public:
    EditorRepresentation& operator=(const EditorRepresentation& rep){
        e = rep.e;
        position = rep.position;
        rotation=rep.rotation;
        scale=rep.scale;
        representation_type=rep.representation_type;
        return *this;
    }
    EditorRepresentation(const EditorRepresentation& rep){
        e = rep.e;
        position = rep.position;
        rotation=rep.rotation;
        scale=rep.scale;
        representation_type=rep.representation_type;
    }
};

class BulletRepresentation{
    entt::entity e;
    int representation_type;
public:
    const entt::entity get_entity_id()const{
        return e;
    }
public:
    BulletRepresentation(entt::entity e, int type):e(e), representation_type(type){}
    friend class BulletScene;
    friend class RepresentationManager<BulletRepresentation>;
public:
    BulletRepresentation& operator=(const BulletRepresentation& rep){ 
        e = rep.e; representation_type = rep.representation_type; 
        return *this;
    }
};




#endif // REPRESENTATION_H_


#ifndef EDITORGUI_H_
#define EDITORGUI_H_

#include "CollisionWrappers.h"
#include <unordered_map>
#include <string>
#include "MeshFunctions2.h"
#include "glm/gtx/transform.hpp"

inline void static_data_reset(BulletInit::BulletConfigData& data){
    data.mass = 0;
    data.rolling_friction = 0;
    data.spinning_friction = 0;
}

// DOCU: for updating keys
// inherited by the components which have a unique selection per representation
// not inherited by the component for selecting representations
// components has functions for step 2 and 3. step 1 and 4 is implemented in the class which owns the components
// step 1: return pointer to key for the selected representation
// step 2: set correct selection at components for the representation
// step 3: update the components and return the current selection
// step 4: update the key pointer for the representation component
// let this be for for example collision shape and another for models and then have the button in something that includes both maybe

template<typename Derived>
class GuiBaseSingleton{
public:
    void update(){
        static_cast<Derived*>(this)->update();
    }

    static Derived& get(){
        static Derived instance;
        return instance;
    }

    GuiBaseSingleton& operator=(const GuiBaseSingleton&)=delete;
    GuiBaseSingleton(const GuiBaseSingleton&)=delete;

    GuiBaseSingleton()=default;
    ~GuiBaseSingleton()=default;
};

struct RepresentationPointer{
    int model;
    int collision;
    void invalidate(){
        model = -1;
        collision = -1;
    }
    bool valid(){
        return model != -1 && collision != -1;
    }
    RepresentationPointer& operator=(const RepresentationPointer other){
        model = other.model;
        collision = other.collision;
        return *this;
    }
    RepresentationPointer(const RepresentationPointer& other)=default;
    RepresentationPointer():model(-1), collision(-1){}
    RepresentationPointer(int model, int collision):model(model), collision(collision){}
    ~RepresentationPointer()=default;
};

enum COLLISION_GUI_FLAGS{
    IS_DYNAMIC = 0,
    OTHER = 1,
};

struct CollisionManagerGuiData{
    std::string path;
    std::bitset<8> gui_flags{0};
    BulletInit::BulletConfigData config;


};

class CollisionManagerGuiComponent{
    SelectionList<std::string> collision_shapes;
    std::unordered_map<std::string, CollisionWrappers::CollisionShape*> str_to_param;
    CollisionManagerGuiData data_buffer;

public:
    void set_data(const CollisionManagerGuiData& data){
        data_buffer = data;
    }
    CollisionManagerGuiData& get_data(){
        return data_buffer;
    }


private:


    void select_collision_shape(){
        if(ImGui::BeginListBox("Collision Shapes")){
            for(int i = 0; i < collision_shapes.size(); ++i){
                bool is_selected = i == collision_shapes.selected_id;
                if(ImGui::Selectable(collision_shapes[i].c_str(), is_selected)){
                    collision_shapes.toggle_selected(i);
                }
            }
            ImGui::EndListBox();
        }
    }

    void selection_configuration_parameters(){
        if(collision_shapes.has_selection()){
            auto& selection = collision_shapes.get_selection();
            str_to_param[selection]->config();
        }
    }

    void physics_parameters(){
        auto checkbox = [&, this](const std::string& str, int flag){
            bool b = this->data_buffer.gui_flags[flag];
            ImGui::Checkbox(str.c_str(), &b);
            this->data_buffer.gui_flags[flag] = b;
            return b;
        };

        bool is_dynamic =   checkbox("is_dynamic", COLLISION_GUI_FLAGS::IS_DYNAMIC);
        bool other =        checkbox("other", COLLISION_GUI_FLAGS::OTHER);



        if(ImGui::CollapsingHeader("Physics Configuration")){
            if(is_dynamic){
                ImGui::InputFloat("mass",               &data_buffer.config.mass);
                ImGui::InputFloat("friction",           &data_buffer.config.friction);
                ImGui::InputFloat("restitution",        &data_buffer.config.restitution);
                ImGui::InputFloat("rolling_friction",   &data_buffer.config.rolling_friction);
                ImGui::InputFloat("spinning_friction",  &data_buffer.config.spinning_friction);
            }
            else
            {
                ImGui::InputFloat("friction",           &data_buffer.config.friction);
                ImGui::InputFloat("restitution",        &data_buffer.config.restitution);
            }
        }
    }


    void reset_collision_parameters(){
        for(int i = 0; i < collision_shapes.size(); ++i){
            if(auto* ptr = str_to_param[collision_shapes[i]];  ptr != nullptr){
                ptr->reset();
            }
        }
    }


    bool add_physics(){
        if(collision_shapes.has_selection()){
            auto selection = collision_shapes.get_selection();
            auto param = str_to_param[selection];
            return param->create();
        }
        return 0;
    }

public:

    btCollisionShape* create(const std::string& path){
        if(!has_selection()){
            return nullptr;
        }
        auto* tri_mesh_cast = dynamic_cast<CollisionWrappers::TriangleMeshCollisionShape*>(str_to_param[collision_shapes.get_selection()]);
        if(tri_mesh_cast != nullptr){
            tri_mesh_cast->set_path(path);
        }
        return str_to_param[collision_shapes.get_selection()]->create();
    }

    bool has_selection(){return collision_shapes.has_selection();}
    CollisionWrappers::CollisionShape* get_configuration(){return str_to_param[collision_shapes.get_selection()];}
    void set_current_selection(int id){collision_shapes.selected_id = id;}
    int get_current_selection()const{return collision_shapes.selected_id;}
    void deselect(){collision_shapes.deselect();}

    void update(){
        select_collision_shape();
        selection_configuration_parameters();
        physics_parameters();
    }

    CollisionManagerGuiComponent(){
        auto init_add = [this](std::string str, CollisionWrappers::CollisionShape* config){
            collision_shapes.add_representation(str);
            str_to_param[str] = config;
        };

        init_add("sphere",  new CollisionWrappers::SphereCollisionShape);
        init_add("box",     new CollisionWrappers::BoxCollisionShape);
        init_add("capsule", new CollisionWrappers::CapsuleCollisionShape);
        init_add("bvh tri mesh",  new CollisionWrappers::TriangleMeshCollisionShape);

        deselect();
    }


};



class RepresentationManagerGuiComponent{

    struct AdditionalData{
        glm::vec3 position{0, 0, 0};
        glm::vec3 rotation{0, 0, 0};
        glm::vec3 scale{1, 1, 1};
        RepresentationPointer pointer{-1, -1};
        CollisionManagerGuiData collision_data;
    };


    RepresentationSelectionListWithAdditionalData<AdditionalData> representation_list;
    EditorRepresentationManager editor_manager;


    void manage(){
        if(ImGui::Button("New")){
            representation_list.add_representation(editor_manager.create_representation(glm::vec3{0, 0, 0}, glm::vec3{0, 0, 0}, glm::vec3{1, 1, 1}), AdditionalData());
        }
        ImGui::SameLine();
        if(ImGui::Button("Delete")){
            if(representation_list.has_selection()){
                auto& selection = representation_list.get_selection();
                editor_manager.delete_representation(selection);
                representation_list.throw_away(representation_list.selected_id);
            }
        }

    }

    // need a copy over
    void config(){
        if(representation_list.has_selection()){
            auto& data = representation_list.get_selection_data();
            ImGui::DragFloat3("position", (float*)&data.position, 0.5f);
            ImGui::DragFloat3("rotation", (float*)&data.rotation, 0.5f);
            ImGui::DragFloat3("scale"   , (float*)&data.scale,    0.5f);
        }
    }

    void list(){
        if(ImGui::BeginListBox("Representation List")){
            for(int i = 0; i < representation_list.size(); ++i){
                auto& representation =
                    editor_manager.get_representation(representation_list[i]);
                const auto e = representation.get_entity_id();
                bool is_selected = i == representation_list.selected_id;
                if(ImGui::Selectable(std::to_string((int)e).c_str(), is_selected)){
                    representation_list.toggle_selected(i);
                }
            }
            ImGui::EndListBox();
        }
    }

public:

    void copy_over(){
        auto& registry = EditorRegistry::get().m_registry;
        auto& list = representation_list;
        for(int i = 0; i < list.representations.size(); ++i){
            auto e = editor_manager.get_representation(list.representations[i]).e;
            const auto& data = list.additional_data[i];
            auto& position = registry.get<Component::Position>(e);
            auto& rotation = registry.get<Component::Rotation>(e);           
            auto& scale = registry.get<Component::Scale>(e);
            position.pos = data.position;
            rotation.rotation = data.rotation;
            scale.scale = data.scale;
        }
    }

    AdditionalData* get_additional_data(){
        if(!representation_list.has_selection()){
            return nullptr;
        }
        return &representation_list.get_selection_data();
    }


public:
    void move_selected_to_bullet(btCollisionShape* collision_shape){

        glm::mat4 rotate_matrix(1.f);
        rotate_matrix = glm::rotate(rotate_matrix, Math::deg_to_rad(20), glm::vec3(0, 1, 0));
        rotate_matrix = glm::rotate(rotate_matrix, Math::deg_to_rad(20), glm::vec3(1, 0, 0));
        rotate_matrix = glm::rotate(rotate_matrix, Math::deg_to_rad(20), glm::vec3(0, 0, 1));

        auto* data2 = get_additional_data();
        if(auto* data = &representation_list.get_selection_data(); data != nullptr){
            if(!data->collision_data.gui_flags[COLLISION_GUI_FLAGS::IS_DYNAMIC])
                static_data_reset(data->collision_data.config);
            editor_manager.move_to_bullet_with_data(representation_list.get_selection(), collision_shape, data->collision_data.config);
            representation_list.throw_away(representation_list.selected_id);
        }
    }

    std::tuple<bool, entt::entity> get_entity(){
        if(representation_list.has_selection()){
            return std::make_tuple(1, editor_manager.get_representation(representation_list.get_selection()).e);
        }

        return std::make_tuple(0, entt::entity());
    }

    MeshLoaderNew::MeshMetaData set_render_for_selected(const std::string& file_loc_name){
        if(representation_list.has_selection()){
            return editor_manager.get_representation(representation_list.get_selection()).set_render(file_loc_name);
        }
    }

    void delete_render_for_selected(){
        if(representation_list.has_selection()){
            editor_manager.get_representation(representation_list.get_selection()).remove_render();
        }
    }

    void deselect(){
        representation_list.deselect();
    }

    bool has_selection(){return representation_list.has_selection();}
    RepresentationKey get_selection(){return representation_list.get_selection();}
    RepresentationPointer* get_key_for_selected(){
        if(!representation_list.has_selection()){
            return nullptr;
        }
        return &representation_list.get_selection_data().pointer;
    }


    void update(){
        manage();
        list();
        config();
    }

    RepresentationManagerGuiComponent():
        editor_manager(EditorRepresentationManager::get())
    {}

};

class ModelManagerGuiComponent{
    ModelSaveFile save_file;
    ReferenceSelectionList<std::string> selection_list;
    MeshLoaderNew::MeshFlags m_flags;
    MeshLoaderNew::MeshMetaData meta_data;
public:
    int play_animation{-1};
    int no_animations{0};
private:

    void meta_data_display(){
        if(ImGui::CollapsingHeader("Meta Data")){
            ImGui::Text("%s", std::string("no_meshes: " + std::to_string(meta_data.no_meshes)).c_str());
            ImGui::Text("%s", std::string("no_materials: " + std::to_string(meta_data.no_materials)).c_str());
            ImGui::Text("%s", std::string("no_positions: " + std::to_string(meta_data.no_positions)).c_str());
            ImGui::Text("%s", std::string("no_normals: " + std::to_string(meta_data.no_normals)).c_str());
            ImGui::Text("%s", std::string("no_tangents: " + std::to_string(meta_data.no_tangents)).c_str());
            ImGui::Text("%s", std::string("no_tex_coords: " + std::to_string(meta_data.no_tex_coords)).c_str());
            ImGui::Text("%s", std::string("no_indices: " + std::to_string(meta_data.no_indices)).c_str());
            ImGui::Text("%s", std::string("no_bones: " + std::to_string(meta_data.no_bones)).c_str());
            ImGui::Text("%s", std::string("no_transform: " + std::to_string(meta_data.no_transform)).c_str());
            ImGui::Text("%s", std::string("no_animations: " + std::to_string(meta_data.no_animations)).c_str());
        }
    }

    void list(){
        if(ImGui::BeginListBox("Models")){
            for(int i = 0; i < selection_list.size(); ++i){
                bool is_selected = i == selection_list.selected_id;
                int prev_selection = selection_list.selected_id;
                if(ImGui::Selectable(selection_list[i].c_str(), is_selected)){
                    selection_list.toggle_selected(i);
                }
            }
            ImGui::EndListBox();
        }
    }
public:
    void load_meta_data(const MeshLoaderNew::MeshMetaData& data){
        meta_data = data;
        no_animations = data.no_animations;
        m_flags = data.flags;
    }

    MeshLoaderNew::MeshRenderId create(){
        return std::get<0>(MeshLoaderNew::load_mesh(selection_list.get_selection()));
    }

    void display_mesh_flags(){
        if(ImGui::CollapsingHeader("Mesh Flags")){
            for(int i = 0; i < MeshLoaderNew::MESHFLAG::MESHFLAG_COUNT; ++i){
                bool is_set = m_flags[i];
                ImGui::Checkbox(MeshLoaderNew::MESHFLAG::meshflag_to_str[i].c_str(), &is_set);
            }
        }
    }

    void deselect(){selection_list.deselect();}

    void set_current_selection(int id){selection_list.selected_id = id;}
    int get_current_selection()  {return selection_list.selected_id;}
    std::string& get_selection() {return selection_list.get_selection();}
    bool has_selection()         {return selection_list.has_selection();}

    void update(){
        list();
        display_mesh_flags();
        meta_data_display();
    }

    ModelManagerGuiComponent()
        :selection_list(&save_file.model_locations)
    {
        deselect();
    }

};

class RepresentationModelManagerGui : public GuiBaseSingleton<RepresentationModelManagerGui>{
    CollisionManagerGuiComponent collision_manager;
    ModelManagerGuiComponent model_manager;
    RepresentationManagerGuiComponent representation_manager;
public:

    void copy_over_data(){
        representation_manager.copy_over();
    }

    bool valid_for_bullet_add(){return collision_manager.has_selection() && model_manager.has_selection(); representation_manager.has_selection();}
    void deselect_components(){
        collision_manager.deselect();
        model_manager.deselect();
        representation_manager.deselect();
    }

    void bullet_settings(){
        if(ImGui::Button("Move to bullet")){
            if(valid_for_bullet_add()){
                representation_manager.move_selected_to_bullet(collision_manager.create(model_manager.get_selection()));
                deselect_components();
            }
        }

    }

    // should pass playing animation too maybe or query it
    void animation(){
        if(ImGui::CollapsingHeader("Animation")) {
            int set_animation = model_manager.play_animation;
            if(model_manager.no_animations < 1)
                return;
            ImGui::InputInt("playing animation", &set_animation);
            if (set_animation >= 0 && set_animation < model_manager.no_animations && set_animation != model_manager.play_animation) {
                model_manager.play_animation = set_animation;
                auto &registry = EditorRegistry::get().m_registry;
                auto [status, e] = representation_manager.get_entity();
                if (status) {
                    auto &animation_controller = registry.get<Component::AnimationController>(e);
                    animation_controller.playing_animation = set_animation;
                }
            }
        }
    }

    // TODO: here some order is set. so may have functions call to make it more clear
    void update(){
        ImGui::Begin("RepresentationModelManagerGui");
        auto* additional_data = representation_manager.get_additional_data();

        RepresentationPointer* ptr = nullptr;
        CollisionManagerGuiData* collision_data_ptr = nullptr;
        if(additional_data != nullptr){
            ptr = &(additional_data->pointer);
            collision_data_ptr = &(additional_data->collision_data);
        }
        representation_manager.update();


        auto model_has_updated =     [&ptr](int model)      { if(ptr == nullptr) return false; return ptr->model != model; };
        auto collision_has_updated = [&ptr](int collision)  { if(ptr == nullptr) return false; return ptr->collision != collision; };
        
        if(ptr != nullptr){
            model_manager.set_current_selection(ptr->model);
            collision_manager.set_current_selection(ptr->collision);
            collision_manager.set_data(*collision_data_ptr);
        }
        else{
            model_manager.deselect();
            collision_manager.deselect();
        }

        model_manager.update();
        collision_manager.update();

        auto current_model = model_manager.get_current_selection();
        auto current_collision = collision_manager.get_current_selection();

        if(collision_data_ptr != nullptr){
            *collision_data_ptr = collision_manager.get_data();
        }

        // need to return flag
        if(model_has_updated(current_model)){
            if(model_manager.has_selection())
                model_manager.load_meta_data(representation_manager.set_render_for_selected(model_manager.get_selection()));
            else
                representation_manager.delete_render_for_selected();
        }


        if(ptr != nullptr){
            *ptr = RepresentationPointer(current_model, current_collision);
        }

        animation();
        bullet_settings();

        ImGui::End();
    }

};


#endif // EDITORGUI_H_

#ifndef NEW_GUI_H_
#define NEW_GUI_H_

#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "TextureFunctions.h"
#include "entt/entt.hpp"
#include <memory>
#include <vector>
#include "Representation.h"
#include "Utils.h"
#include <tuple>
#include "glm/glm.hpp"
#include "Components.h"
#include "Utils.h"
#include "Actor.h"
#include "MeshFunctions2.h"
#include "Actor.h"
#include "GuiHelpers.h"
#include "EditorRegistry.h"
#include "bullet.h"

class NewGui{
public:
    // need to delete from list when moving
    // delete from this
    RepresentationSelectionList editor_list;
    RepresentationSelectionList characters_list;
    RepresentationSelectionList bullet_list;

    //GuiGlobalVariablesSingleton& global_variables;

    RepresentationManager<RepresentationCharacter>&       character_representation_manager;
    RepresentationManager<EditorRepresentation>&          editor_manager;
    RepresentationManager<BulletRepresentation>&          bullet_manager;

public:



// ghost obj not set
    void characters() {
        ImGui::Begin("Characters selection");
        if (characters_list.has_selection()) {
            auto key = characters_list.get_selection().key;
            auto id = characters_list.selected_id;

            if (ImGui::Button("controll")) {
                auto& representation = character_representation_manager.get_representation(key);
                move_character_to_singleton(representation.character);
                characters_list.throw_away(id);
            }
            if (ImGui::Button("delete")) {
                character_representation_manager.delete_representation(key);
                characters_list.throw_away(id);

                bool has = 0;
                for(int i = 0; i < characters_list.representations.size(); ++i){
                    if(characters_list.representations[i].key.key == key.key)
                        has = 1;
                }
                assert(!characters_list.has_selection());
                assert(!has);
            }
        }
        ImGui::End();
    }

    void characters_display_list() {
        ImGui::Begin("Characters");
        if (ImGui::Button("Create character")) {
            glm::vec3 position(0, 0, 0);
            glm::vec3 rotation(0, 0, 0);
            glm::vec3 scale(1.f, 1.f, 1.f);
            auto key = character_representation_manager.create_character_representation(1,
                position, rotation, scale);

            characters_list.add_representation(key);
        }

        ImGui::Text("Representation List");
        for (int i = 0; i < characters_list.size(); ++i){
            auto &representation =
                character_representation_manager.get_representation(characters_list[i].key);

            const auto e = representation.character->e;

            bool is_selected = i == characters_list.selected_id;
            if(ImGui::Selectable(std::to_string((int)e).c_str(), is_selected)){
                characters_list.toggle_selected(i);
            }
        }

        ImGui::End();
    }

    void bullet_selection(){
        ImGui::Begin("Bullet");
        if(bullet_list.has_selection()){
            if(ImGui::Button("delete")){
                auto& selection = bullet_list.get_selection();
                auto& key = selection.key;
                auto e = bullet_manager.get_representation(key).get_entity_id();
                BulletScene::get().stop_manage_representation(key);
                Registry::get().registry.destroy(e);
                bullet_list.throw_away(bullet_list.selected_id);
            }
        }
       ImGui::End();
    }

    void bullet_display_list(){
        ImGui::Begin("Bullet");
        for(int i = 0; i < bullet_list.size(); ++i){
            bool is_selected = i == bullet_list.selected_id;
            if(ImGui::Selectable(std::to_string(i).c_str(), is_selected)){
                bullet_list.toggle_selected(i);
            }
        }
        ImGui::End();
    }

    void selection(){
        auto* registry = &EditorRegistry::get().m_registry;
        auto& manager = RepresentationManager<EditorRepresentation>::get();

        ImGui::Begin("Selection");

        if(editor_list.has_selection()){
            auto& selection = editor_list.get_selection();
            auto& key = selection.key;
            auto& rep = editor_manager.get_representation(key);

            ImGui::InputFloat3("position", (float*)&rep.position);
            ImGui::InputFloat3("rotation", (float*)&rep.rotation);
            ImGui::InputFloat3("scale",    (float*)&rep.scale);

            if(key.registry_owner == RegistryOwner::EDITORREGISTRY){
                if(ImGui::Button("Add Physics")){
                    bullet_list.add_representation(editor_manager.move_to_bullet(key));
                    editor_list.throw_away(editor_list.selected_id);
                }
            }
            else if(key.registry_owner == RegistryOwner::BULLETREGISTRY){
                if(ImGui::Button("Remove Physics")){
                }
            }
            ImGui::SameLine();
            ImGui::Button("DELETE");
        }
        ImGui::End();
    }

    void list(){
        ImGui::Begin("Representations");

        ImGui::BeginGroup();
        ImGui::Text("Add representation");
        if (ImGui::Button("Ball")) {
            auto key = editor_manager.create_primitive_representation(RepresentationType::SPHERE, glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(2,2,2));
            editor_list.add_representation(key);
        }

        ImGui::SameLine();
        if (ImGui::Button("Box")) {
            auto key = editor_manager.create_primitive_representation(RepresentationType::BOX, glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(2,2,2));
            editor_list.add_representation(key);
        }
        ImGui::EndGroup();
        ImGui::Text("Representation List");
        for(int i = 0; i < editor_list.size(); ++i){
            auto& representation = 
                editor_manager.get_representation(editor_list[i].key);
            const auto e = representation.get_entity_id();
            bool is_selected = i == editor_list.selected_id;
            if(ImGui::Selectable(std::to_string((int)e).c_str(), is_selected)){
                editor_list.toggle_selected(i);
            }
        }
        ImGui::End();
    }
    void render(){
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    void update(){
        list();
        selection();
        characters();
        characters_display_list();
        bullet_display_list();
    }

    void new_frame(){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    void init(GLFWwindow* window){
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplGlfw_InitForOpenGL(window, 1);
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui::StyleColorsDark();
    }

    NewGui(const NewGui&)=delete;
    NewGui& operator=(const NewGui&)=delete;
    ~NewGui()=default;

    explicit NewGui():
        character_representation_manager(RepresentationManager<RepresentationCharacter>::get()),
        editor_manager(RepresentationManager<EditorRepresentation>::get()),
        bullet_manager(RepresentationManager<BulletRepresentation>::get())
    {
    }

    static NewGui& get(){
        static NewGui instance;
        return instance;
    }
};



#endif // NEW_GUI_H_


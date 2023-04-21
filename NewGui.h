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
#include "ModelSaveFile.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "EditorGui.h"

namespace GUI_WIDGETS{

class TextureGui : public GuiBaseSingleton<TextureGui>{
    std::vector<std::tuple<std::string, unsigned int>> rendered_textures;
    std::unordered_map<std::string, int> texture_string_name_count;


private:
    void render_textures(){
        ImGui::Begin("Texture");
        for(auto& x : rendered_textures){
            ImGui::Begin(std::get<0>(x).c_str());
                ImVec2 wsize = ImGui::GetWindowSize();
                ImGui::Image((ImTextureID)std::get<1>(x), wsize, ImVec2(0, 1), ImVec2(1, 0));
            ImGui::End();
        }
        ImGui::End();
    }
public:

    void add_render_texture(const std::string& text, unsigned int texture_id){
        int count = ++texture_string_name_count[text];
        if(count < 1){
            rendered_textures.push_back(std::make_tuple(text + std::to_string(count), texture_id));
            return;
        }
        rendered_textures.push_back(std::make_tuple(text, texture_id));
    }

    void remove_render_texture(unsigned int texture_id){
        const int size = rendered_textures.size();
        for(int i = 0; i < size; ++i){
            auto& ref = rendered_textures[i];
            if(texture_id == std::get<1>(ref)){
                std::swap(ref, rendered_textures[size-1]);
                rendered_textures.pop_back();
                --texture_string_name_count[std::get<0>(ref)];
            }
        }
    }
    void update(){
        render_textures();
    }
};

class CharacterManagerGui : public GuiBaseSingleton<CharacterManagerGui>{
    RepresentationSelectionList characters_list;
    CharacterRepresentationManager& character_representation_manager;

public:
    void characters() {
        ImGui::BeginChild("Characters");

        if(ImGui::CollapsingHeader("Selection")){
            if (characters_list.has_selection()) {
                auto key = characters_list.get_selection();
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
                            if(characters_list.representations[i] == key)
                                has = 1;
                        }
                        assert(!characters_list.has_selection());
                        assert(!has);
                    }
                }
        }
        ImGui::EndChild();
    }

    void characters_display_list() {
        ImGui::BeginChild("Characters");
        if(ImGui::CollapsingHeader("Add new character")){
            if (ImGui::Button("Create character")) {
                glm::vec3 position(0, 0, 0);
                glm::vec3 rotation(0, 0, 0);
                glm::vec3 scale(1.f, 1.f, 1.f);
                auto key = character_representation_manager.create_character_representation(CharacterTypes::SHOOTER,
                    position, rotation, scale);

                characters_list.add_representation(key);
            }
        }

        if(ImGui::CollapsingHeader("Characters List")){
            for (int i = 0; i < characters_list.size(); ++i){
                auto &representation =
                    character_representation_manager.get_representation(characters_list[i]);

                const auto e = representation.character->e;

                bool is_selected = i == characters_list.selected_id;
                if(ImGui::Selectable(std::to_string((int)e).c_str(), is_selected)){
                    characters_list.toggle_selected(i);
                }

            }
        }

        ImGui::EndChild();
    }
    void update(){
        ImGui::Begin("Characters");
        characters_display_list();
        characters();
        ImGui::End();
    }
    CharacterManagerGui():
        character_representation_manager(CharacterRepresentationManager::get())
    {

    }
};

}



// delet this
class NewGui{
public:
    void render(){}
    void update(){}
    void new_frame(){}
    NewGui(const NewGui&)=delete;
    NewGui& operator=(const NewGui&)=delete;
    ~NewGui()=default;
    NewGui(){}

    static NewGui& get(){
        static NewGui instance;
        return instance;
    }
};

namespace GUI_USER_CODE{
    inline void new_frame(){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    inline void render(){
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    inline void init(GLFWwindow* window){
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplGlfw_InitForOpenGL(window, 1);
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui::StyleColorsDark();
    }

    // views and such and toggle them
    inline void update(){
        GUI_WIDGETS::TextureGui::get().update();
        GUI_WIDGETS::CharacterManagerGui::get().update();

        RepresentationModelManagerGui::get().update();

        NewGui::get().update();
    }

}

#endif // NEW_GUI_H_


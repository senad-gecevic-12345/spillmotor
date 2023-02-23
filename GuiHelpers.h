#ifndef GUIHELPERS_H_
#define GUIHELPERS_H_

#include "Representation.h"

struct Selection{
    RepresentationKey key; 
    bool operator==(const Selection& selection){
        return key == selection.key;
    }
    Selection(RepresentationKey key):key(key){}
};

template<typename T>
struct SelectionData{
    T data;
    bool operator==(const SelectionData<T>& selection){
        if(this == &selection)
            return 1;
        return data == selection.data;
    }
    SelectionData(T data):data(data){
    }
};

template<typename T>
struct SelectionList{
    int selected_id{-1};
    std::vector<SelectionData<T>> representations;

    std::size_t size(){return representations.size();}

    typename std::vector<SelectionData<T>>::iterator begin(){
        return representations.begin();
    };
    typename std::vector<SelectionData<T>>::iterator end(){
        return representations.end();
    };
    void add_representation(T data){
        representations.push_back(SelectionData<T>(data));
    }
    bool has_selection(){
        return selected_id != -1;
    }
    SelectionData<T>& get_selection(){
        return representations[selected_id];
    }
    void deselect(){
        selected_id = -1;
    }
    void toggle_selected(SelectionData<T>& selection){
        for(int i = 0; i < representations.size(); ++i){
            if(representations[i] == selection){
                if(i == selected_id){
                    deselect();
                }
                else{
                    deselect();
                    representations[i].is_selected = 1;
                    selected_id = i;
                }
                return;
            }
        }
    }

    SelectionData<T>& operator[](int i){
        return representations[i];
    }


};

struct RepresentationSelectionList{
    int selected_id{-1}; 
    std::vector<Selection> representations;
    using RepType = std::vector<Selection>;
    std::size_t size(){return representations.size();}
    RepType::iterator begin(){
        return representations.begin();
    };
    RepType::iterator end(){
        return representations.end();
    };
    void add_representation(RepresentationKey key){
        representations.push_back(Selection(key));
    }
    void deselect(){
        selected_id = -1;
    }
    void throw_away(int i){
        assert(i < representations.size());
        assert(i > -1);
        std::swap(representations[i], representations[representations.size()-1]);
        representations.pop_back();
        selected_id = -1;
    }
    bool has_selection(){
        return selected_id != -1;
    }
    Selection& get_selection(){
        assert(selected_id > -1);
        assert(selected_id < representations.size());
        return representations[selected_id];
    }
    
    void toggle_selected(int i){
        if(i == selected_id)
            selected_id = -1;
        else 
            selected_id = i;
    }
    Selection& operator[](int i){
        return representations[i];
    }
};


inline void emplace_new_mesh_to_entt(std::string file_loc_name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale){
    auto& registry = Registry::get().registry;
    auto e = registry.create();
    registry.emplace<Tags::RepresentationRenderNewMeshLoader>(e);
    auto& mesh = registry.emplace<MeshLoaderNew::Mesh>(e);
    auto& position = registry.emplace<Component::Position>(e);
    position.pos = _position;
    auto& rotate = registry.emplace<Component::Rotation>(e);
    rotate.x = (float)_rotation.x; rotate.y = (float)_rotation.y; rotate.z = (float)_rotation.z;
    auto& scale = registry.emplace<Component::Scale>(e, _scale);

    bool status = MeshLoaderNew::load_mesh(file_loc_name, mesh);
    std::cout << "\nstatus: " << status << "\n";
}

#endif // GUIHELPERS_H_



#ifndef GUIGLOBALVARIABLESSINGLETON_H_
#define GUIGLOBALVARIABLESSINGLETON_H_

#include <vector>
#include <deque>
#include <array>

namespace MeshLoaderNew{
    struct MeshGlobalVariable;
};


// need a key
template<typename T>
class GuiGlobalVariablesSingleton{
public:
    std::vector<T> assets;
    
    static GuiGlobalVariablesSingleton<T>& get(){
        static GuiGlobalVariablesSingleton instance;
        return instance;
    }

    friend class ComponentSelectionBase;
    friend class NewGui;
};



namespace{
    static constexpr int ARRAY_MESH_MAX_UNIQUE_COUNT{64};
};


class AnimationMeshStorage{
public:
    struct AnimationLocation{
        int location{-1};
        explicit AnimationLocation(int location):location(location){}
        AnimationLocation(const AnimationLocation& other)=default;
        AnimationLocation& operator=(const AnimationLocation& other)=default;
        //AnimationLocation(const AnimationLocation&& other):location(std::move(other.location)){};
        //AnimationLocation& operator=(const AnimationLocation&& other){location=std::move(other.location);};
        int& operator()(){
            return location;
        }
    };
private:

    // do linear search and not by string
    //std::unordered_map<std::string, int> file_loc_name_hash;
    std::array<MeshLoaderNew::MeshAnimationData, ARRAY_MESH_MAX_UNIQUE_COUNT> array;
    std::array<int, ARRAY_MESH_MAX_UNIQUE_COUNT> available_ids = {1};

    int count{0};

public:

    int size()const{return count;}
    bool has_available_storage()const{return count < ARRAY_MESH_MAX_UNIQUE_COUNT;}

    MeshLoaderNew::MeshAnimationData* get_mesh(int location){
        return &array[location];
    }

    AnimationLocation reserve_mesh(){
        if(count >= ARRAY_MESH_MAX_UNIQUE_COUNT){
            return AnimationLocation{-1};
        }
        int out = count;
        ++count;
        return AnimationLocation(out);
        //return {count++};
    }
    bool free_mesh(AnimationLocation loc){
        available_ids[loc()] = 1;
        array[loc()].importer.FreeScene();
        return 1;
    }

    MeshLoaderNew::MeshAnimationData* get_new_mesh(const std::string& file_loc_name){
        if(!has_available_storage())
            return nullptr;
        ++count;
        return &array[count-1];
    }

    MeshLoaderNew::MeshAnimationData& operator[](int x){
        return array[x];
    }
    static AnimationMeshStorage& get(){
        static AnimationMeshStorage instance;
        return instance;
    }

};


class MeshStorage{
    std::unordered_map<std::string, MeshLoaderNew::Mesh> file_loc_name_hash;

public:
    MeshLoaderNew::Mesh* get_mesh(const std::string& file_loc_name){
        return &file_loc_name_hash[file_loc_name];
    }
    
    MeshLoaderNew::Mesh& emplace_back(MeshLoaderNew::Mesh&& mesh, const std::string& file_loc_name){
        file_loc_name_hash[file_loc_name] = std::move(mesh);
        return file_loc_name_hash[file_loc_name];
    }

    static MeshStorage& get(){
        static MeshStorage instance;
        return instance;
    }

};

struct LoadMeshData{
    MeshLoaderNew::Mesh* mesh{nullptr};
    MeshLoaderNew::MeshFlags flags{};
    int no_animations{0};
    AnimationMeshStorage::AnimationLocation animation_location{-1};
    LoadMeshData()=default;
    ~LoadMeshData()=default;
    LoadMeshData(const LoadMeshData& other):
        mesh(other.mesh), flags(other.flags), animation_location(other.animation_location), no_animations(other.no_animations){}
    LoadMeshData& operator=(const LoadMeshData& other){
        if(this == &other)
            return *this;
        mesh=(other.mesh);
        flags=(other.flags);
        animation_location=(other.animation_location);
        no_animations=(other.no_animations);
        return *this;
    }
};

// seems so dumb
class MeshLoadDataStorage{
public:
    std::unordered_map<std::string, LoadMeshData> map;
    static MeshLoadDataStorage& get(){
        static MeshLoadDataStorage instance;
        return instance;
    }
};


#endif // GUIGLOBALVARIABLESSINGLETON_H_


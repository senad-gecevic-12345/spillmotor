#ifndef MESHFUNCTIONSTWO_H_
#define MESHFUNCTIONSTWO_H_

#include <vector>
#include <string>
#include "Components.h"
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "BulletCollision/CollisionShapes/btTriangleMesh.h"
#include <bitset>
#include <array>
#include <unordered_map>
#include <assimp/mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <utility>

struct LoadMeshData;

namespace MeshLoaderNew{

    namespace{
        static constexpr unsigned int MAX_BONES_PER_VERTEX{4};
        static constexpr unsigned int MAX_BONES{200};
    }


    enum BUFFER_TYPE{
        POS,
        NORMAL,
        TEXCOORD,
        TANGENT,
        BONE,
        BONETRANSFORM,
        BUFFER_TYPE_COUNT,
        INDEX
    };

    namespace MESHFLAG{
        enum{
            DIFFUSE = 0,
            NORMALMAP = 1,
            ROUGHNESSMAP = 2,
            BONES = 3,
            MESHFLAG_COUNT
        };
        static std::unordered_map<int, std::string> meshflag_to_str{
            {DIFFUSE, "DIFFUSE"},
            {NORMALMAP, "NORMALMAP"},
            {ROUGHNESSMAP, "ROUGHNESSMAP"},
            {BONES, "BONES"}
        };
    }

    using MeshFlags = std::bitset<MESHFLAG::MESHFLAG_COUNT>;

    struct BoneData;   

    struct PBRMaterial{
        float roughness;
        bool is_metal;
        glm::vec3 color;
    };

    struct Material{
        unsigned int VAO;
        glm::vec3 ambient_color;
        glm::vec3 diffuse_color;
        glm::vec3 specular_color;
        PBRMaterial material;
        Component::Texture diffuse;
        Component::Texture specular_exponent;
        Component::Texture normal;
        Component::Texture roughness;
    };


    // MeshOffset
    struct BasicMeshEntry{
        unsigned int no_indices;
        unsigned int base_vertex;
        unsigned int base_index;
        unsigned int material_index;
    };

    class Importer{
    public:
        Assimp::Importer importer;
        static Importer& get(){
            static Importer instance;
            return instance;
        }
    };
    

    struct Mesh{
        unsigned int VAO;
        std::vector<BasicMeshEntry> meshes;
        std::vector<Material> materials;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> tangents;
        std::vector<glm::vec2> tex_coords;
        std::vector<unsigned int> indices;
        std::vector<BoneData> bones;

        std::array<unsigned int, BUFFER_TYPE_COUNT> buffers;

        Mesh()=default;
        ~Mesh()=default;
        Mesh(const Mesh& other)=delete;
        Mesh& operator=(const Mesh& other)=delete;

        Mesh(Mesh&& other)noexcept:
            VAO(std::move(other.VAO)),
            meshes(std::move(other.meshes)),
            materials(std::move(other.materials)),
            positions(std::move(other.positions)),
            normals(std::move(other.normals)),
            tangents(std::move(other.tangents)),
            tex_coords(std::move(other.tex_coords)),
            indices(std::move(other.indices)),
            buffers(std::move(other.buffers))
        {}

        Mesh& operator=(Mesh&& other){
            VAO = std::move(other.VAO);
            meshes = std::move(other.meshes);
            materials = std::move(other.materials);
            positions = std::move(other.positions);
            normals = std::move(other.normals);
            tangents = std::move(other.tangents);
            tex_coords = std::move(other.tex_coords);
            indices = std::move(other.indices);
            buffers = std::move(other.buffers);
            return *this;
        }
        explicit Mesh(unsigned int load_flags, Assimp::Importer& importer, const std::string& file_loc_name)
        {}

    };

    struct MeshRenderId{
        unsigned int VAO;
        std::vector<BasicMeshEntry> meshes;
        std::vector<Material> materials;
        unsigned int buffers[BUFFER_TYPE_COUNT];
    };


    inline MeshRenderId create_mesh_render_id(const Mesh& mesh){
        MeshRenderId out;
        out.VAO = mesh.VAO;
        for(int i = 0; i < BUFFER_TYPE_COUNT; ++i){
            out.buffers[i] = mesh.buffers[i];
        }
        for(auto x : mesh.meshes){
            out.meshes.push_back(x);
        }
        for(auto x : mesh.materials){
            out.materials.push_back(x);
        }
        return out;
    }

    enum EXTENSIONS{
        INVALID = -1,
        PNG = 0, 
        JPG = 1
    };


    struct MeshMetaData{
        MeshFlags flags;
        unsigned int no_meshes;
        unsigned int no_materials;
        unsigned int no_positions;
        unsigned int no_normals;
        unsigned int no_tangents;
        unsigned int no_tex_coords;
        unsigned int no_indices;
        unsigned int no_bones;
        unsigned int no_transform;
        unsigned int no_animations;
        MeshMetaData():
            flags(),
            no_meshes(0),
            no_materials(0),
            no_positions(0),
            no_normals(0),
            no_tangents(0),
            no_tex_coords(0),
            no_indices(0),
            no_bones(0),
            no_transform(0),
            no_animations(0)
        {}
        MeshMetaData(
            MeshFlags flags,
            unsigned int no_meshes,
            unsigned int no_materials,
            unsigned int no_positions,
            unsigned int no_normals,
            unsigned int no_tangents,
            unsigned int no_tex_coords,
            unsigned int no_indices,
            unsigned int no_bones,
            unsigned int no_transform,
            unsigned int no_animations
        ):
            flags(flags),
            no_meshes(no_meshes),
            no_materials(no_materials),
            no_positions(no_positions),
            no_normals(no_normals),
            no_tangents(no_tangents),
            no_tex_coords(no_tex_coords),
            no_indices(no_indices),
            no_bones(no_bones),
            no_transform(no_transform),
            no_animations(no_animations)
        {}
    };

    struct MeshAnimationData{
        Assimp::Importer importer;
        const aiScene* scene;
        glm::mat4 inv;
        std::vector<BoneData> bones;
        std::unordered_map<std::string, int> bone_name_to_index;

        std::vector<glm::mat4> transforms;
        std::vector<glm::mat4> offsets;

    };


    std::tuple<MeshRenderId, MeshFlags> load_mesh(const std::string& file_loc_name);
    LoadMeshData load_mesh_with_meta_data(const std::string& file_loc_name);
    void load_create_triangle_mesh(const std::string& file_loc_name, btTriangleMesh& target);
    btConvexHullShape* load_create_convex_mesh(const std::string& file_loc_name);


    struct BoneData{
        std::array<int, MAX_BONES_PER_VERTEX> ids;
        std::array<float, MAX_BONES_PER_VERTEX> weights = {0};
        BoneData(){
            for(int i = 0; i < MAX_BONES_PER_VERTEX; ++i){
                ids[i] = -1;
            }
        }
        void add(unsigned int id, float w){
            for(int i = 0; i < MAX_BONES_PER_VERTEX; ++i){
                if(weights[i] == 0){
                    ids[i] = id;
                    weights[i] = w;
                    return;
                }
            }
            assert(0);
        }
    };

    MeshLoaderNew::MeshAnimationData* get_bone_transform(float time_in_seconds, Component::AnimationController anim_controller);

}

#endif // MESHFUNCTIONSTWO_H_

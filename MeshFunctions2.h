#ifndef MESHFUNCTIONSTWO_H_
#define MESHFUNCTIONSTWO_H_

#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "Components.h"
//#include "GuiGlobalVariablesSingleton.h"
//
namespace MeshLoaderNew{
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
        // has no bind function
        Component::Texture diffuse;
        Component::Texture specular_exponent;
    };

    // MeshOffset
    struct BasicMeshEntry{
        unsigned int no_indices;
        unsigned int base_vertex;
        unsigned int base_index;
        unsigned int material_index;
    };

    enum BUFFER_TYPE{
        INDEX = 0,
        POS       = 1,
        TEXCOORD  = 2,
        NORMAL    = 3,
    };


    // no probably needs it for the offset
    // one mesh or several meshes? like here there is no duplacetes for the meshes
    // but this joins the scene, no?
    // the tree has 4797 meshes
    struct Mesh{
        unsigned int VAO;
        // VBO in basic mesh entry or material
        std::vector<BasicMeshEntry> meshes;
        std::vector<Material> materials;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> tex_coords;
        std::vector<unsigned int> indices;
        unsigned int buffers[4];
    };



    struct MeshRenderId{
        unsigned int VAO;
        std::vector<BasicMeshEntry> meshes;
        std::vector<Material> materials;
        unsigned int buffers[4];
    };

    inline MeshRenderId create_mesh_render_id(const Mesh& mesh){
        MeshRenderId out;
        out.VAO = mesh.VAO;
        for(int i = 0; i < 4; ++i){
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
        PNG, 
        JPG
    };

    bool load_mesh(std::string file_loc_name, Mesh& target);

    struct MeshGlobalVariable{
        std::string file_loc_name;
        Mesh mesh;
    };

    MeshRenderId load_mesh(std::string file_loc_name);

}

#endif // MESHFUNCTIONSTWO_H_

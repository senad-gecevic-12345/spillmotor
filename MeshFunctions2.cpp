#include "MeshFunctions2.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <iostream>
#include <cstring>
#include "Components.h"
#include "TextureFunctions.h"
#include "GuiGlobalVariablesSingleton.h"


namespace MeshLoaderNew{
    std::string get_directory(const std::string& str){
#ifndef _WIN32
            const char dir_sep = '/';
#else
            const char dir_sep = '\\';
#endif
        if(str.size() < 2)
            return std::string("INVALID"); 

        for(int i = str.size(); i > 1; --i){
            if(str[i-1] == dir_sep){
                return str.substr(0, i);
            }
        }

        return std::string("INVALID");
    }

    int get_extension(const std::string& str){
        char extension[10];
        //std::string extension;
        int counter{0};
        for(int i = str.size()-1; i >= 0; --i){

            if(str[i] == '.'){
                // wrong
                extension[counter] = '\0';
                // add counter
                break;
            }
            //extension.push_back(str[i]);
            extension[counter] = str[i];

            ++counter;

            // this is not correct
            if(counter >= 9 || i == 0)
                return EXTENSIONS::INVALID;
        }
        char temp;
        char* end = &extension[counter-1];
        char* start = &extension[0];
        while(start < end){
            temp = *end;
            *end = *start;
            *start = temp;
            ++start;
            --end;
        }
        if(strcmp(extension, "png") == 0){
            return EXTENSIONS::PNG;
        }
        else if(strcmp(extension, "jpeg") == 0){
            return EXTENSIONS::JPG;
        }
        else if(strcmp(extension, "jpg") == 0){
            return EXTENSIONS::JPG;
        }
    
        return EXTENSIONS::INVALID;
    }

    void count_vertices_and_indices(const aiScene* scene, unsigned int& no_vertices, unsigned int& no_indices, Mesh& mesh){
        for(std::size_t i = 0; i < mesh.meshes.size(); ++i){
            mesh.meshes[i].material_index = scene->mMeshes[i]->mMaterialIndex;
            // this may be false if is triangulated
            mesh.meshes[i].no_indices = scene->mMeshes[i]->mNumFaces * 3;
            mesh.meshes[i].base_vertex = no_vertices;
            mesh.meshes[i].base_index = no_indices;
            no_vertices += scene->mMeshes[i]->mNumVertices;
            no_indices  += mesh.meshes[i].no_indices;
        }
    }

    void reserve_space(const unsigned int& no_vertices, const unsigned int& no_indices, Mesh& mesh){
        mesh.positions.reserve(no_vertices);
        mesh.normals.reserve(no_vertices);
        mesh.tex_coords.reserve(no_vertices);
        mesh.indices.reserve(no_indices);
    }

    void load_colors(const aiMaterial* material, Material& mat){
        aiColor3D white(1.f, 1.f, 1.f);
        aiColor3D ambient_color (0, 0, 0);
        aiColor3D diffuse_color (0, 0, 0);
        aiColor3D specular_color(0, 0, 0);
        int shading_model = 0;
        if(material->Get(AI_MATKEY_SHADING_MODEL, shading_model) == AI_SUCCESS){

        }
        if(material->Get(AI_MATKEY_COLOR_AMBIENT, ambient_color) == AI_SUCCESS){
            mat.ambient_color.x = ambient_color.r;
            mat.ambient_color.y = ambient_color.g;
            mat.ambient_color.z = ambient_color.b;
        }
        else{
            // probably load white for all
            mat.ambient_color = glm::vec3(white.r, white.g, white.b);
        }
        if(material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color) == AI_SUCCESS){
            mat.diffuse_color.x = diffuse_color.r;
            mat.diffuse_color.y = diffuse_color.g;
            mat.diffuse_color.z = diffuse_color.b;
        }
        if(material->Get(AI_MATKEY_COLOR_SPECULAR, specular_color) == AI_SUCCESS){
            mat.specular_color.x = specular_color.r;
            mat.specular_color.y = specular_color.g;
            mat.specular_color.z = specular_color.b;
        }
    }

    bool load_texture_from_file(const std::string& file_loc_name, Component::Texture& texture){
        auto extension = get_extension(file_loc_name);
        switch(extension) {
            case EXTENSIONS::PNG:
                TextureFunctions::load_texture_png(file_loc_name, texture);
                break;
            case EXTENSIONS::JPG:
                TextureFunctions::load_texture_jpg(file_loc_name, texture);
                break;
            case EXTENSIONS::INVALID: default:
                return 0;
        }
        return 1;
    }

    bool load_diffuse_texture(const std::string& directory, const aiMaterial* material, Material& mat){
        if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0){
            aiString path;
            if(material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
                // append this to the directory
                
                // does only print sphere .png
                // prev
                //std::string texture_path(path.data);
                std::string texture_path  = directory + std::string(path.data);
                std::cout << "texture_path: " << texture_path << std::endl;

                if(load_texture_from_file(texture_path, mat.diffuse)){
                    return 1;
                }
            }
        }
        return 0;
    }

    bool load_specular_texture(const std::string& directory, const aiMaterial* material, Material& mat){
        if(material->GetTextureCount(aiTextureType_SHININESS) > 0){
            aiString path;
            if(material->GetTexture(aiTextureType_SHININESS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
                std::string texture_path(directory + path.data);
                if(load_texture_from_file(texture_path, mat.specular_exponent)){
                    return 1;
                }
            }
        }
        return 0;
    }

    void populate_buffers(Mesh& mesh){

        glGenVertexArrays(1, &mesh.VAO);
		glGenBuffers(1, &mesh.buffers[BUFFER_TYPE::POS]);
		glGenBuffers(1, &mesh.buffers[BUFFER_TYPE::NORMAL]);
        glGenBuffers(1, &mesh.buffers[BUFFER_TYPE::TEXCOORD]);
        glGenBuffers(1, &mesh.buffers[BUFFER_TYPE::INDEX]);

        glBindVertexArray(mesh.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BUFFER_TYPE::POS]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*mesh.positions.size(), &mesh.positions[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BUFFER_TYPE::NORMAL]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*mesh.normals.size(), &mesh.normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BUFFER_TYPE::TEXCOORD]);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec2) * mesh.tex_coords.size(), &mesh.tex_coords[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.buffers[BUFFER_TYPE::INDEX]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*mesh.indices.size(), &mesh.indices[0], GL_STATIC_DRAW);
    }

    void set_specular_exponent_texture_unit(unsigned int tex){
    }
    void controll_specular_exponent(bool enable){
        if(enable){
        }
        else{
        }
    }

    void render_diffuse_temp(Mesh& mesh){
        glBindVertexArray(mesh.VAO);
        for(std::size_t i = 0; i < mesh.meshes.size(); ++i){
            auto mat_index = mesh.meshes[i].material_index;
            TextureFunctions::bind(mesh.materials[mat_index].diffuse);
            glDrawElementsBaseVertex(
                GL_TRIANGLES,
                mesh.meshes[i].no_indices,
                GL_UNSIGNED_INT,
                (void*)(sizeof(unsigned int) * mesh.meshes[i].base_index),
                mesh.meshes[i].base_vertex);
        }
    }

    void init_mesh(const aiMesh* mesh, Mesh& output){
        for(int i = 0; i < mesh->mNumVertices; ++i){
            const aiVector3D& pos = mesh->mVertices[i];
            output.positions.push_back(glm::vec3(pos.x, pos.y, pos.z));

            if(mesh->mNormals){ //??
                const aiVector3D& normal = mesh->mNormals[i];
                output.normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
            }
            else{
                output.normals.push_back(glm::vec3(0, 1, 0));
            }

            // should not be a vector 2d?
            const aiVector3D& texcoord =
                mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : aiVector3D(0, 0, 0);
            output.tex_coords.push_back(glm::vec2(texcoord.x, texcoord.y));
        };

        for(int i = 0;  i < mesh->mNumFaces; ++i){
            const aiFace& face = mesh->mFaces[i];
            output.indices.push_back(face.mIndices[0]);
            output.indices.push_back(face.mIndices[1]);
            output.indices.push_back(face.mIndices[2]);
        }
    };

    void init_from_scene(const aiScene* scene, const std::string& file_loc_name, Mesh& mesh){
        mesh.meshes.resize(scene->mNumMeshes);
        mesh.materials.resize(scene->mNumMaterials);

        unsigned int no_vertices{0};
        unsigned int no_indices{0};

        count_vertices_and_indices(scene, no_vertices, no_indices, mesh);

        reserve_space(no_vertices, no_indices, mesh);

        for(std::size_t i = 0; i < mesh.meshes.size(); ++i){
            const aiMesh* _mesh = scene->mMeshes[i];
            init_mesh(_mesh, mesh);
        }

        for(unsigned int i = 0; i < scene->mNumMaterials; ++i){
            const aiMaterial* material = scene->mMaterials[i];

            auto directory = get_directory(file_loc_name);
            load_diffuse_texture (directory, material, mesh.materials[i]);
        }

        populate_buffers(mesh);
    }

    bool load_mesh(std::string file_loc_name, Mesh& target){
        Assimp::Importer importer;

        unsigned int ASSIMP_LOAD_FLAGS = aiProcess_Triangulate;

        const aiScene* scene =
            importer.ReadFile(file_loc_name.c_str(), ASSIMP_LOAD_FLAGS);
        if(scene)
            init_from_scene(scene, file_loc_name, target);
        else
            return 0;

        glBindVertexArray(0);
        return 1;
    };








    MeshRenderId load_mesh(std::string file_loc_name){
        auto& meshes = MeshSingleton::get().meshes;

        MeshGlobalVariable gvar; 
        load_mesh(file_loc_name, gvar.mesh);
        auto out = create_mesh_render_id(gvar.mesh);
        meshes.emplace_back(std::move(gvar));
        return out;
    }






}


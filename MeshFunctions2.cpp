#include "MeshFunctions2.h" 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include "glm/glm.hpp"
#include <iostream>
#include <cstring>
#include "Components.h"
#include "TextureFunctions.h"
#include "GuiGlobalVariablesSingleton.h"

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from)
{
    glm::mat4 to;
    to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1; to[0][3] = (GLfloat)from->d1;
    to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2; to[1][3] = (GLfloat)from->d2;
    to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3; to[2][3] = (GLfloat)from->d3;
    to[3][0] = (GLfloat)from->a4; to[3][1] = (GLfloat)from->b4;  to[3][2] = (GLfloat)from->c4; to[3][3] = (GLfloat)from->d4;
    return to;
}
glm::mat3 aiMatrix3x3ToGlm(const aiMatrix3x3* from)
{
    glm::mat4 to;
    to[0][0] = (GLfloat)from->a1; to[0][1] = (GLfloat)from->b1;  to[0][2] = (GLfloat)from->c1;
    to[1][0] = (GLfloat)from->a2; to[1][1] = (GLfloat)from->b2;  to[1][2] = (GLfloat)from->c2;
    to[2][0] = (GLfloat)from->a3; to[2][1] = (GLfloat)from->b3;  to[2][2] = (GLfloat)from->c3;
    return to;
}



unsigned int find_position(float animation_ticks, const aiNodeAnim* anim){
    for(unsigned int i = 0; i < anim->mNumPositionKeys - 1; ++i){
        float t = (float)anim->mPositionKeys[i + 1].mTime;
        if(animation_ticks < t){
            return i;
        }
    }
    return 0;
}

unsigned int find_rotation(float animation_ticks, const aiNodeAnim* anim){
    for(unsigned int i = 0; i < anim->mNumRotationKeys - 1; ++i){
        float t = (float)anim->mRotationKeys[i + 1].mTime;
        if(animation_ticks < t){
            return i;
        }
    }
    return 0;
}

unsigned int find_scaling(float animation_ticks, const aiNodeAnim* anim){
    for(unsigned int i = 0; i < anim->mNumScalingKeys - 1; ++i){
        float t = (float)anim->mScalingKeys[i + 1].mTime;
        if(animation_ticks < t){
            return i;
        }
    }
    return 0;
}






namespace MeshLoaderNew{


    
    void parse_bone_for_mesh(const aiBone* bone, const BasicMeshEntry& mesh_entry, MeshAnimationData& anim, Mesh& mesh);

    std::string get_directory(const std::string& str){
#ifndef _WIN32
            const char dir_sep = '/';
#else
            const char dir_sep = '\\';
#endif
        if(str.size() < 2)
            return {"INVALID"};

        for(int i = str.size(); i > 1; --i){
            if(str[i-1] == dir_sep){
                return str.substr(0, i);
            }
        }

        return {"INVALID"};
    }

    int get_extension(const std::string& str){
        char extension[10];
        int counter{0};

        for(int i = str.size()-1; i >= 0; --i){
            // placed the wrong way
            if(str[i] == '.'){
                extension[counter] = '\0';
                break;
            }

            extension[counter] = str[i];
            ++counter;

            if(counter >= 9 || i == 0)
                return EXTENSIONS::INVALID;
        }

        if(counter < 1)
            return EXTENSIONS::INVALID;

        char temp;
        char* end =     &extension[counter-1];
        char* start =   &extension[0];

        while(start < end){
            temp = *end;
            *end = *start;
            *start = temp;
            ++start;
            --end;
        }

        if(strcmp(extension, "png") == 0){
            std::cout << "png" << std::endl;
            return EXTENSIONS::PNG;
        }
        else if(strcmp(extension, "jpeg") == 0){
            std::cout << "jpeg" << std::endl;
            return EXTENSIONS::JPG;
        }
        else if(strcmp(extension, "jpg") == 0){
            std::cout << "jpg" << std::endl;
            return EXTENSIONS::JPG;
        }
    
        std::cout << "invalid\n";
        return EXTENSIONS::INVALID;
    }

    struct SceneData{
        unsigned int no_vertices;
        unsigned int no_indices;
        unsigned int no_bones;
        SceneData():no_vertices(0), no_indices(0), no_bones(0){}
    };

    SceneData parse_meshes_and_return_total_count(const aiScene* scene, Mesh& mesh){
        SceneData out;
        for(std::size_t i = 0; i < mesh.meshes.size(); ++i){

            mesh.meshes[i].material_index = scene->mMeshes[i]->mMaterialIndex;
            mesh.meshes[i].no_indices = scene->mMeshes[i]->mNumFaces * 3;
            mesh.meshes[i].base_vertex = out.no_vertices;
            mesh.meshes[i].base_index = out.no_indices;

            out.no_vertices += scene->mMeshes[i]->mNumVertices;
            out.no_indices  += mesh.meshes[i].no_indices;
            out.no_bones    = scene->mMeshes[i]->mNumBones;

        }
        return out;
    }


    void count_vertices_and_indices(const aiScene* scene, unsigned int& no_vertices, unsigned int& no_indices, Mesh& mesh){
        for(std::size_t i = 0; i < mesh.meshes.size(); ++i){
            mesh.meshes[i].material_index = scene->mMeshes[i]->mMaterialIndex;
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

    bool load_texture_from_file(const std::string& file_loc_name, Component::Texture& texture, bool is_embedded = false, unsigned int size = 0, void* data = nullptr){
        if(is_embedded) {
            TextureFunctions::load_texture_embedded(texture, size, data);
            return 1;
        }
        auto extension = get_extension(file_loc_name);
        switch(extension) {
            case EXTENSIONS::PNG:{
                TextureFunctions::load_texture_png(file_loc_name, texture);
                return 1;
            }
            case EXTENSIONS::JPG:{
                TextureFunctions::load_texture_jpg(file_loc_name, texture);
                return 1;
             }
            case EXTENSIONS::INVALID: default:{
                return 0;
              }
        }
    }

    // load the embedded texture
    bool load_diffuse_texture(const std::string& directory, const aiMaterial* material, Material& mat, const aiScene* scene){
        if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0){
            aiString path;
            if(material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
                auto* embedded = scene->GetEmbeddedTexture(path.C_Str());
                std::string texture_path  = directory + std::string(path.data);
                if(embedded){
                    auto size = embedded->mWidth;
                    load_texture_from_file(texture_path, mat.diffuse, 1, size, embedded->pcData);
                    return 1;
                }
                if(load_texture_from_file(texture_path, mat.diffuse)){
                    return 1;
                }
            }
        }
        return 0;
    }

    bool load_normal_map_texture(const std::string& directory, const aiMaterial* material, Material& mat){
        if(material->GetTextureCount(aiTextureType_NORMALS) > 0){
            aiString path;

            if(material->GetTexture(aiTextureType_NORMALS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
                std::string texture_path  = directory + std::string(path.data);
                std::cout << "texture path height: " << texture_path << std::endl;
                if(load_texture_from_file(texture_path, mat.normal)){
                    return 1;
                }
                else{
                    return 0;
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
        return false;
    }

    void populate_buffers(Mesh& mesh){

        glGenVertexArrays(1, &mesh.VAO);
		glGenBuffers(1, &mesh.buffers[BUFFER_TYPE::POS]);
		glGenBuffers(1, &mesh.buffers[BUFFER_TYPE::NORMAL]);
        glGenBuffers(1, &mesh.buffers[BUFFER_TYPE::TEXCOORD]);
        glGenBuffers(1, &mesh.buffers[BUFFER_TYPE::INDEX]);

        glGenBuffers(1, &mesh.buffers[BUFFER_TYPE::TANGENT]);
        glGenBuffers(1, &mesh.buffers[BUFFER_TYPE::BONE]);

        glBindVertexArray(mesh.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BUFFER_TYPE::POS]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*mesh.positions.size(), &mesh.positions[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(BUFFER_TYPE::POS);
        glVertexAttribPointer(BUFFER_TYPE::POS, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BUFFER_TYPE::NORMAL]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*mesh.normals.size(), &mesh.normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(BUFFER_TYPE::NORMAL);
        glVertexAttribPointer(BUFFER_TYPE::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BUFFER_TYPE::TEXCOORD]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.tex_coords.size(), &mesh.tex_coords[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(BUFFER_TYPE::TEXCOORD);
        glVertexAttribPointer(BUFFER_TYPE::TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // new entry
        glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BUFFER_TYPE::TANGENT]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.tangents.size(), &mesh.tangents[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(BUFFER_TYPE::TANGENT);
        glVertexAttribPointer(BUFFER_TYPE::TANGENT, 3, GL_FLOAT, GL_FALSE, 0, 0);


        glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BUFFER_TYPE::BONE]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.bones[0]) * mesh.bones.size(), &mesh.bones[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(4);
        glVertexAttribIPointer(4, MAX_BONES_PER_VERTEX, GL_INT, sizeof(BoneData), (const void*)0);

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, MAX_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(BoneData), (const void*)(MAX_BONES_PER_VERTEX * sizeof(int)));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.buffers[BUFFER_TYPE::INDEX]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*mesh.indices.size(), &mesh.indices[0], GL_STATIC_DRAW);
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
            output.positions.emplace_back(pos.x, pos.y, pos.z);

            if(mesh->mNormals){ //??
                const aiVector3D& normal = mesh->mNormals[i];
                output.normals.emplace_back(normal.x, normal.y, normal.z);
            }
            else{
                output.normals.emplace_back(0, 1, 0);
            }

            // should not be a vector 2d?
            const aiVector3D& texcoord =
                mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : aiVector3D(0, 0, 0);
        output.tex_coords.emplace_back(texcoord.x, texcoord.y);
        }
        for(int i = 0;  i < mesh->mNumFaces; ++i){
            const aiFace& face = mesh->mFaces[i];
            output.indices.push_back(face.mIndices[0]);
            output.indices.push_back(face.mIndices[1]);
            output.indices.push_back(face.mIndices[2]);
        }
    }



    void init_mesh_with_tangents(const aiMesh* mesh, Mesh& output){
        for(int i = 0; i < mesh->mNumVertices; ++i){
            const aiVector3D& pos = mesh->mVertices[i];
            output.positions.emplace_back(pos.x, pos.y, pos.z);
            
            // dont remember what these tangents are and are for
            // for normals
            if(mesh->mTangents){
                const aiVector3D& tangent = mesh->mTangents[i];
                output.tangents.emplace_back(tangent.x, tangent.y, tangent.z);
            }
            if(mesh->mNormals){ //??
                const aiVector3D& normal = mesh->mNormals[i];
                output.normals.emplace_back(normal.x, normal.y, normal.z);
            }
            else{
                output.normals.emplace_back(0, 1, 0);
            }

            // cross tangent and normal for bitangent

            // should not be a vector 2d?
            const aiVector3D& texcoord =
                mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : aiVector3D(0, 0, 0);
            output.tex_coords.emplace_back(texcoord.x, texcoord.y);
        }

        for(int i = 0;  i < mesh->mNumFaces; ++i){
            const aiFace& face = mesh->mFaces[i];
            output.indices.push_back(face.mIndices[0]);
            output.indices.push_back(face.mIndices[1]);
            output.indices.push_back(face.mIndices[2]);
        }
    };

    void initialize_mesh(const aiScene* scene, const std::string& file_loc_name, Mesh& mesh, MeshFlags& out_flags, MeshAnimationData* animation_data){
        MeshFlags flag;
        for(int i = 0; i < scene->mNumMeshes; ++i){
            mesh.meshes.push_back(BasicMeshEntry());
        }
        mesh.materials.resize(scene->mNumMaterials);

        auto count = parse_meshes_and_return_total_count(scene, mesh);


        unsigned int no_vertices    = count.no_vertices;
        unsigned int no_indices     = count.no_indices;
        unsigned int no_bones       = count.no_bones;


        reserve_space(no_vertices, no_indices, mesh);

        for(int i = 0; i < no_vertices; ++i){
            mesh.bones.push_back(BoneData());
        }

        for(std::size_t i = 0; i < scene->mNumMeshes; ++i){
            const aiMesh* _mesh = scene->mMeshes[i];
            init_mesh_with_tangents(_mesh, mesh);
            if(_mesh->HasBones())
                flag[MESHFLAG::BONES] = 1;

            for(int j = 0; j < _mesh->mNumBones; ++j)
                parse_bone_for_mesh(_mesh->mBones[j], mesh.meshes[i], *animation_data, mesh);
        }

        for(unsigned int i = 0; i < scene->mNumMaterials; ++i){
            const aiMaterial* material = scene->mMaterials[i];

            auto directory = get_directory(file_loc_name);
            bool flag_diffuse = load_diffuse_texture(directory, material, mesh.materials[i], scene);
            bool flag_normal_map = load_normal_map_texture(directory, material, mesh.materials[i]);

            if(flag_normal_map)
                flag[MESHFLAG::NORMALMAP] = true;
            if(flag_diffuse)
                flag[MESHFLAG::DIFFUSE] = true;
        }

        populate_buffers(mesh);

        animation_data->inv = glm::inverse(aiMatrix4x4ToGlm(&scene->mRootNode->mTransformation));

        out_flags = flag;
    }

    std::tuple<bool, LoadMeshData> load_mesh_from_disk(const std::string& file_loc_name, unsigned int load_flags){
        LoadMeshData load_data;
        auto& animation_location = load_data.animation_location;
        animation_location = AnimationMeshStorage::get().reserve_mesh();
        if(animation_location() == -1)
            return std::make_tuple(0, load_data);

        auto& flags = load_data.flags;
        load_data.mesh = MeshStorage::get().get_mesh(file_loc_name);
        assert(load_data.mesh != nullptr);
        auto* animation_mesh = AnimationMeshStorage::get().get_mesh(animation_location());
        auto& importer = animation_mesh->importer;
        animation_mesh->scene = importer.ReadFile(file_loc_name.c_str(), load_flags);
        auto* scene = animation_mesh->scene;

        if(scene){
            initialize_mesh(scene, file_loc_name, *load_data.mesh, flags, animation_mesh);

            if(flags[MESHFLAG::BONES]){
                load_data.no_animations = scene->mNumAnimations;
            }
            else{
                AnimationMeshStorage::get().free_mesh(animation_location);
                animation_location() = -1;
            }

        }
        else{
            AnimationMeshStorage::get().free_mesh(animation_location);
            animation_location() = -1;

            return std::make_tuple(0, load_data);
        }


        return std::make_tuple(1, load_data);

    }

    LoadMeshData load_search_mesh(const std::string& file_loc_name, unsigned int load_flags){

        auto& storage = MeshLoadDataStorage::get();
        if(storage.map.find(file_loc_name) != storage.map.end()){
            return storage.map[file_loc_name];
        }

        auto out = load_mesh_from_disk(file_loc_name,/* mesh, load_data.flags,*/ load_flags/*, load_data.animation_location*/);

        storage.map[file_loc_name] = std::get<1>(out);

        return storage.map[file_loc_name];
    }

    LoadMeshData load_mesh_with_meta_data(const std::string& file_loc_name){
        unsigned int ASSIMP_LOAD_FLAGS = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
        return load_search_mesh(file_loc_name, ASSIMP_LOAD_FLAGS);
    }

    std::tuple<MeshRenderId, MeshFlags> load_mesh(const std::string& file_loc_name){
        auto data = load_mesh_with_meta_data(file_loc_name);
        auto& mesh = *data.mesh;
        return std::make_tuple(create_mesh_render_id(*data.mesh), data.flags);
    }


    void load_create_triangle_mesh_helper(const MeshLoaderNew::Mesh& mesh, btTriangleMesh& target){
        assert(mesh.indices.size() % 3 == 0);
        for(int i = 0; i < mesh.indices.size(); i += 3){
            auto& v1 = mesh.positions[mesh.indices[i]];
            auto& v2 = mesh.positions[mesh.indices[i + 1]];
            auto& v3 = mesh.positions[mesh.indices[i + 2]];
            target.addTriangle(btVector3(v1.x, v1.y, v1.z), btVector3(v2.x, v2.y, v2.z), btVector3(v3.x, v3.y, v3.z));
        }
    }


    void load_create_triangle_mesh(const std::string& file_loc_name, btTriangleMesh& target){
        unsigned int ASSIMP_LOAD_FLAGS = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
        auto& mesh = *load_search_mesh(file_loc_name, ASSIMP_LOAD_FLAGS).mesh;
        load_create_triangle_mesh_helper(mesh, target);
    }

    btConvexHullShape* load_create_convex_mesh(const std::string& file_loc_name){
        unsigned int ASSIMP_LOAD_FLAGS = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
        auto& mesh = *load_search_mesh(file_loc_name, ASSIMP_LOAD_FLAGS).mesh;
        return new btConvexHullShape((btScalar*)&(mesh.positions[0]), mesh.positions.size(), sizeof(glm::vec3));
    }

    // BONES
    void parse_node(const aiNode* node){
        for(int i = 0; i < node->mNumChildren; ++i){
            parse_node(node->mChildren[i]);
        }
    }


    int get_bone_id(std::unordered_map<std::string, int>& map, const aiBone* bone){
        int id = 0;
        std::string name(bone->mName.C_Str());

        if(map.find(name) == map.end()){
            id = map.size();
            map[name] = id;
        }
        else{
            id = map[name];
        }
        return id;
    }

    // the bones 
    void parse_bone_for_mesh(const aiBone* bone, const BasicMeshEntry& mesh_entry, MeshAnimationData& anim, Mesh& mesh){
        BoneData out;
        std::string bone_name = bone->mName.C_Str();
        auto& map = anim.bone_name_to_index;
        int bone_id = get_bone_id(map, bone);

        if(bone_id == anim.transforms.size()){
            anim.offsets.emplace_back(aiMatrix4x4ToGlm(&bone->mOffsetMatrix));
            anim.transforms.emplace_back(glm::mat4(1.f));
        }
        for(int i = 0; i < bone->mNumWeights; ++i){
            const aiVertexWeight& w = bone->mWeights[i];
            unsigned int id         = bone->mWeights[i].mVertexId + mesh_entry.base_vertex;

            mesh.bones[id].add(bone_id, w.mWeight);
        }
    }

    void calc_interp_position(float animation_ticks, const aiNodeAnim* anim, aiVector3D& out){
        if(anim->mNumPositionKeys == 1){
            out = anim->mPositionKeys[0].mValue;
            return;
        }
        unsigned int pidx = find_position(animation_ticks, anim);
        unsigned int npidx = pidx + 1;
        assert(npidx < anim->mNumPositionKeys);
        float t1 = (float)anim->mPositionKeys[pidx].mTime;
        float t2 = (float)anim->mPositionKeys[npidx].mTime;
        float deltatime = t2 - t1;
        float factor = (animation_ticks - t1)/deltatime;
        assert(factor >= 0.0f && factor <= 1.0f);
        const aiVector3D& start = anim->mPositionKeys[pidx].mValue;
        const aiVector3D& end = anim->mPositionKeys[npidx].mValue;
        aiVector3D delta = end - start;
        out = start + factor * delta;
    }



    void calc_interp_rotation(float animation_ticks, const aiNodeAnim* anim, aiQuaternion& out){
        if(anim->mNumRotationKeys == 1){
            out = anim->mRotationKeys[0].mValue;
            return;
        }
        unsigned int ridx = find_rotation(animation_ticks, anim);
        unsigned int nridx = ridx + 1;
        float t1  = (float)anim->mRotationKeys[ridx].mTime;
        float t2  = (float)anim->mRotationKeys[nridx].mTime;
        float deltatime = t2 - t1;
        float factor = (animation_ticks - t1)/deltatime;
        aiQuaternion& start =   anim->mRotationKeys[ridx].mValue;
        aiQuaternion& end =     anim->mRotationKeys[nridx].mValue;
        aiQuaternion::Interpolate(out, start, end, factor);
        out.Normalize();
    }

    void calc_interp_scale(float animation_ticks, const aiNodeAnim* anim, aiVector3D& out){
        if(anim->mNumScalingKeys == 1){
            out = anim->mScalingKeys[0].mValue;
            return;
        }
        unsigned int sidx = find_scaling(animation_ticks, anim);
        unsigned int nsidx = sidx + 1;
        float t1 = (float)anim->mScalingKeys[sidx].mTime;
        float t2 = (float)anim->mScalingKeys[nsidx].mTime;
        float deltatime = t2 - t1;
        float factor = (animation_ticks - t1) / deltatime;
        const aiVector3D& start =   anim->mScalingKeys[sidx].mValue;
        const aiVector3D& end =     anim->mScalingKeys[nsidx].mValue;
        aiVector3D delta = end - start;
        out = start + factor * delta;

    }

    const aiNodeAnim* find_node_anim(const aiAnimation* animation, const std::string& node_name){
        for(int i = 0; i < animation->mNumChannels; ++i){
            const aiNodeAnim* node_anim = animation->mChannels[i];
            if(std::string(node_anim->mNodeName.data) == node_name){
                return node_anim;
            }
        }
        return nullptr;
    }

    void read_node_hierarcy(float ticks, const aiNode* node, const aiScene* scene, const glm::mat4& parent_transform, MeshAnimationData& anim, int playing_animation){
        std::string name = node->mName.data;
        glm::mat4 node_transformation(aiMatrix4x4ToGlm(&node->mTransformation));

        // the first animation
        const aiAnimation* animation = scene->mAnimations[playing_animation];
        const aiNodeAnim* node_anim = find_node_anim(animation, name);

        if(node_anim){
            aiVector3D scale;
            calc_interp_scale(ticks, node_anim, scale);
            glm::mat4 mat_scale(1.f);
            glm::scale(mat_scale, glm::vec3(scale.x, scale.y, scale.z));

            aiQuaternion rotation;
            calc_interp_rotation(ticks, node_anim, rotation);
            auto matrix = rotation.GetMatrix();
            glm::mat4 mat_rotation(aiMatrix3x3ToGlm(&matrix));
            mat_rotation[0][3] = 0;
            mat_rotation[1][3] = 0;
            mat_rotation[2][3] = 0;
            mat_rotation[3][3] = 1.f;

            mat_rotation[3][0] = 0;
            mat_rotation[3][1] = 0;
            mat_rotation[3][2] = 0;

            aiVector3D translation;
            calc_interp_position(ticks, node_anim, translation);
            glm::mat4 mat_translation(1.f);
            mat_translation = glm::translate(mat_translation, glm::vec3(translation.x, translation.y, translation.z));

            node_transformation = mat_translation * mat_rotation * mat_scale;
        }

        glm::mat4 global_transformation = parent_transform * node_transformation;

        if(anim.bone_name_to_index.find(name) != anim.bone_name_to_index.end()){
            int bone_index = anim.bone_name_to_index[name];
            auto root = scene->mRootNode->mTransformation;
            anim.transforms[bone_index] = anim.inv * global_transformation * anim.offsets[bone_index];
        }
        for(int i = 0; i < node->mNumChildren; ++i){
            read_node_hierarcy(ticks, node->mChildren[i], scene, global_transformation, anim, playing_animation);
        }
    }


    MeshLoaderNew::MeshAnimationData* get_bone_transform(float time_in_seconds, Component::AnimationController anim_controller){
        auto* anim = AnimationMeshStorage::get().get_mesh(anim_controller.animation_location);

        if(anim == nullptr || anim_controller.playing_animation == -1 || anim_controller.no_animations < 1)
            return anim;

        const aiScene* scene = anim->scene;
        float ticks_per_seconds = (float)(scene->mAnimations[anim_controller.playing_animation]->mTicksPerSecond != 0 ? scene->mAnimations[anim_controller.playing_animation]->mTicksPerSecond : 25.f);
        float time_in_ticks = time_in_seconds * ticks_per_seconds;
        float animation_time_ticks = fmod(time_in_ticks, (float)scene->mAnimations[anim_controller.playing_animation]->mDuration);

        glm::mat4 identity(1.f);
        read_node_hierarcy(animation_time_ticks, scene->mRootNode, scene, identity, *anim, anim_controller.playing_animation);
        return anim;
    }

}


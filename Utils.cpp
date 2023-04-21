#include "Utils.h"
#include "TextureFunctions.h"
#include "Shaders.h"
#include "EditorRegistry.h"
#include "NewGui.h"

namespace Utils{
    void copy_over_fix(entt::registry& registry){
        static_cast<void>(registry.storage<Tags::TriangleEnemy>());
        static_cast<void>(registry.storage<Tags::Ally>());
        static_cast<void>(registry.storage<Tags::TriangleProjectile >());
        static_cast<void>(registry.storage<Tags::Player >());
        static_cast<void>(registry.storage<Tags::Triangle >());
        static_cast<void>(registry.storage<Tags::Bullet >());
        static_cast<void>(registry.storage<Tags::ManualRender >());
        static_cast<void>(registry.storage<Tags::Model3D>());
        static_cast<void>(registry.storage<Tags::TexModel3D>());
        static_cast<void>(registry.storage<Tags::Sphere>());
        static_cast<void>(registry.storage<Tags::Physics>());
        static_cast<void>(registry.storage<Tags::BulletPhysics>());
        static_cast<void>(registry.storage<Tags::RepresentationRender>());
        static_cast<void>(registry.storage<Tags::RepresentationRenderNewMeshLoader>());
        static_cast<void>(registry.storage<Component::AnimationController>());

        static_cast<void>(registry.storage<RenderTags::BonesRendering>());
        static_cast<void>(registry.storage<RenderTags::_3D_OBJECT>());
        static_cast<void>(registry.storage<RenderTags::_3D_TEXTURE>());
        static_cast<void>(registry.storage<RenderTags::_2D_COLOR>());
        static_cast<void>(registry.storage<RenderTags::_2D_DEFAULT>());

        static_cast<void>(registry.storage<MeshLoaderNew::MeshRenderId>());
        static_cast<void>(registry.storage<Component::Scale>());
        static_cast<void>(registry.storage<Component::Position>());
        static_cast<void>(registry.storage<Component::Rotation>());
        static_cast<void>(registry.storage<Component::Render>());
        static_cast<void>(registry.storage<Component::Color>());
        static_cast<void>(registry.storage<Component::_3DObjectData>());
        static_cast<void>(registry.storage<Component::SphereCollider>());
        static_cast<void>(registry.storage<Component::SelectedShader>());
        static_cast<void>(registry.storage<Component::Mesh>());
        static_cast<void>(registry.storage<Component::MeshPointers>());
        static_cast<void>(registry.storage<Component::Force>());
        static_cast<void>(registry.storage<Component::LifeTime>());
        static_cast<void>(registry.storage<Component::Texture>());
        static_cast<void>(registry.storage<Component::Render3D>());
        static_cast<void>(registry.storage<Component::ApplyForce>());
        static_cast<void>(registry.storage<Component::ControllVelocity>());
        static_cast<void>(registry.storage<Component::Velocity>());
        static_cast<void>(registry.storage<Component::Name>());
        static_cast<void>(registry.storage<Component::OpenGLRotation>());
    }
	void copy_over_to_other_registry(entt::registry& to_registry, entt::registry& from_registry, entt::entity to, entt::entity from){
		for(auto [id, storage]: from_registry.storage()) {
			if(auto *other = to_registry.storage(id); other && storage.contains(from)) {
				other->emplace(to, storage.get(from));
			}
		}
	}


}

namespace Rectangle {
	float vertices[] = {
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f, 
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f	
	};
	unsigned int indices[] = {
		0, 1, 3, 
		1, 2, 3 
	};
}

namespace RectangleRightTop {
	float vertices[] = {
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f, 
		 1.0f,  0.0f, 0.0f,  1.0f, 0.0f,
		 0.0f,  0.0f, 0.0f,  0.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,  0.0f, 1.0f	
	};
	unsigned int indices[] = {
		0, 1, 3, 
		1, 2, 3 
	};
}

namespace Background {
	float vertices[] = {
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f	
	};
	unsigned int indices[] = {
		0, 1, 3, 
		1, 2, 3 
	};
}

namespace triangle {
	float vertices[] = {
	-0.5f, -0.5f, 0.0f, 
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
}

// may need the same glfw include or glad
namespace CreateRender {
	void create_render_triangle(Component::Render& render) {
		glGenVertexArrays(1, &render.VAO);
		glGenBuffers(1, &render.VBO);
		glBindVertexArray(render.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, render.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle::vertices), triangle::vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void create_render(Component::Render& render, Component::Mesh& mesh) {
		glGenVertexArrays(1, &render.VAO);
		glGenBuffers(1, &render.VBO);
		glGenBuffers(1, &render.EBO);

		glBindVertexArray(render.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, render.VBO);

		glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.vertices.size() * sizeof(GLfloat), (GLsizeiptr*)&mesh.vertices[0]);
		glBufferSubData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(GLfloat), mesh.uv.size(), &mesh.uv[0]);
		glBufferSubData(GL_ARRAY_BUFFER, (mesh.vertices.size() * sizeof(GLfloat)) + (mesh.uv.size() * sizeof(GLfloat)), (mesh.normals.size() * sizeof(GLfloat)), &mesh.normals[0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo.size() * sizeof(GLfloat), &mesh.ebo[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(mesh.vertices.size()* sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)((mesh.vertices.size() * sizeof(GLfloat)) + (mesh.uv.size()*sizeof(GLint))));
		glEnableVertexAttribArray(2);
	}
}

namespace Functions{
	void print(Component::Velocity vel){
		std::cout << vel.x << " " << vel.y << " " << vel.z << "\n";
	}


	void print(Component::Position pos){
		std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
	}

	void for_each_enemies(entt::registry& registry, std::function<void(entt::registry&, entt::entity)> func){
		auto group = registry.group<Component::Position>(entt::get<Component::Velocity>);
		for(auto& e : group){
			func(registry, e);	
		}
	}

	

	void print_enemy_names(entt::registry& registry) {
		auto view = registry.view<Tags::TriangleEnemy, Component::Name>();
		for (auto e : view) {
			const Component::Name& name = registry.get<Component::Name>(e);
			std::cout << "EnemyName: " << name.str << "\n";
		}
	}
	void print_ally_names(entt::registry& registry) {
		auto view = registry.view<Tags::Ally, Component::Name>();
		for (auto e : view) {
			const Component::Name& name = registry.get<Component::Name>(e);
			std::cout << "AllyName: " << name.str << "\n";
		}
	}

	
	void spawn_allies(entt::registry& registry, unsigned int count) {
		for (unsigned int i = 0; i < count; ++i) {
			entt::entity e = registry.create();
			emplace<Tags::Ally, Component::Position, Component::Velocity, Component::Name>(registry, e);
			auto& name = registry.get<Component::Name>(e);
			name.str = "ally";
		}
	}

	void spawn_enemies(entt::registry& registry, unsigned int count /*some sort of context*/){
		for(unsigned int i = 0; i < count; ++i){
			entt::entity e = registry.create();
			// should render be passed with mesh or not?
			emplace<Tags::TriangleEnemy, Component::Position, Component::Velocity, Component::Name, Component::Mesh>(registry, e);
			auto& name = registry.get<Component::Name>(e);
			name.str = "enemy";
		}
	}
}

namespace Debug{
	void create_render_rectangle_top_right(Component::Render& render) {

		glGenVertexArrays(1, &render.VAO);
		glGenBuffers(1, &render.VBO);
		glGenBuffers(1, &render.EBO);

		glBindVertexArray(render.VAO);

		glBindBuffer(GL_ARRAY_BUFFER, render.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(RectangleRightTop::vertices), RectangleRightTop::vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RectangleRightTop::indices), RectangleRightTop::indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}


}

#pragma region Background
namespace Background {
	void create_render_background(Component::Render& render) {

		glGenVertexArrays(1, &render.VAO);
		glGenBuffers(1, &render.VBO);
		glGenBuffers(1, &render.EBO);

		glBindVertexArray(render.VAO);

		glBindBuffer(GL_ARRAY_BUFFER, render.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Background::vertices), Background::vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Background::indices), Background::indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

	}

	entt::entity entity;

	void init(entt::registry& registry) {
		entity = registry.create();
		Functions::emplace<Component::Texture, Component::Render, Tags::ManualRender>(registry, entity);
		auto [texture, render] = registry.get<Component::Texture, Component::Render>(entity);
		std::string str = "/home/solidus/Assets/31367.jpg";
		create_render_background(render);
		TextureFunctions::load_texture_jpg(str, texture);
	}

	unsigned int create_shader() {
		unsigned int shader_program;
		std::string shader_folder = "/home/solidus/Assets/Shader/";
		std::string fragment_shader_source = Shaders::background_fragment_shader_1;
		std::string vertex_shader_source = Shaders::background_vertex_shader_1;
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		const char* frag_char = fragment_shader_source.c_str();
		const char* vert_char = vertex_shader_source.c_str();
		glShaderSource(fragment_shader, 1, &frag_char, NULL);
		glShaderSource(vertex_shader, 1, &vert_char, NULL);
		glCompileShader(fragment_shader);
		glCompileShader(vertex_shader);
		shader_program = glCreateProgram();
		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);
		glLinkProgram(shader_program);
		int success;
		char infoLog[512];
		glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		return shader_program;
	}

	void draw_background(entt::registry& registry, unsigned int shader_program) {
		auto [texture, render] = registry.get<Component::Texture, Component::Render>(entity);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glUseProgram(shader_program);
		glBindVertexArray(render.VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
#pragma endregion



void create_triangle(entt::registry& registry, Component::Render& render) {
	CreateRender::create_render_triangle(render);
}
void create_triangle(entt::registry& registry) {
	auto e = registry.create();
	Functions::emplace<Component::Render, Component::Position, Tags::Triangle, Component::Velocity>(registry, e);
	auto& render = registry.get<Component::Render>(e);
	auto& pos = registry.get<Component::Position>(e);
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	CreateRender::create_render_triangle(render);
}

namespace SHADERS{
unsigned int load_shader(const char* shader_loc_name, int type){
	unsigned int shader = glCreateShader(type);
	std::ifstream file;
	file.open(shader_loc_name);
	std::string line;
	std::string shader_string;
	while(getline(file, line)){
		shader_string.append(line);
		shader_string.append("\n");
	}
	file.close();

	const char* shader_str = shader_string.c_str();
	glShaderSource(shader, 1, &shader_str, NULL);
	glCompileShader(shader);
	return shader;
}
void FlashLightShaderNormalMap::uniform(glm::mat4 view, glm::mat4 proj,
	glm::vec3 position, glm::vec3 direction){
	glUniformMatrix4fv(u_view,  1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(u_proj,  1, GL_FALSE, glm::value_ptr(proj));
	glUniform3fv(u_position,    1, glm::value_ptr(position));
	glUniform3fv(u_direction,   1, glm::value_ptr(direction));
}
void FlashLightShaderNormalMap::bind(){
	glUseProgram(shader_program);
	//glUniform1f(u_cutoff, glm::cos(glm::radians(0.3f)));
	glUniform1f(u_cutoff, 10.f);

}


void FlashLightShaderNew::uniform(glm::mat4 view, glm::mat4 proj,
	glm::vec3 position, glm::vec3 direction){
	glUniformMatrix4fv(u_view,  1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(u_proj,  1, GL_FALSE, glm::value_ptr(proj));
	glUniform3fv(u_position,    1, glm::value_ptr(position));
	glUniform3fv(u_direction,   1, glm::value_ptr(direction));
}

void FlashLightShaderNew::uniform(glm::mat4 view, glm::mat4 proj,
	glm::vec3 position, glm::vec3 direction, glm::mat4 light_projection){
	glUniformMatrix4fv(u_view,  1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(u_proj,  1, GL_FALSE, glm::value_ptr(proj));
	// TODO: want this set to the character position and not the camera position
	// light.position
	glUniform3fv(u_position,    1, glm::value_ptr(position));
	// light.direction
	glUniform3fv(u_direction,   1, glm::value_ptr(direction));
	glUniformMatrix4fv(u_light_projection,  1, GL_FALSE, glm::value_ptr(light_projection));

}


void BonesShader::bind()const{
    glUseProgram(shader_program);
}
BonesShader::BonesShader(){
    shader_program = link_shaders(
            load_shader((std::string(shader_folder) + "bones_shader.fs").c_str(), GL_FRAGMENT_SHADER),
            load_shader((std::string(shader_folder) + "bones_shader.vs").c_str(), GL_VERTEX_SHADER)
    );
    u_view = glGetUniformLocation(shader_program, "view");
    u_proj = glGetUniformLocation(shader_program, "projection");
    u_model = glGetUniformLocation(shader_program, "model");
    for(int i = 0; i < MAX_BONES; ++i){
        std::string name = "bones[" + std::to_string(i) +  "]";
        std::cout << name << '\n';
        u_bones[i] = glGetUniformLocation(shader_program, name.c_str());
    }
}

void BonesShader::bones_uniform(const glm::mat4& bone_transform, unsigned int location)const{
    glUniformMatrix4fv(u_bones[location], 1, GL_FALSE, glm::value_ptr(bone_transform));
}
void BonesShader::uniform(const glm::mat4& view, const glm::mat4& proj)const{
    glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(u_proj, 1, GL_FALSE, glm::value_ptr(proj));
}

void FlashLightShaderNew::bind(){
	glUseProgram(shader_program);
	//glUniform1f(u_cutoff, glm::cos(glm::radians(0.3f)));
	glUniform1f(u_cutoff, 10.f);

}

FlashLightShaderNormalMap::FlashLightShaderNormalMap(){
	shader_program = link_shaders(
		load_shader((std::string(shader_folder) + "flashlight_normal.fs").c_str(), GL_FRAGMENT_SHADER),
		load_shader((std::string(shader_folder) + "flashlight_normal.vs").c_str(), GL_VERTEX_SHADER)
	);

	u_cutoff = glGetUniformLocation(shader_program,   "light.cutoff");
	u_position  = glGetUniformLocation(shader_program, "light.position");
	u_direction = glGetUniformLocation(shader_program, "light.direction");
	u_fov = glGetUniformLocation(shader_program, "light.fov");
	u_col = glGetUniformLocation(shader_program, "light.col");
	u_len = glGetUniformLocation(shader_program, "light.len");

	u_view = glGetUniformLocation(shader_program, "view");
	u_proj = glGetUniformLocation(shader_program, "projection");
	u_model = glGetUniformLocation(shader_program, "model");
	u_light_projection = glGetUniformLocation(shader_program, "light_projection");

}

FlashLightShaderNew::FlashLightShaderNew(){
	shader_program = link_shaders(
		load_shader((std::string(shader_folder) + "flashlight_new.fs").c_str(), GL_FRAGMENT_SHADER),
		load_shader((std::string(shader_folder) + "flashlight_new.vs").c_str(), GL_VERTEX_SHADER)
	);

	u_cutoff = glGetUniformLocation(shader_program,   "light.cutoff");
	u_position  = glGetUniformLocation(shader_program, "light.position");
	u_direction = glGetUniformLocation(shader_program, "light.direction");

	u_view = glGetUniformLocation(shader_program, "view");
	u_proj = glGetUniformLocation(shader_program, "projection");
	u_model = glGetUniformLocation(shader_program, "model");
	u_light_projection = glGetUniformLocation(shader_program, "light_projection");


}
void FlashLightShaderNormalMap::uniform(glm::mat4 view, glm::mat4 proj, glm::vec3 position, glm::vec3 direction, float fov, float len, glm::vec3 col){
	glUniformMatrix4fv(u_view,  1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(u_proj,  1, GL_FALSE, glm::value_ptr(proj));
	glUniform3fv(u_position,    1, glm::value_ptr(position));
	glUniform3fv(u_direction,   1, glm::value_ptr(direction));
	glUniform3fv(u_col,   1, glm::value_ptr(col));
	glUniform1f(u_fov, fov);
	glUniform1f(u_len, len);

}
void FlashLightShaderNormalMap::uniform(glm::mat4 view, glm::mat4 proj, glm::vec3 position, glm::vec3 direction, float fov, float len, glm::vec3 col, glm::mat4 light_projection){
	glUniformMatrix4fv(u_view,  1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(u_proj,  1, GL_FALSE, glm::value_ptr(proj));
	glUniform3fv(u_position,    1, glm::value_ptr(position));
	glUniform3fv(u_direction,   1, glm::value_ptr(direction));
	glUniform3fv(u_col,   1, glm::value_ptr(col));
	glUniform1f(u_fov, fov);
	glUniform1f(u_len, len);
	glUniformMatrix4fv(u_light_projection,  1, GL_FALSE, glm::value_ptr(light_projection));
}

void FlashLightShader::uniform(glm::mat4 view, glm::mat4 proj,
				glm::vec3 position, glm::vec3 direction){
	glUniformMatrix4fv(u_view,  1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(u_proj,  1, GL_FALSE, glm::value_ptr(proj));
	glUniform3fv(u_position,    1, glm::value_ptr(position));
	glUniform3fv(u_direction,   1, glm::value_ptr(direction));
}
void FlashLightShader::bind(){
	glUseProgram(shader_program);
	glUniform1f(u_constant, 1.0f);
	glUniform1f(u_linear, 0.09f);
	glUniform1f(u_quadratic, 0.032f);
	glUniform1f(u_cutoff, glm::cos(glm::radians(12.5f)));
	glUniform1f(u_shininess, 32.0f);

}

FlashLightShader::FlashLightShader(){
	shader_program = link_shaders(
		load_shader((std::string(shader_folder) + "flashlight.fs").c_str(), GL_FRAGMENT_SHADER),
		load_shader((std::string(shader_folder) + "flashlight.vs").c_str(), GL_VERTEX_SHADER)
	);

	u_constant = glGetUniformLocation(shader_program,   "light.constant");
	u_linear = glGetUniformLocation(shader_program,     "light.linear");
	u_quadratic = glGetUniformLocation(shader_program,  "light.quadratic");
	u_cutoff = glGetUniformLocation(shader_program,   "light.cutoff");
	u_shininess = glGetUniformLocation(shader_program,   "light.shininess");

	u_view = glGetUniformLocation(shader_program, "view");
	u_proj = glGetUniformLocation(shader_program, "projection");
	u_model = glGetUniformLocation(shader_program, "model");
	u_position  = glGetUniformLocation(shader_program, "light.position");
	u_direction = glGetUniformLocation(shader_program, "light.direction");

}

	void create_render_fbo_render(Component::Render& render) {
		glGenVertexArrays(1, &render.VAO);
		glGenBuffers(1, &render.VBO);
		glGenBuffers(1, &render.EBO);

		glBindVertexArray(render.VAO);

		glBindBuffer(GL_ARRAY_BUFFER, render.VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Rectangle::vertices), Rectangle::vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Rectangle::indices), Rectangle::indices, GL_STATIC_DRAW);

        render.triangle_count = sizeof(Rectangle::indices)/sizeof(Rectangle::indices[0]) / 3;

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}






void BulletShader::bind(){
	glUseProgram(shader_program);
}

void BulletShader::uniform(glm::mat4 view, glm::mat4 proj){
	glUniformMatrix4fv(u_view,  1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(u_proj,  1, GL_FALSE, glm::value_ptr(proj));
}

BulletShader::BulletShader(){
	shader_program = link_shaders(
		load_shader((std::string(shader_folder) + "bullet_light.fs").c_str(), GL_FRAGMENT_SHADER),
		load_shader((std::string(shader_folder) + "bullet_light.vs").c_str(), GL_VERTEX_SHADER)
	);

	u_light_pos = glGetUniformLocation(shader_program, "diffuse_pos");
	u_light_dir = glGetUniformLocation(shader_program, "diffuse_dir");
	u_light_col = glGetUniformLocation(shader_program, "diffuse_col");

	u_view = glGetUniformLocation(shader_program, "view");
	u_proj = glGetUniformLocation(shader_program, "proj");
	u_model = glGetUniformLocation(shader_program, "model");
}

void SkyBoxShader::uniform(glm::mat4 view, glm::mat4 proj){
	glUniformMatrix4fv(u_view,  1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(u_proj,  1, GL_FALSE, glm::value_ptr(proj));
}
void SkyBoxShader::bind(){

	glDepthFunc(GL_LEQUAL);
	glUseProgram(shader_program);
}

SkyBoxShader::SkyBoxShader(){
	shader_program = link_shaders(
		load_shader((std::string(shader_folder) + "skybox.fs").c_str(), GL_FRAGMENT_SHADER),
		load_shader((std::string(shader_folder) + "skybox.vs").c_str(), GL_VERTEX_SHADER)
	);
	u_view = glGetUniformLocation(shader_program, "view");
	u_proj = glGetUniformLocation(shader_program, "proj");
}


}


namespace Shader{
    
    namespace{
        const std::string shadowmapvs{"/home/solidus/Assets/Shaders/shadowmap.vs"};
        const std::string shadowmapfs{"/home/solidus/Assets/Shaders/shadowmap.fs"};
    }

    ShadowmapShader::ShadowmapShader():Shader(shadowmapvs, shadowmapfs){
        u_light_matrix = glGetUniformLocation(shader_program, "lightMatrix");
        u_model = glGetUniformLocation(shader_program, "model");
    }

    namespace{
        const std::string debugvs{"/home/solidus/Assets/Shaders/shadowmap_imgui_render.vs"};
        const std::string debugfs{"/home/solidus/Assets/Shaders/shadowmap_imgui_render.fs"};
    }

    ShadowmapDebugShader::ShadowmapDebugShader():
        Shader(debugvs, debugfs)
     {
        u_depth_map = glGetUniformLocation(shader_program, "depthMap");
        u_near = glGetUniformLocation(shader_program, "near_plane");
        u_far = glGetUniformLocation(shader_program, "far_plane");

        // stupid
        SHADERS::create_render_fbo_render(render);
    }



}
namespace FBO{

    namespace{
        const std::string shadowmapvs{"/home/solidus/Assets/Shaders/shadowmap.vs"};
        const std::string shadowmapfs{"/home/solidus/Assets/Shaders/shadowmap.fs"};
    }

    ShadowmapFBO::ShadowmapFBO():
        m_width{1024}, m_height{1024}, m_shader()
    {
        glGenFramebuffers(1, &m_fbo);

        TextureFunctions::create_depth_texture(m_texture, m_width, m_height);
        glBindTexture(GL_TEXTURE_2D, m_texture.id);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture.id, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


	ShadowMapSingleton::ShadowMapSingleton(){
        auto& gui = GUI_WIDGETS::TextureGui::get();
        gui.add_render_texture("shadowmap_render", fbo.m_texture.id);
	}

    void ShadowMapSingleton::update(glm::mat4 light_projection){
		glUseProgram(shadowmap_shader.shader_program);

		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo.m_fbo);
		glClear(GL_DEPTH_BUFFER_BIT);


        entt::registry& bullet_registry = Registry::get().registry;
        entt::registry& editor_registry = EditorRegistry::get().m_registry;

		glUniformMatrix4fv(shadowmap_shader.u_light_matrix,  1, GL_FALSE, glm::value_ptr(light_projection));
		Renderer::representation_renderer_normalmap_bullet_fbo(&bullet_registry, shadowmap_shader.shader_program, shadowmap_shader.u_model);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }


    void create_render_fbo_texture(Component::Texture& texture){
        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);
  
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE,NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    }

}

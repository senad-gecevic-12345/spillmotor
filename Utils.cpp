#include "Utils.h"
#include "TextureFunctions.h"
#include "Shaders.h"

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
	std::cout << shader_string << "aoeu";

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

	u_view = glGetUniformLocation(shader_program, "view");
	u_proj = glGetUniformLocation(shader_program, "projection");
	u_model = glGetUniformLocation(shader_program, "model");


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

void BulletShader::bind(){
	glUseProgram(shader_program);
}
void BulletShader::uniform(glm::mat4 view, glm::mat4 proj){
	glUniformMatrix4fv(u_view,  1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(u_proj,  1, GL_FALSE, glm::value_ptr(proj));
}
BulletShader::BulletShader(){

	std::cout << (std::string(shader_folder)).c_str();
	std::cout << (std::string(shader_folder)).c_str();

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

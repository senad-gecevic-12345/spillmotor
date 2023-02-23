#include "Window.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "entt/entt.hpp"
#include "Components.h"
//#include "EditorRegistry.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include <chrono>
#include <array>
#include "TextureFunctions.h"
#include <cmath>
//#include "Tree.h"
#include "Shaders.h"
#include "Math.h"
#include <cstdlib>
#include "bullet.h"
#include "RenderFunctions.h"
#include "Tags.h"
//#include "gui_imgui.h"
#include "World.h"
#include "Utils.h"
#include "Systems.h"
#include "NewGui.h"
#include "Actor.h"

// TODO: think the swap interval is implemented wrong, and check that bullet updates 1 / 60
// seems like the update is correct, but the bullet update may not

namespace CreateRender {
	void create_render_triangle(Component::Render& render);
	void create_render(Component::Render& render, Component::Mesh& mesh);
}

void create_triangle(entt::registry& registry, Component::Render& render);
void create_triangle(entt::registry& registry);

namespace Shader {
	struct ShaderProgram {
		unsigned int shader_program;
		void fallback(){}
		ShaderProgram(){}
		ShaderProgram(std::string _vertex_shader_source, std::string _fragment_shader_source) {
			std::string fragment_shader_source = fragment_shader_source;
			std::string vertex_shader_source = vertex_shader_source;
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
			char log[512];
			glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader_program, 512, NULL, log);
				std::cout << "LINKING FAILED\n" << log << std::endl;
			}
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);
		}
	};
}

int create_shader_program_basic(const char* vertex_shader_source, const char* fragment_shader_source, unsigned int& shader_program) {
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	const char* frag_char = fragment_shader_source;
	const char* vert_char = vertex_shader_source;

	glShaderSource(fragment_shader, 1, &frag_char, NULL);
	glShaderSource(vertex_shader, 1, &vert_char, NULL);
	glCompileShader(fragment_shader);
	glCompileShader(vertex_shader);
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	int success;
	char error_log[512];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, error_log);
		std::cout << "ERROR: SHADER PROGRAM FAILED TO LINK: " << vertex_shader_source << error_log << std::endl;
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return success;
}



std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>create_shader_program_uniform_color() {
	unsigned int shader_program;  
	create_shader_program_basic(Shaders::vertex_shader_color_select, Shaders::fragment_shader_color_select, shader_program); 

	unsigned int proj_loc = glGetUniformLocation(shader_program, "proj");
	unsigned int view_loc = glGetUniformLocation(shader_program, "view");
	unsigned int model_loc = glGetUniformLocation(shader_program, "model");
    unsigned int color_loc = glGetUniformLocation(shader_program, "color");

	return std::make_tuple(shader_program, model_loc, view_loc, proj_loc, color_loc);
}

std::tuple<unsigned int, unsigned int, unsigned int, unsigned int>create_shader_program_default() {
	unsigned int shader_program;
	create_shader_program_basic(Shaders::vertex_shader_1, Shaders::fragment_shader_1_default, shader_program); 
	unsigned int proj_loc = glGetUniformLocation(shader_program, "proj");
	unsigned int view_loc = glGetUniformLocation(shader_program, "view");
	unsigned int model_loc = glGetUniformLocation(shader_program, "model");

	return std::make_tuple(shader_program, model_loc, view_loc, proj_loc);
}

std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>create_shader_program_color() {
	unsigned int shader_program;
	create_shader_program_basic(Shaders::vertex_shader_1, Shaders::fragment_shader_1_color, shader_program); 
	unsigned int proj_loc = glGetUniformLocation(shader_program, "proj");
	unsigned int view_loc = glGetUniformLocation(shader_program, "view");
	unsigned int model_loc = glGetUniformLocation(shader_program, "model");
	unsigned int color_loc = glGetUniformLocation(shader_program, "color");

	return std::make_tuple(shader_program, model_loc, view_loc, proj_loc, color_loc);
}

std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>create_shader_program_3d_bullet() {
	unsigned int shader_program;
	create_shader_program_basic(Shaders::vertex_shader_bullet_, Shaders::fragment_shader_bullet_, shader_program);

	unsigned int proj_loc = glGetUniformLocation(shader_program, "proj");
	unsigned int view_loc = glGetUniformLocation(shader_program, "view");
	unsigned int model_loc = glGetUniformLocation(shader_program, "model");

    unsigned int pos = glGetUniformLocation(shader_program, "diffuse_pos");
    unsigned int dir = glGetUniformLocation(shader_program, "diffuse_dir");
    unsigned int col = glGetUniformLocation(shader_program, "diffuse_col");

	return std::make_tuple(shader_program, model_loc, view_loc, proj_loc, pos, dir, col);
}
std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>create_shader_program_3d_selected() {
	unsigned int shader_program;
	create_shader_program_basic(Shaders::vertex_shader_selected, Shaders::fragment_shader_selected, shader_program);
	unsigned int proj_loc = glGetUniformLocation(shader_program, "proj");
	unsigned int view_loc = glGetUniformLocation(shader_program, "view");
	unsigned int model_loc = glGetUniformLocation(shader_program, "model");
	unsigned int hightlight = glGetUniformLocation(shader_program, "selected");

	return std::make_tuple(shader_program, model_loc, view_loc, proj_loc, hightlight);
}
std::tuple<unsigned int, unsigned int, unsigned int, unsigned int>create_shader_program_3d() {
	unsigned int shader_program;
	create_shader_program_basic(Shaders::vertex_shader_2, Shaders::fragment_shader_2,  shader_program);
	unsigned int proj_loc = glGetUniformLocation(shader_program, "proj");
	unsigned int view_loc = glGetUniformLocation(shader_program, "view");
	unsigned int model_loc = glGetUniformLocation(shader_program, "model");

	return std::make_tuple(shader_program, model_loc, view_loc, proj_loc);
}

namespace VelocityFunctions {
	float length(Component::Velocity velocity) {
		return sqrt(((velocity.x * velocity.x) + (velocity.y * velocity.y) + (velocity.z * velocity.z)));
	}

	void move(glm::vec3 direction, float speed_x, float speed_y, Component::ControllVelocity& target) {
		if (direction.x == 0 && direction.y == 0 && direction.z == 0)return;

		glm::vec3 normalize_vec = glm::normalize(direction);
		glm::vec3 x(1, 0, 0);
		glm::vec3 y(0, 1, 0);
		float dot_product_x = glm::dot(normalize_vec, x);
		float dot_product_y = glm::dot(normalize_vec, y);
		glm::vec3 result = glm::vec3(0.f, 0.f, 0.f);

		target.x = result.x;
		target.y = result.y;
		target.z = result.z;
	}

}

void gl_init(const unsigned int shader_program) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.1f, 0.1f, 0.9f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

namespace Render2D {
void draw_triangle_default(unsigned int model_loc, entt::registry& registry) {
	glEnable(GL_DEPTH_TEST);
	auto group = registry.group<>(entt::get<Tags::Triangle,Component::Position, Component::Render, Component::Velocity>, entt::exclude<Component::Color>);
	for (auto e : group) {
		auto [pos,vel] = registry.get<Component::Position, Component::Velocity>(e);
		auto& render = registry.get < Component::Render > (e);
			
		glm::mat4 model(1.f);
		model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(render.VAO);
		//glBindVertexArray(rend.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
void draw_triangle_color(unsigned int model_loc, unsigned int color_loc, entt::registry& registry) {
	glEnable(GL_DEPTH_TEST);
	auto group = registry.group<>(entt::get<Tags::Triangle,Component::Position, Component::Render, Component::Velocity, Component::Color>);
	for (auto e : group) {
		auto [pos,vel, color] = registry.get<Component::Position, Component::Velocity, Component::Color>(e);
		auto& render = registry.get < Component::Render > (e);
			
		glm::mat4 model(1.f);
		model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(color_loc, color.color.x, color.color.y, color.color.z);
		glBindVertexArray(render.VAO);
		//glBindVertexArray(rend.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
void draw_triangle_(unsigned int model_loc, entt::registry& registry) {
	glEnable(GL_DEPTH_TEST);
	auto group = registry.group<Tags::Triangle>(entt::get<Component::Position, Component::Render, Component::Velocity, Component::ApplyForce>);
	for (auto e : group) {
		auto [pos, rend, vel] = registry.get<Component::Position, Component::Render, Component::Velocity>(e);
		auto& render = registry.get < Component::Render > (e);
		glm::mat4 model(1.f);
		model = glm::translate(model, glm::vec3(pos.x, pos.y , pos.z ));
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(render.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
}

namespace RENDER3D {

	Shader::ShaderProgram render3d_shader;

void draw_graphics(entt::registry& registry) {
	glEnable(GL_DEPTH_TEST);
	auto group = registry.group<>(entt::get<Component::Render3D, Component::Position>);
	for (auto e : group) {
		auto& render = registry.get<Component::Render3D>(e);
		glBindVertexArray(render.VAO);
		glUseProgram(render3d_shader.shader_program);
	}
}
}


/*
void copy_over_test(){
	auto& from_registry = Registry::get().registry;
	auto& _registry = EditorRegistry::get();
	auto& to_registry = EditorRegistry::get().m_registry;
	auto from = from_registry.create();
	auto to = to_registry.create();

	from_registry.emplace<Component::Position>(from);

	std::cout << "(to)   REGISTRY COPY OVER HAS POSITION: " << to_registry.all_of<Component::Position>(to) << std::endl;
	std::cout << "(from) REGISTRY COPY OVER HAS POSITION: " << from_registry.all_of<Component::Position>(from) << std::endl;

	Utils::copy_over_to_other_registry(to_registry, from_registry, to, from);

	std::cout << "(to)   REGISTRY COPY OVER HAS POSITION: " << to_registry.all_of<Component::Position>(to) << std::endl;
	std::cout << "(from) REGISTRY COPY OVER HAS POSITION: " << from_registry.all_of<Component::Position>(from) << std::endl;
}
*/


int main() {

	Registry& registry = Registry::get();
	EditorRegistry& editor_registry = EditorRegistry::get();
	Utils::copy_over_fix(registry.registry);
	Utils::copy_over_fix(editor_registry.m_registry);

	int init = Window::init();
	if (init != 1) {
		Window::terminate();
		return -1;
	}

	auto [shader_program_bullet, model_loc_bullet, view_loc_bullet, proj_loc_bullet, bsl_u_pos, bsl_u_dir, bsl_u_col] = create_shader_program_3d_bullet();

	std::string skybox_dir = "/home/solidus/Assets/HDRI/";
	std::array<std::string, 6> skybox_files{
		std::string(skybox_dir + "skybox_2.jpg"),
		std::string(skybox_dir + "skybox_0.jpg"),
	std::string(	skybox_dir + "skybox_5.jpg"),
	std::string(	skybox_dir + "skybox_4.jpg"),
	std::string(	skybox_dir + "skybox_1.jpg"),
	std::string(	skybox_dir + "skybox_3.jpg")};

	auto skybox = SKYBOX::create_skybox(skybox_files);
	SHADERS::SkyBoxShader skybox_shader;

	SHADERS::BulletShader bullet_shader;
	SHADERS::FlashLightShader flashlight_shader;
	SHADERS::FlashLightShaderNew flashlight_shader_new;

	glm::mat4 glm_proj = glm::perspective(45.f, 1920.f / 1080.f, 0.1f, 500.f);

	gl_init(shader_program_bullet);

	glm::mat4 identity = glm::mat4(1.f);
	glm::mat4 view = glm::mat4(1.f);
	view = glm::translate(view, glm::vec3(0.f, 0.f, -100.f));
	int fps{0};

	// custom projection
	using namespace Math;
	//float aa = Window::get_aspect_ratio();
	//float fov = 45.f;

	const double fps_limit = 1.0 / 240.0;
	double last_update = 0;
	double last_frame = 0;
	double time_since_last_fps_update = 0;


	// model
	std::string unit_shapes_folder{ "/home/solidus/Assets/UnitShapes/" };
	std::string obj_file_loc_name{unit_shapes_folder + "sphere.obj"};
	std::string mtl_file_loc_name{unit_shapes_folder + "sphere.mtl"};
	std::string level_obj_file_loc_name =	"/home/solidus/Assets/platform.obj";
	std::string level_mtl_file_loc_name =	"/home/solidus/Assets/platform.mtl";

	//Model::TextureModel3D model(registry.registry, level_obj_file_loc_name, level_mtl_file_loc_name, glm::vec3(0, -10, 0));

	//Model::Model3D model(registry.registry, obj_file_loc_name, mtl_file_loc_name);

	unsigned int background_shader_program = Background::create_shader();
	Background::init(registry.registry);

	BulletScene& bullet = BulletScene::get();
	bullet.init();
    Controller::ControllerSingleton& character_controller = Controller::ControllerSingleton::get();

	//EnemySystem::spawn_enemies_2(registry.registry, 200);

/*
	copy_over_test();
	return 0;
*/

	auto* _window = Window::get_window();
    ControllerCameraFunctions::CameraPosTemp new_cam;
    NewGui& new_gui = NewGui::get();
    new_gui.init(_window);

	while (!Window::window_should_close()) {
		double now = glfwGetTime();
		PollKeyboard::get().update();
		glfwPollEvents();
		if ((now - last_frame) >= fps_limit) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(background_shader_program);

            // wrong
			World::Clock::get().update();
			float dt = World::Clock::get().get_dt();
			float mouse_x = Window::mouse_x;
			float mouse_y = Window::mouse_y;

            // BULLET UPDATE
            // check that dt is correct
            Bullet::step_and_copy_over(bullet, dt);

			LifetimeSystem::update(registry.registry);

			GUISystem::editor_copy_over();

            auto* wrld = BulletScene::get().world->getCollisionWorld();
			character_controller.update(registry.registry, wrld, dt, mouse_x, mouse_y);
            // what is this
			glm::vec3 cam_pos =  character_controller.camera.m_position;
			glm::mat4 glm_view = character_controller.camera.get_view();


			glm::vec4 front = glm::vec4(0, 0, -1, 0) * glm_view;
			flashlight_shader_new.bind();
			flashlight_shader_new.uniform(glm_view, glm_proj,
									  cam_pos, front);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glClearColor(0.1f, 0.1f, 0.9f, 1.0f);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glEnable(GL_BLEND);

			Renderer::representation_renderer_new_mesh_render(&registry.registry,
										   flashlight_shader_new.shader_program, flashlight_shader_new.u_model);

			Renderer::representation_renderer_new_mesh_render(&(editor_registry.m_registry),
										   flashlight_shader_new.shader_program, flashlight_shader_new.u_model);



            // SKYBOX
            auto camera_front = character_controller.camera.get_front();
            camera_front = glm::normalize(camera_front);
			glm::mat4 mat = glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,0) + camera_front, glm::vec3(0,1,0));
			skybox_shader.bind();
			skybox_shader.uniform(mat, glm_proj);
			skybox.draw();

            // GUI
            new_gui.new_frame();
            new_gui.update();
            new_gui.render();

			Window::swap_buffers();

			if (fps > 250) {
				auto time = now - time_since_last_fps_update;
				std::cout << "fps: " << (double)fps / time << "\n";
				fps = 0;
				time_since_last_fps_update = now;
			}

			++fps;
			last_frame = now;
		}
		last_update = now;
	}
	Window::terminate();
	return 0;
}


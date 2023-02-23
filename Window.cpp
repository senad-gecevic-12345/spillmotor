#include "Window.h"

const int Window::gl_ver_maj{ 3 };
const int Window::gl_ver_min{ 3 };
int Window::width{ 1920 };
int Window::height{ 1080 };
double Window::mouse_x{ 0 };
double Window::mouse_y{ 0 };
bool Window::cursor{ 1 };
GLFWwindow* Window::window{ nullptr };
Keyboard* Keyboard::keyboard{ nullptr };
PollKeyboard* PollKeyboard::instance{ nullptr };
std::unordered_map<int, bool> Window::key_map;

void Window::swap_buffers() { glfwSwapBuffers(window); }
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	Window::width = width;
	Window::height = height;
	glViewport(0, 0, width, height);
}

void Window::cursor_position_callback(GLFWwindow* window, double mouse_x, double mouse_y) {
	if (!cursor)
		return;
	Window::mouse_x = mouse_x;
	Window::mouse_y = mouse_y;
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
}

//other config
void Window::keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// press and or release
	// wrong

	key_map[key] = !key_map[key];

	if (key == GLFW_KEY_ESCAPE) {
		set_window_should_close();
	}
	if (key == GLFW_KEY_W) {
		Keyboard::get().w = !Keyboard::get().w;
	}
	if (key == GLFW_KEY_A) {
		Keyboard::get().a = !Keyboard::get().a;
	}
	if (key == GLFW_KEY_S) {
		Keyboard::get().s = !Keyboard::get().s;
	}
	if (key == GLFW_KEY_D) {
		Keyboard::get().d = !Keyboard::get().d;
	}
	if (key == GLFW_KEY_SPACE) {
		Keyboard::get().space = !Keyboard::get().space;
	}
	if (key == GLFW_KEY_X) {
		Keyboard::get().x = !Keyboard::get().x;
	}
	if (key == GLFW_KEY_Z) {
		//Keyboard::get().z = !Keyboard::get().z;
		cursor = 1;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	}
	if (key == GLFW_KEY_X) {
		cursor = 0;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	}

}

void Window::set_window_should_close() {
	glfwSetWindowShouldClose(window, true);
}

GLFWwindow* Window::get_window() { return window; }

bool Window::window_should_close() { return glfwWindowShouldClose(window); }

float Window::get_aspect_ratio() {
	return (float)width / (float)height;
}

void Window::terminate() {
	glfwDestroyWindow(window);
}

int Window::init() {
	static bool is_init = false;
	if (is_init)return -1;
	if (!glfwInit())return -1;

	/*
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	*/

	// top of sphere not rendering correctly
	// seems sampling affect sampling
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(width, height, "Window", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSwapInterval(0);
	glfwSetFramebufferSizeCallback(window, Window::framebuffer_size_callback);
	glfwSetCursorPosCallback(window, Window::cursor_position_callback);
	glfwSetMouseButtonCallback(window, Window::mouse_button_callback);
	glfwSetKeyCallback(window, Window::keyboard_callback);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
	// GLFW_STICKY_KEYS 
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_MULTISAMPLE);
	//glfwSwapInterval(120);
	is_init = true;
	return 1;
}



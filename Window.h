#ifndef WINDOW_H_
#define WINDOW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <cstring>

struct Keyboard {
    static Keyboard* keyboard;
    bool w, a, s, d, space, q, e, z, x, c;
    Keyboard() {
        w = false;
        a = false;
        s = false;
        d = false;
        space = false;
        q = false;
        e = false;
        z = false;
        x = false;
        c = false;
    }
    static Keyboard& get() {
        if (keyboard == nullptr) {
            keyboard = new Keyboard();
        }
		return *keyboard;
    }
};

class Window
{
public:
    static const int gl_ver_maj, gl_ver_min;
    static int width, height;
    static double mouse_x, mouse_y;
	static bool cursor;
    static GLFWwindow* window;
	// for key_repeat
	static std::unordered_map<int, bool> key_map;
public:
    static GLFWwindow* get_window();
    static void swap_buffers();
    static void framebuffer_size_callback(GLFWwindow*, int win_width, int win_height);
    static void cursor_position_callback(GLFWwindow*, double mouse_x, double mouse_y);
    static void mouse_button_callback(GLFWwindow*, int button, int action, int mods);
    static void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void set_window_should_close();
    static bool window_should_close();
    static void terminate();
    static int init();
	static float get_aspect_ratio();
};

struct PollKeyboard {
    static PollKeyboard* instance;
    static PollKeyboard& get() {
        if (instance == nullptr) {
            instance = new PollKeyboard();
        }
        return *instance;
    }
	int get_key_state(int key) {
		return glfwGetKey(Window::get_window(), key);
	}
    bool w, a, s, d, space, q, e, z, x, c, up, down, left, right;
    void update() {
        // lol
        auto check = [](int keycode, bool& key){
            if(glfwGetKey(Window::get_window(), keycode) != key){
                key = !key;
            }
        };
        check(GLFW_KEY_W, w);
        check(GLFW_KEY_A, a);
        check(GLFW_KEY_S, s);
        check(GLFW_KEY_D, d);
        check(GLFW_KEY_Z, z);
        check(GLFW_KEY_X, x);
        check(GLFW_KEY_UP, up);
        check(GLFW_KEY_DOWN, down);
        check(GLFW_KEY_LEFT, left);
        check(GLFW_KEY_RIGHT, right);
        check(GLFW_KEY_SPACE, space);
    }
     PollKeyboard() {
         //memset(keys, 0, sizeof(keys));
        w = false;
        a = false;
        s = false;
        d = false;
        space = false;
        q = false;
        e = false;
        z = false;
        x = false;
        c = false;
		up = false;
		down = false;
		left = false;
		right = false;
     }
};


#endif // WINDOW_H_

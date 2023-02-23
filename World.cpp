#include "World.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace World {
	Clock& Clock::get() {
		static Clock instance;
        return instance;
	}
	float Clock::get_dt() const { return (float)dt; }
	float Clock::get_time() const { return glfwGetTime(); }
	void Clock::update(){
		double now = glfwGetTime();
		dt = now - last_update;
		last_update = now;
	}
	Clock::Clock(){
		init_time = glfwGetTime();
		last_update = init_time;
		dt = 0;

	}
	float get_time() {
		return Clock::get().get_time();
	}


};


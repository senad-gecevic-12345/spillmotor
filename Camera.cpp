#include "Camera.h"
#include "World.h"

glm::mat4 make_view_matrix(glm::vec3 pos, glm::vec3 front, glm::vec3 up) {
	return glm::lookAt(pos, pos + front, up);
}

glm::vec3 ControllerCameraFunctions::get_update_camera_pos(const CameraData& data, CameraPosTemp& pos_data){
    const glm::vec3 _front = data.front;
    glm::vec3& pos = pos_data.pos;
    auto dt = World::Clock::get().get_dt();
    PollKeyboard::get().update();
    pos_data._last_z = Keyboard::get().z;
    pos_data._last_x = Keyboard::get().x;
    pos_data._last_w = Keyboard::get().w;
    pos_data._last_a = Keyboard::get().a;
    pos_data._last_s = Keyboard::get().s;
    pos_data._last_d = Keyboard::get().d;
    return pos;
}

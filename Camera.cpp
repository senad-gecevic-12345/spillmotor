#include "Camera.h"
#include "World.h"
glm::mat4 make_view_matrix(glm::vec3 pos, glm::vec3 front, glm::vec3 up) {
	return glm::lookAt(pos, pos + front, up);
}

void ControllerCameraFunctions::mouse_update(double xposIn, double yposIn, CameraData& camera)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    float xoffset = xpos - camera.m_last_x;
    float yoffset = camera.m_last_y - ypos; 
    camera.m_last_x = xpos;
    camera.m_last_y = ypos;

    float sensitivity = 0.1f; 
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.m_yaw += xoffset;
    camera.m_pitch += yoffset;

    if (camera.m_pitch > 89.0f)
        camera.m_pitch = 89.0f;
    if (camera.m_pitch < -89.0f)
        camera.m_pitch = -89.0f;

    glm::vec3 _front;
    _front.x = cos(glm::radians(camera.m_yaw)) * cos(glm::radians(camera.m_pitch));
    _front.y = sin(glm::radians(camera.m_pitch));
    _front.z = sin(glm::radians(camera.m_yaw)) * cos(glm::radians(camera.m_pitch));
    camera.front = glm::normalize(_front);
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
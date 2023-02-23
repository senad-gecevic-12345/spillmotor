#ifndef CAMERA_H_
#define CAMERA_H_


// after turing the camera either correct or not. dont know about that turn as should be restricted

#include "glm/glm.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"
#include "Math.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"

using namespace Math;

glm::mat4	make_projection_matrix(float fov, int width, int height);
glm::mat4	make_view_matrix(glm::vec3 pos, glm::vec3 front, glm::vec3 up);
namespace ControllerCameraFunctions{

// just copy over the data
// create a custom actor with virtual functions and another data path to attach this or somethig
struct CameraData {
    float m_last_x{0};
    float m_last_y{0};
    float m_yaw{0}, m_pitch{0};
    glm::vec3 front = glm::vec3(0,0,1);
    CameraData(float x, float y):m_last_x(x), m_last_y(y){}
};

void mouse_update(double xposIn, double yposIn, CameraData& camera);


// this is needed
// the position update is not good at all

inline glm::mat4 get_fps_camera(double x, double y, double sensitivity, const CameraData& camera_data_2, glm::vec3 camera_pos) {




    glm::vec3 up = glm::vec3(0, 1.f, 0);
    return glm::lookAt(camera_pos, camera_pos + camera_data_2.front, up);
}

inline glm::mat4 get_fps_camera(double x, double y, double sensitivity, const CameraData& camera_data_2, glm::vec3 camera_pos, glm::vec3 lookat) {
    glm::vec3 up = glm::vec3(0, 1.f, 0);
    return glm::lookAt(camera_pos, lookat + camera_data_2.front, up);
}

}

namespace ControllerCameraFunctions{
    // wtf is this even
struct CameraPosTemp{
    double last_x{0}, last_y{0};
    float rot_x{0}, rot_y{0};
    bool _last_z = false;
    bool _last_x = false;
    bool _last_w{ 0 }, _last_a{ 0 }, _last_s{ 0 }, _last_d{ 0 };
    glm::vec3 pos = glm::vec3(0,0,0);

};

// need just the directoion or something
glm::vec3 get_update_camera_pos(const CameraData& data, CameraPosTemp& pos_data);

}


namespace NewCamera{
    class Camera{
    public:
        double m_yaw;
        double m_pitch;
        float m_offset_lenth{15};
        
        glm::vec3 m_follow_pos;
        glm::vec3 m_front;
        glm::vec3 m_position;

        glm::mat4 m_view;

    public:
        Camera(){

        }
        glm::mat4 get_view(){
            return m_view;
        }

        void update(double mouse_x, double mouse_y, glm::vec3 follow_pos){
            m_follow_pos = follow_pos;

            static double last_x{mouse_x}, last_y{mouse_y};

            m_yaw -= last_x - mouse_x;
            m_pitch -= last_y - mouse_y;

            last_x = mouse_x;
            last_y = mouse_y;

            if(m_pitch > 20.f){
                m_pitch = 20.f;
            }
            if(m_pitch < -0.f){
                m_pitch = -0.f;
            }
            if(m_yaw >= 360.f){
                m_yaw = 0;
            }
            if(m_yaw <= -360.f){
                m_yaw = 0;
            }

            m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            m_front.y = sin(glm::radians(m_pitch));
            m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

            glm::vec3 offset = m_front * m_offset_lenth;

            glm::vec3 up = glm::vec3(0, 1.f, 0);
            m_view = glm::lookAt(offset + follow_pos, follow_pos, up);

            m_position = offset + follow_pos;
        }

        glm::vec3 get_front(){
            return glm::normalize(m_follow_pos - (m_front + m_follow_pos));
        }
    };

}

#endif // CAMERA_H_

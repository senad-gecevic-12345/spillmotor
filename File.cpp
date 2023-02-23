#include "File.h"
#include <string>
namespace ParserFunctions{

int first_non_space(const std::string& str, int index){
    int i = index;
    for( ; i < str.size(); ++i){
        if(str[i] != ' ')
            break;
    }
    return i;
};

std::string float_to_str(float fl){
    return std::to_string(fl);
}

std::string glm_vec3_to_str(const glm::vec3 vec){
    return std::to_string(vec.x) + " " + std::to_string(vec.y) + " " + std::to_string(vec.z) + " ";
}

std::string bullet_quaternion_to_str(const btQuaternion quaternion){
    btScalar zyx[3];
    quaternion.getEulerZYX(zyx[0], zyx[1], zyx[2]);
    return std::to_string((float)zyx[0]) + " " + std::to_string((float)zyx[1]) + " " + std::to_string((float)zyx[2]) + " ";
}

std::tuple<int, std::string> get_word(const std::string& str, int index){
    if(index == -1)
        return std::make_tuple(-1, "");

    std::string out;
    int i = first_non_space(str, index);
    for( ; i < str.size(); ++i){
        char ch = str[i];
        if(ch == ' '){
            break;
        }
        if(ch == '\n'){
            break;
        }
        out.push_back(ch);
    }
    if(out.size() == 0) 
        return std::make_tuple(-1, "");
    return std::make_tuple(i, out);
}

std::tuple<int, float> get_float(const std::string& str, int index){
    if(index == -1)
        return std::make_tuple(-1, -1);
    int i = first_non_space(str, index);
    std::string float_chars;

    for( ; i < str.size(); ++i){
        char ch = str[i];
        if(isdigit(ch)){
            float_chars.push_back(ch);
        }
        else if(ch == '.'){
            float_chars.push_back(ch);

        }         
        else{
            break;
        }
    }
    
    if(float_chars.size() == 0)
        return std::make_tuple(-1, -1);
    return std::make_tuple(i, std::stof(float_chars));
}

std::tuple<int, btQuaternion> get_quaternion(const std::string& str, int index){
    btScalar val[3];
    btQuaternion quaternion;
    int idx = index;
    for(int i = 0; i < 3; ++i){
        auto [r_idx, r_fl] = get_float(str, index);
        val[i] = r_fl;
        idx = r_idx;
        if(idx == -1)
            return std::make_tuple(-1, quaternion);
    }
    quaternion.setEulerZYX(val[0], val[1], val[2]);
    return std::make_tuple(idx, quaternion);
}

std::tuple<int, glm::vec3> get_vec3(const std::string& str, int index){
    glm::vec3 vec;
    int idx = index;
    for(int i = 0; i < 3; ++i){
        auto [r_idx, r_fl] = get_float(str, index);
        vec[i] = r_fl;
        idx = r_idx;
        if(idx == -1)
            return std::make_tuple(-1, vec);
    }
    return std::make_tuple(idx, vec);
}

std::vector<std::string> read_file(std::string file_loc_name){
    std::ifstream file;
    file.open(file_loc_name);
    std::string line;
    std::vector<std::string> out;
    if(file.is_open()){
        while(getline(file, line)){
            out.push_back(line);
        }
    }
    return out;
}

// return next pos or something
// create a custom stream or stream operators for level file thingy
void write_file(std::string file_loc_name, const std::vector<std::string>& vec){
    std::ofstream file;
    // does it clear
    file.open(file_loc_name);
    if(file.is_open()){
        // doest it have \n
        for(int i = 0; i < vec.size(); ++i)
            file << vec[i];
    }
    file.close();
};

}


#ifndef MODELSAVEFILE_H_
#define MODELSAVEFILE_H_

#include <fstream>

class ModelSaveFile{
public:
    std::string save_file_location{"/home/solidus/Programming/ECS/ECSAOENTHAUNTH/model_save_file.txt"};
    std::vector<std::string> model_locations;
    bool check(const std::string& file_loc_name){
        std::ifstream stream(file_loc_name);
        return stream.good();
    }
    void read_from_file(){
        std::ifstream stream(save_file_location);
        std::string buffer;
        while(getline(stream, buffer)){
            model_locations.push_back(buffer);
        }
    }
public:
    void reload(){
        model_locations.clear();
        read_from_file();
    }
    const std::vector<std::string>& get_list()const{
        return model_locations;
    }
    void add(const std::string& file_loc_name){
        std::ofstream stream;
        stream.open(save_file_location, std::ios_base::app);
        std::string buffer;
        for(auto x : model_locations){
            if(file_loc_name == x){
                return;
            }
        }
        stream << file_loc_name;
    }
    void remove(const std::string& file_loc_name){
        for(int i = 0; i < model_locations.size(); ++i){
            if(model_locations[i] == file_loc_name){
                std::swap(model_locations[i], model_locations[model_locations.size()-1]);
                model_locations.pop_back();
                std::ofstream stream;
                stream.open(save_file_location, std::ofstream::out|std::ofstream::trunc);
                stream.flush();
                stream.close();
                read_from_file();
            }
        }
    }
    ModelSaveFile(){
        read_from_file();
    }
};



#endif // MODELSAVEFILE_H_

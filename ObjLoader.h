#pragma once 

#include "utils.h"
#include "HittableList.h"
#include "Triangle.h"

// TODO: DELETE!!
#include "Quad.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> tokenize(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

class ObjLoader {
    public:
    inline static void load(HittableList& hittable_list, const std::string& file_name, const std::shared_ptr<Material> material) {
        std::vector<Vec3> points;

        std::ifstream file(file_name);
        if (!file.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::vector<std::string> tokens = tokenize(line, ' ');

            if (tokens.size() == 4 && tokens[0] == "v") {
                points.push_back(Vec3(std::stod(tokens[1]), std::stod(tokens[2]), std::stod(tokens[3])));
            } else if (tokens.size() == 4 && tokens[0] == "f") {
                // Triangle face
                std::vector<Vec3> triange_points; 
                for (int i = 1; i < 4; ++i) {
                    std::vector<std::string> pointIdx = tokenize(tokens[i], '/');
                    triange_points.push_back(points[std::stoi(pointIdx[0]) - 1]);
                }
                hittable_list.add(std::make_shared<Triangle>(
                    triange_points[0], triange_points[1] - triange_points[0], triange_points[2] - triange_points[0],
                    material));
            } else if (tokens.size() == 5 && tokens[0] == "f") { 
                // break face with 4 vertices into two triangles
                std::vector<Vec3> vertices; 
                for (int i = 1; i < 5; ++i) {
                    std::vector<std::string> pointIdx = tokenize(tokens[i], '/');
                    vertices.push_back(points[std::stoi(pointIdx[0]) - 1]);
                }
                // hittable_list.add(std::make_shared<Triangle>(
                //     vertices[0], vertices[0] - vertices[1], vertices[0] - vertices[3],
                //     material));
                // hittable_list.add(std::make_shared<Triangle>(
                //     vertices[2], vertices[2] - vertices[1], vertices[2] - vertices[3],
                //     material));
                hittable_list.add(std::make_shared<Triangle>(
                    vertices[0], vertices[1] - vertices[0], vertices[2] - vertices[0],
                    material));
                hittable_list.add(std::make_shared<Triangle>(
                    vertices[0], vertices[2] - vertices[0], vertices[3] - vertices[0],
                    material));
            } 
        }

        file.close();
    }
};

#include <model.h>

namespace rt {
    std::vector<std::string> split(std::string str, char c) {
        std::string part;
        std::vector<std::string> parts;
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == c) {
                parts.push_back(part);
                part = "";
                continue;
            }
            part += str[i];
        }
        parts.push_back(part);
        return parts;
    }

    void loadOBJ(std::vector<triangle_t>& tris, char* path, mat::material_t mat, glm::vec3 position) {
        std::ifstream file(path);
        std::vector<glm::vec3> vertices;
        int count = 0;
        for( std::string line; std::getline(file, line); ) {
            
            std::vector<std::string> parts = split(line, ' ');
            if (parts[0] == "v") {
                vertices.push_back(glm::vec3(std::stof(parts[1]) / 5.0f, std::stof(parts[2]) / 5.0f, std::stof(parts[3]) / 5.0f) + position);
            }
            else if (parts[0] == "f") {
                count++;
                tris.push_back(rt::createTri(vertices[std::stoi(split(parts[1], '/')[0]) - 1], vertices[std::stoi(split(parts[2], '/')[0]) - 1], vertices[std::stoi(split(parts[3], '/')[0]) - 1], mat, glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
            }
        }
        printf("Added %d tris.\n", count);
    }
}
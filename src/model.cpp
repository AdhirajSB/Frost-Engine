#include "model.h"


Model::Model(const std::string &objPath){
    std::ifstream fileOBJ(objPath);

    if (!fileOBJ.is_open()){
        throw std::runtime_error("Failed to open file at: " + objPath);
    }


    std::vector<Vertex> vertices;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    std::string line;
    while (std::getline(fileOBJ, line)){
        // Ignore comments and empty lines
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string prefix; iss >> prefix;

        if (prefix == "v"){
            float x, y, z;
            iss >> x >> y >> z;
            positions.push_back(glm::vec3(x, y, z));
        }
        else if (prefix == "vn"){
            float x, y, z;
            iss >> x >> y >> z;
            normals.push_back(glm::vec3(x, y, z));
        }
        else if (prefix == "vt"){
            float u, v;
            iss >> u >> v;
            uvs.push_back(glm::vec2(u, v));
        }
        else if (prefix == "f"){
            std::string vertexStr;

            Vertex triangle[3]; // Iterate over 3 vertices to get a triangle
            for (int i = 0; i < 3; i++){
                iss >> vertexStr;

                std::istringstream viss(vertexStr);
                std::string indStr;

                int posIndex(-1), uvIndex(-1), normIndex(-1);
                if (std::getline(viss, indStr, '/') && !indStr.empty()){
                    posIndex = std::stoi(indStr) - 1;
                }
                if (std::getline(viss, indStr, '/') && !indStr.empty()){
                    uvIndex = std::stoi(indStr) - 1;
                }
                if (std::getline(viss, indStr, '/') && !indStr.empty()){
                    normIndex = std::stoi(indStr) - 1;
                }

                if (posIndex != -1) triangle[i].pos = positions[posIndex];
                if (uvIndex != -1) triangle[i].uvCoord = uvs[uvIndex];
                if (normIndex != -1) triangle[i].norm = normals[normIndex];
            }

            for (int i = 0; i < 3; i++){
                vertices.push_back(triangle[i]);
            }
        }
    }

    positions.clear();
    normals.clear();
    uvs.clear();

    SetupBuffers(vertices);
    vertices.clear();
}


void Model::SetupBuffers(const std::vector<Vertex>& vertices){
    m_vertexCount = vertices.size();

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertexCount, vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, norm));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uvCoord));

    glBindVertexArray(0); // Unbind for safety
}

void Model::Draw() const{
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
}

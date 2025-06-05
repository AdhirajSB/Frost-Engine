#include "model.h"


Model::Model(const std::string &objPath){
    std::ifstream fileOBJ(objPath);

    if (!fileOBJ.is_open()){
        throw std::runtime_error("Failed to open file at: " + objPath);
    }
    LoadMTL(objPath);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    std::string line;
    while (std::getline(fileOBJ, line)){
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
        else if (prefix == "usemtl"){
            iss >> m_CurrentMaterial;
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
                m_MaterialGroup[m_CurrentMaterial].push_back(triangle[i]);
            }
        }
    }

    positions.clear();
    normals.clear();
    uvs.clear();

    SetupBuffers();
}

void Model::LoadMTL(std::string mtlPath){
    int size = mtlPath.length();
    mtlPath[size - 3] = 'm';
    mtlPath[size - 2] = 't';
    mtlPath[size - 1] = 'l';

    std::ifstream fileMTL(mtlPath);
    if (!fileMTL.is_open()){
        throw std::runtime_error("Failed to open file at: " + mtlPath);
    }

    std::string line;
    while (std::getline(fileMTL, line)){
        if (line[0] == '#' || line.empty()) continue;

        std::istringstream iss(line);
        std::string prefix; iss >> prefix;

        if (prefix == "newmtl"){
            iss >> m_CurrentMaterial;
        }
        else if (prefix == "Ka"){
            float r, g, b;
            iss >> r >> g >> b;
            m_MaterialProperty[m_CurrentMaterial].ambientColor = glm::vec3(r, g, b);
        }
        else if (prefix == "Kd"){
            float r, g, b;
            iss >> r >> g >> b;
            m_MaterialProperty[m_CurrentMaterial].diffuseColor = glm::vec3(r, g, b);
        }
        else if (prefix == "Ks"){
            float r, g, b;
            iss >> r >> g >> b;
            m_MaterialProperty[m_CurrentMaterial].specularColor = glm::vec3(r, g, b);
        }
        else if (prefix == "Ke"){
            float r, g, b;
            iss >> r >> g >> b;
            m_MaterialProperty[m_CurrentMaterial].emissiveColor = glm::vec3(r, g, b);
        }
        else if (prefix == "d"){
            float alpha; iss >> alpha;
            m_MaterialProperty[m_CurrentMaterial].alpha = alpha;
        }
    }
}


void Model::SetupBuffers(){
    for (const auto& [material, vertices]: m_MaterialGroup){
        unsigned int VAO, VBO;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, norm));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uvCoord));

        glBindVertexArray(0);

        m_RenderID[material] = VAO;
    }
}

void Model::Draw(Shader* shader){
    for (const auto& [material, vertices]: m_MaterialGroup){
        shader->SetVec3("ambientColor", m_MaterialProperty.at(material).ambientColor);
        shader->SetVec3("diffuseColor", m_MaterialProperty.at(material).diffuseColor);
        shader->SetVec3("specularColor", m_MaterialProperty.at(material).specularColor);
        shader->SetVec3("emissiveColor", m_MaterialProperty.at(material).emissiveColor);
        shader->SetFloat("alpha", m_MaterialProperty.at(material).alpha);
        
        glBindVertexArray(m_RenderID.at(material));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }
}

void Model::DrawPicking() const{
    for (const auto& [material, vertices]: m_MaterialGroup){
        glBindVertexArray(m_RenderID.at(material));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }
}
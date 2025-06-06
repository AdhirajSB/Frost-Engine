#include "model.h"
#include <filesystem>


Model::Model(const std::string &objPath){
    std::ifstream fileOBJ(objPath);

    if (!fileOBJ.is_open()){
        throw std::runtime_error("Failed to open file at: " + objPath);
    }

    stbi_set_flip_vertically_on_load(true);
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
        else if (prefix == "f") {
            std::string vertexStr;

            Vertex triangle[3];
            for (int i = 0; i < 3; i++) {
                iss >> vertexStr;
                std::istringstream viss(vertexStr);
                std::string indStr;

                int posIndex(-1), uvIndex(-1), normIndex(-1);
                if (std::getline(viss, indStr, '/') && !indStr.empty())
                    posIndex = std::stoi(indStr) - 1;
                if (std::getline(viss, indStr, '/') && !indStr.empty())
                    uvIndex = std::stoi(indStr) - 1;
                if (std::getline(viss, indStr, '/') && !indStr.empty())
                    normIndex = std::stoi(indStr) - 1;

                if (posIndex != -1) triangle[i].pos = positions[posIndex];
                if (uvIndex != -1) triangle[i].uvCoord = uvs[uvIndex];
                if (normIndex != -1) triangle[i].norm = normals[normIndex];
            }

            // === Tangent Calculation ===
            glm::vec3 edge1 = triangle[1].pos - triangle[0].pos;
            glm::vec3 edge2 = triangle[2].pos - triangle[0].pos;

            glm::vec2 deltaUV1 = triangle[1].uvCoord - triangle[0].uvCoord;
            glm::vec2 deltaUV2 = triangle[2].uvCoord - triangle[0].uvCoord;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            glm::vec3 tangent = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
            tangent = glm::normalize(tangent);

            for (int i = 0; i < 3; i++) {
                triangle[i].tangent = tangent;
                m_MaterialGroup[m_CurrentMaterial].push_back(triangle[i]);
            }
        }
    }

    positions.clear();
    normals.clear();
    uvs.clear();

    SetupBuffers();
}

unsigned int LoadTextureFromFile(const std::string& mapPath){
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(mapPath.c_str(), &width, &height, &nrComponents, 0);
    if (data){
        GLenum format = 0;
        GLenum internalFormat = 0;

        if (nrComponents == 4) {
            format = GL_RGBA;
            internalFormat = GL_RGBA8;
        }
        else if (nrComponents == 3){
            format = GL_RGB;
            internalFormat = GL_RGB8;
        }
        else if (nrComponents == 1) {
            format = GL_RED;
            internalFormat = GL_R8;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "glTexImage2D failed with error: 0x" << std::hex << error << std::dec << std::endl;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else{
        stbi_image_free(data);
        throw std::runtime_error("Texture failed to load at path: " + mapPath);
    }
        
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

void Model::LoadMTL(std::string objPath){
    namespace fs = std::filesystem;

    fs::path mtlPath = objPath;
    mtlPath.replace_extension(".mtl");

    std::ifstream fileMTL(mtlPath);
    if (!fileMTL.is_open()){
        throw std::runtime_error("Failed to open file at: " + mtlPath.string());
    }

    fs::path mtlDir = mtlPath.parent_path();

    std::string line;
    while (std::getline(fileMTL, line)){
        if (line[0] == '#' || line.empty()) continue;

        std::istringstream iss(line);
        std::string prefix; iss >> prefix;

        if (prefix == "newmtl"){
            iss >> m_CurrentMaterial;
        }
        else if (prefix == "Na"){
            float shininess; iss >> shininess;
            m_MaterialProperty[m_CurrentMaterial].specularExponent = shininess;
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
        else if (prefix == "map_Kd"){
            std::string relPath; iss >> relPath;
            std::string fullPath = (mtlDir / relPath).string();
            m_MaterialProperty[m_CurrentMaterial].diffuseTexture = LoadTextureFromFile(fullPath);
            m_MaterialProperty[m_CurrentMaterial].hasDiffuseTexture = true;
        }
        else if (prefix == "map_d"){
            std::string relPath; iss >> relPath;
            std::string fullPath = (mtlDir / relPath).string();
            m_MaterialProperty[m_CurrentMaterial].alphaTexture = LoadTextureFromFile(fullPath);
            m_MaterialProperty[m_CurrentMaterial].hasAlphaTexture = true;
        }
        else if (prefix == "map_Bump"){
            std::string relPath; iss >> relPath;
            std::string fullPath = (mtlDir / relPath).string();
            m_MaterialProperty[m_CurrentMaterial].normalTexture = LoadTextureFromFile(fullPath);
            m_MaterialProperty[m_CurrentMaterial].hasNormalTexture = true;
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
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));

        glBindVertexArray(0);

        m_RenderID[material] = VAO;
    }
}

void Model::Draw(Camera* camera, Shader *shader){
    for (const auto& [material, vertices]: m_MaterialGroup){
        const Material& mat = m_MaterialProperty.at(material);
        
        shader->SetFloat("specularExponent", mat.specularExponent);
        shader->SetVec3("ambientColor", mat.ambientColor);
        shader->SetVec3("diffuseColor", mat.diffuseColor);
        shader->SetVec3("specularColor", mat.specularColor);
        shader->SetVec3("emissiveColor", mat.emissiveColor);
        shader->SetFloat("alpha", mat.alpha);
        
        shader->SetBool("hasDiffuseTexture", mat.hasDiffuseTexture);
        shader->SetBool("hasAlphaTexture", mat.hasAlphaTexture);
        shader->SetBool("hasNormalTexture", mat.hasNormalTexture);

        shader->SetVec3("cameraPos", camera->CameraPos());

        if (mat.hasDiffuseTexture){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mat.diffuseTexture);
            shader->SetInt("diffuseTexture", 0);
        }
        if (mat.hasAlphaTexture){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mat.alphaTexture);
            shader->SetInt("alphaTexture", 1);
        }
        if (mat.hasNormalTexture){
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, mat.normalTexture);
            shader->SetInt("normalTexture", 2);
        }

        glBindVertexArray(m_RenderID.at(material));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        for (int i = 0; i < 3; i++){
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

void Model::DrawOutline(Shader *outlineShader){
    outlineShader->Use();
    for (const auto& [material, vertices]: m_MaterialGroup){
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
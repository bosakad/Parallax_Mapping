#include "Geometry.hpp"

/**
 * @brief      Constructs the plane object.
 * 
 * @param[in]  vertexShaderPath  The vertex shader path
 * @param[in]  fragmentShaderPath  The fragment shader path
 * @param[in]  translation       The translation
 *  @param[in]  rotation       The rotation
 * @param[in]  scale             The scale
 * @param[in]  texturePath       The texture path
 * @param[in]  normalMapPath     The normal map path
 * @param[in]  heightMapPath     The height map path
 * @param[in]  inverseH          Indicates if inverse heightmap
 * 
 * @return     The plane object.
 * 
 * NOTE: Partially inspired by https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/4.normal_mapping/normal_mapping.cpp
*/
Object * CreatePlane(const std::string& vertexShaderPath, const std::string& fragmentShaderPath,
                     const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale,
                     const std::string& diffuseMapPath, const std::string& normalMapPath, 
                     const std::string& heightMapPath, bool inverseH, int continousTexture){

                        // positions
                        glm::vec3 pos1(-1.0f,  1.0f, 0.0f);
                        glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
                        glm::vec3 pos3( 1.0f, -1.0f, 0.0f);
                        glm::vec3 pos4( 1.0f,  1.0f, 0.0f);

                        // texture coordinates
                        glm::vec2 uv1(0.0f, 1.0f);
                        glm::vec2 uv2(0.0f, 0.0f);
                        glm::vec2 uv3(1.0f, 0.0f);  
                        glm::vec2 uv4(1.0f, 1.0f);

                        // normal vector
                        glm::vec3 nm(0.0f, 0.0f, 1.0f);

                        // calculate tangent/bitangent vectors of both triangles
                        glm::vec3 tangent1, bitangent1;
                        glm::vec3 tangent2, bitangent2;
                        // triangle 1
                        // ----------
                        glm::vec3 edge1 = pos2 - pos1;
                        glm::vec3 edge2 = pos3 - pos1;
                        glm::vec2 deltaUV1 = uv2 - uv1;
                        glm::vec2 deltaUV2 = uv3 - uv1;

                        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

                        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

                        // triangle 2
                        // ----------
                        edge1 = pos3 - pos1;
                        edge2 = pos4 - pos1;
                        deltaUV1 = uv3 - uv1;
                        deltaUV2 = uv4 - uv1;

                        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


                        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

                        // normalize
                        bitangent1 = glm::normalize(bitangent1);
                        bitangent2 = glm::normalize(bitangent2);

                        tangent1 = glm::normalize(tangent1);
                        tangent2 = glm::normalize(tangent2);

                        std::vector<GLfloat> data = {
                            // positions            // normal         // texcoords  // tangent                          // bitangent
                            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
                            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
                            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
                            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
                        };

                        // define indices
                        std::vector<GLuint> indices = {0, 1, 2, 0, 2, 3};

                        // create object 

                        Object * plane = new Object(vertexShaderPath, fragmentShaderPath);

                        // upload data to GPU
                        plane->UploadVertices(data, indices);

                        // load texture
                        plane->LoadTexture(diffuseMapPath, 0, false);
                        plane->LoadTexture(normalMapPath, 1, false);
                        plane->LoadTexture(heightMapPath, 2, inverseH);

                        // set affine transformation
                        plane->SetTranslation(translation);
                        plane->SetRotation(rotation);
                        plane->SetScale(scale);
                        
                        // set number of elements
                        plane->SetNumberOfElements(indices.size());

                        // set continous texture to false / true
                        plane->SetContinuousTexture(continousTexture);

                        return plane;
                     }
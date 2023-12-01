#include "ObjectParser.hpp"
#include <bits/stdc++.h> 
#include "PPM.hpp"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

// function that parses .obj file and return tuple of indices and vertices
void ObjectParser::Parse_WavefrontOBJ(std::vector<GLfloat> &data, std::vector<GLuint> &indices, 
                                                    std::string &filePath, std::string &texturePath){
      // Check if file is a obj file
    if (filePath.substr(filePath.find_last_of(".") + 1) != "obj"){
        std::cerr << "Error: The file is not an obj file!" << std::endl;
        exit(-2);   // exit program
    }

    // Open file
    std::ifstream objFileStream(filePath);

    // Check if file is open
    if (!objFileStream.is_open()){
        std::cerr << "Error: The file could not be opened!" << std::endl;
        exit(-3);
    }

    ///////////////////////////////////////// parsing ///////////////////////////////////

    // alloc variables
    std::vector<Vertex> verticesArray; // vertex coords
    std::vector<Normal> normalsArray;   // vertex normals
    std::vector<TextureCoords> texturesArray;   // texture coords

    // map that maps vertex indices to index in element array
    std::map< std::tuple<unsigned int, unsigned int, unsigned int> , unsigned int > vertexToIndex;

    bool hasTexture = true; // if the model has texture coords

    std::string token;
    std::string delimiter = "/";  // separating delimiter

    // parse file
    while(objFileStream >> token){

        // save texture name
        if (token == "mtllib"){
            objFileStream >> texturePath;
            std::string directoryPath = filePath.substr(0, filePath.find_last_of("/") + 1);
            texturePath = directoryPath + texturePath;
        }

        // vertex info
        if (token == "v"){

            // parse x, y, z vertex coords
            objFileStream >> token;
            GLfloat x = std::stof(token);
            objFileStream >> token;
            GLfloat y = std::stof(token);
            objFileStream >> token;
            GLfloat z = std::stof(token);

            // save the vertex
            verticesArray.push_back(Vertex(x, y, z));

        }

        if (token == "vn"){

            // parse x, y, z normal coords
            objFileStream >> token;
            GLfloat vx = std::stof(token);
            objFileStream >> token;
            GLfloat vy = std::stof(token);
            objFileStream >> token;
            GLfloat vz = std::stof(token);


            // save the vertex
            normalsArray.push_back(Normal(vx, vy, vz));
        }

        if (token == "vt"){

            // parse s,t  texture coords
            objFileStream >> token;
            GLfloat s = std::stof(token);
            objFileStream >> token;
            GLfloat t = std::stof(token);

            // save the texture coords
            texturesArray.push_back(TextureCoords(s, t));
        }

        // faces info
        if (token == "f"){

            unsigned int vertexIndex, textureIndex, normalIndex;

            // parse all 3 triples 
            for (unsigned pos = 0; pos < 3; pos++){

                objFileStream >> token; // get triple divided by a slash

                                    // get vertex index
                unsigned int posFirstSlash = token.find(delimiter);
                std::string numAsString = token.substr(0, posFirstSlash);

                vertexIndex = std::stoi(numAsString) - 1;
                
                                    // get texture index
                std::string restOfTheString = token.substr(posFirstSlash + 1, token.size());
                unsigned int posNextSlash = restOfTheString.find(delimiter);
                
                textureIndex = 0;
                if (posNextSlash != 0){    // if there is texture coord (next slash is not on the 1 character)
                    numAsString = restOfTheString.substr(0, posNextSlash);
                    textureIndex = std::stoi(numAsString) - 1;  // get texture index
                }
                else{   // no texture - just append 2 zeros and put zeros everywhere
                    if (texturesArray.size() == 0){
                        texturesArray.push_back(TextureCoords(0, 0));
                        hasTexture = false;
                    }
                    
                    
                }

                                    // get normal coord
                // rest of the string
                restOfTheString = restOfTheString.substr(posNextSlash + 1, restOfTheString.size());
                normalIndex = std::stof(restOfTheString) - 1;
                
                // create vertex triple
                std::tuple faceVertex = std::tuple(vertexIndex, normalIndex, textureIndex);
                auto iter = vertexToIndex.find( faceVertex );

                // vertex not yet registered - register it
                if ( iter == vertexToIndex.end() ){
                
                    Vertex * vCoords = &verticesArray[vertexIndex];
                    Normal * nCoords = &normalsArray[normalIndex];
                    TextureCoords* tCoords = &texturesArray[textureIndex];
                    
                    // push the vertex data into the final data array
                    data.push_back(vCoords->x) ;data.push_back(vCoords->y) ;data.push_back(vCoords->z);
                    data.push_back(nCoords->vx);data.push_back(nCoords->vy);data.push_back(nCoords->vz);
                    data.push_back(tCoords->s) ;data.push_back(tCoords->t) ;
                    data.push_back(0.0f) ;data.push_back(0.0f) ;data.push_back(0.0f); // tangent place holder
                    data.push_back(0.0f) ;data.push_back(0.0f) ;data.push_back(0.0f); // bitangent place holder

                    unsigned int stride = 14;
                    unsigned int elementIndex = data.size() / stride - 1;   // number of registered vertices

                    // register the vertex in the map
                    vertexToIndex.insert({ faceVertex , elementIndex });

                    // add index in the indicies buffer
                    indices.push_back(elementIndex);

                }
                else{ // vertex registered - get the index and push it in the indidices buffer
                    
                    // get the registered value
                    unsigned int elementIndex = iter->second;   

                    // add index in the indicies buffer
                    indices.push_back(elementIndex);

                }                
            }
        }
    }

    // if has texture - also compute tangents and bitangents
    if (hasTexture){
        Compute_tangents_bitangets(data, indices);
    
    }

    // close file
    objFileStream.close();

    return;

}


// loads MTL file and saves ambient, diffuse, specular and shininess paramaters
// Returns: path to a texture image
void LoadMTLFile(std::string &mtlFilePath, glm::vec3 &Ka, glm::vec3 &Kd, 
                        glm::vec3 &Ks, float* shininess, 
                        std::string &diffuseTexturePath, std::string &normalTexturePath){ //return values

    // Open .mtl
    std::ifstream objFileStream(mtlFilePath);

    // Check if file is open
    if (!objFileStream.is_open()){
        std::cerr << "Error: The file could not be opened!" << std::endl;
        exit(-3);
    }

    // alloc variables
    std::string token;
    std::string texturePath;

    float defaultShininess = 0.0f; // if no shininess is parsed, default is applied

    // parse the MTL file
    while(objFileStream >> token){

        // shininess
        if (token == "Ns"){
            objFileStream >> token;
            defaultShininess = std::stof(token);
        }

        // normal texture file
        if (token == "map_Bump"){
            objFileStream >> normalTexturePath;
            std::string directoryPath = mtlFilePath.substr(0, mtlFilePath.find_last_of("/") + 1);
            normalTexturePath = directoryPath + normalTexturePath;
        }

        // diffuse texture file
        if (token == "map_Kd"){
            objFileStream >> diffuseTexturePath;
            std::string directoryPath = mtlFilePath.substr(0, mtlFilePath.find_last_of("/") + 1);
            diffuseTexturePath = directoryPath + diffuseTexturePath;
        }

        // ambient
        if (token == "Ka"){
            objFileStream >> token; Ka[0] = std::stof(token);
            objFileStream >> token; Ka[1] = std::stof(token);
            objFileStream >> token; Ka[2] = std::stof(token);
        }

        // diffuse
        if (token == "Kd"){
            objFileStream >> token; Kd[0] = std::stof(token);
            objFileStream >> token; Kd[1] = std::stof(token);
            objFileStream >> token; Kd[2] = std::stof(token);
        }

        // specular
        if (token == "Ks"){
            objFileStream >> token; Ks[0] = std::stof(token);
            objFileStream >> token; Ks[1] = std::stof(token);
            objFileStream >> token; Ks[2] = std::stof(token);
        }

    }

    *shininess = defaultShininess; // apply default or parsed value

    objFileStream.close();

    return;

}

void ObjectParser::Load_Textures(std::string &mtlFilePath, int *width, int *height, 
                                glm::vec3 &Ka, glm::vec3 &Kd, glm::vec3 &Ks, float *shininess, 
            std::vector<uint8_t> &diffuseTextureData, std::vector<uint8_t> &normalTextureData){

    if (mtlFilePath.substr(mtlFilePath.find_last_of(".") + 1) != "mtl"){    
        fprintf(stderr, "Texture is not a .mtl file!\n");
        exit(-1);
    }
    
    std::string diffuseTexturePath, normalTexturePath;

                                // parse the MTL file
    LoadMTLFile(mtlFilePath, Ka, Kd, Ks, shininess, diffuseTexturePath, normalTexturePath);

                                // load ppm texture (.ppm is assumed)

    // load the image data and width and height
    PPM nTexture = PPM(normalTexturePath);
    nTexture.flipUD();
    nTexture.flipLR();
    normalTextureData = nTexture.pixelData();

    // load the image data and width and height
    PPM dTexture = PPM(diffuseTexturePath);
    dTexture.flipUD();
    dTexture.flipLR();
    diffuseTextureData = dTexture.pixelData();

    // set the width and height of the image
    *width = dTexture.getWidth();
    *height = dTexture.getHeight();

    return;
}


// function that computes an average of tangets and bitangents for each vertex
void ObjectParser::Compute_tangents_bitangets(std::vector<GLfloat> &data, std::vector<GLuint> &indices){

    unsigned int numberOfElements = indices.size(); // number of faces
    unsigned int stride = 14;

    unsigned int numberOfVertices = data.size() / stride; // number of vertices
    std::vector<unsigned int> N(numberOfVertices, 0); // number of tangetns per vertex

    for (unsigned int face = 0; face < numberOfElements; face += 3){

        // get indices of a face
        unsigned int i1 = indices[face];
        unsigned int i2 = indices[face + 1];
        unsigned int i3 = indices[face + 2];

        // get vertex coords    
        glm::vec3 pos1 = glm::vec3(data[i1*stride + 0], data[i1*stride + 1], data[i1*stride + 2]);
        glm::vec3 pos2 = glm::vec3(data[i2*stride + 0], data[i2*stride + 1], data[i2*stride + 2]);
        glm::vec3 pos3 = glm::vec3(data[i3*stride + 0], data[i3*stride + 1], data[i3*stride + 2]);

        // get texture coords
        glm::vec2 uv1 = glm::vec2(data[i1*stride + 6], data[i1*stride + 7]);
        glm::vec2 uv2 = glm::vec2(data[i2*stride + 6], data[i2*stride + 7]);
        glm::vec2 uv3 = glm::vec2(data[i3*stride + 6], data[i3*stride + 7]);

        // get edge vectors
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;  

        // compute tangent
        glm::vec3 tangent;

        float invDet = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent.x = invDet * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = invDet * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = invDet * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        // increment number of tangents per vertex
        N[i1] += 1;
        N[i2] += 1;
        N[i3] += 1;

        // add tangent to the data array (and incrementally average them)
        data[i1*stride + 8] = (data[i1*stride + 8]*(N[i1]-1) + tangent.x) / N[i1];
        data[i1*stride + 9] = (data[i1*stride + 9]*(N[i1]-1) + tangent.y) / N[i1];
        data[i1*stride + 10] = (data[i1*stride + 10]*(N[i1]-1) + tangent.z) / N[i1];
        
        data[i2*stride + 8] = (data[i2*stride + 8]*(N[i2]-1) + tangent.x) / N[i2];
        data[i2*stride + 9] = (data[i2*stride + 9]*(N[i2]-1) + tangent.y) / N[i2];
        data[i2*stride + 10] = (data[i2*stride + 10]*(N[i2]-1) + tangent.z) / N[i2];

        data[i3*stride + 8] = (data[i3*stride + 8]*(N[i3]-1) + tangent.x) / N[i3];
        data[i3*stride + 9] = (data[i3*stride + 9]*(N[i3]-1) + tangent.y) / N[i3];
        data[i3*stride + 10] = (data[i3*stride + 10]*(N[i3]-1) + tangent.z) / N[i3];

    }

                    // orthogonalize and normalize the vectors (using Gramm-schmidt)

    for (unsigned int i = 0; i < numberOfVertices; i++){

        // get tangent and bitangent
        glm::vec3 normal    = glm::vec3(data[i*stride + 3], data[i*stride + 4], data[i*stride + 5]); 
        glm::vec3 tangent   = glm::vec3(data[i*stride + 8], data[i*stride + 9], data[i*stride + 10]);

        // normalize the normal
        normal = glm::normalize(normal);

        // orthogonalize tangent
        tangent = glm::normalize(tangent - normal * glm::dot(normal, tangent));

        // compute bitangent
        glm::vec3 bitangent = glm::normalize(glm::cross(normal, tangent));

        // save the final normal, tangent and bitangent
        data[i*stride + 3] = normal.x; data[i*stride + 4] = normal.y; data[i*stride + 5] = normal.z;
        data[i*stride + 8] = tangent.x; data[i*stride + 9] = tangent.y; data[i*stride + 10] = tangent.z;
        data[i*stride + 11] = bitangent.x; data[i*stride + 12] = bitangent.y; data[i*stride + 13] = bitangent.z;

    }

}




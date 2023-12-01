#include "Shader.hpp"
#include <string>
#include <fstream>
#include <iostream>

Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource){
    
    std::string vertexShaderString      = LoadShaderAsString(vertexShaderSource);
    std::string fragmentShaderString    = LoadShaderAsString(fragmentShaderSource);
    
    this->shaderID = CreateShaderProgram(vertexShaderString, fragmentShaderString);
}

Shader::~Shader(){
    // delete pipeline
    glDeleteProgram(this->shaderID);

}

void Shader::Bind(){
    glUseProgram(this->shaderID);
}

void Shader::Unbind(){
    glUseProgram(0);
}

/**
* LoadShaderAsString takes a filepath as an argument and will read line by line a file and return a string that is meant to be compiled at runtime for a vertex, fragment, geometry, tesselation, or compute shader.
* e.g.
*       LoadShaderAsString("./shaders/filepath");
*
* @param filename Path to the shader file
* @return Entire file stored as a single string 
*/
std::string Shader::LoadShaderAsString(const std::string& filename){
    // Resulting shader program loaded as a single string
    std::string result = "";

    std::string line = "";
    std::ifstream myFile(filename.c_str());

    if(myFile.is_open()){
        while(std::getline(myFile, line)){
            result += line + '\n';
        }
        myFile.close();

    }

    return result;
}


/**
* CompileShader will compile any valid vertex, fragment, geometry, tesselation, or compute shader.
* e.g.
*	    Compile a vertex shader: 	CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
*       Compile a fragment shader: 	CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
*
* @param type We use the 'type' field to determine which shader we are going to compile.
* @param source : The shader source code.
* @return id of the shaderObject
*/
GLuint Shader::CompileShader(GLuint type, const std::string& source){
	// Compile our shaders
	GLuint shaderObject;

	// Based on the type passed in, we create a shader object specifically for that
	// type.
	if(type == GL_VERTEX_SHADER){
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	}else if(type == GL_FRAGMENT_SHADER){
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	}

	const char* src = source.c_str();
	// The source of our shader
	glShaderSource(shaderObject, 1, &src, nullptr);
	// Now compile our shader
	glCompileShader(shaderObject);

	// Retrieve the result of our compilation
	int result;
	// Our goal with glGetShaderiv is to retrieve the compilation status
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

	if(result == GL_FALSE){
		int length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		char* errorMessages = new char[length]; // Could also use alloca here.
		glGetShaderInfoLog(shaderObject, length, &length, errorMessages);

		if(type == GL_VERTEX_SHADER){
			std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n" << errorMessages << "\n";
		}else if(type == GL_FRAGMENT_SHADER){
			std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n" << errorMessages << "\n";
		}
		// Reclaim our memory
		delete[] errorMessages;

		// Delete our broken shader
		glDeleteShader(shaderObject);

		return 0;
	}

  return shaderObject;
}

/**
* Creates a graphics program object (i.e. graphics pipeline) with a Vertex Shader and a Fragment Shader
*
* @param vertexShaderSource Vertex source code as a string
* @param fragmentShaderSource Fragment shader source code as a string
* @return id of the program Object
*/
GLuint Shader::CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource){

    // Create a new program object
    GLuint programObject = glCreateProgram();

    // Compile our shaders
    GLuint myVertexShader   = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Link our two shader programs together.
	// Consider this the equivalent of taking two .cpp files, and linking them into
	// one executable file.
    glAttachShader(programObject,myVertexShader);
    glAttachShader(programObject,myFragmentShader);
    glLinkProgram(programObject);

    // Validate our program
    glValidateProgram(programObject);

    // Once our final program Object has been created, we can
	// detach and then delete our individual shaders.
    glDetachShader(programObject,myVertexShader);
    glDetachShader(programObject,myFragmentShader);
	// Delete the individual shaders once we are done
    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    return programObject;
}

void Shader::Upload_Uniform1i_Pipeline(const char * name, int elementValue){
    
    // Upload lights structs 
    GLuint elementLocation = glGetUniformLocation( shaderID, name);
    if(elementLocation >=0){
        glUniform1i(elementLocation, elementValue);

    }else{
        std::cout << "Could not find \"" << name << "\" maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }
}

void Shader::Upload_Uniform1f_Pipeline(const char * name, float elementValue){
    
    // Upload lights structs 
    GLuint elementLocation = glGetUniformLocation( shaderID, name);
    if(elementLocation >=0){
        glUniform1f(elementLocation, elementValue);

    }else{
        std::cout << "Could not find \"" << name << "\" maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }
}

void Shader::Upload_Uniform3f_Pipeline(const char * name, float ev1, float ev2, float ev3){
    
    // Upload lights structs 
    GLuint elementLocation = glGetUniformLocation( shaderID, name);
    if(elementLocation >=0){
        glUniform3f(elementLocation, ev1, ev2, ev3);

    }else{
        std::cout << "Could not find \"" << name << "\" maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

}

void Shader::Upload_Uniform_MAT4fv_Pipeline(const char * name, glm::mat4 &matrix){

    GLint matrixLocation = glGetUniformLocation( shaderID, name);
    if(matrixLocation >=0){
        glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, &matrix[0][0]);
    }else{
        std::cout << "Could not find \"" << name << "\" maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

}

void Shader::Upload_Uniform_MAT3fv_Pipeline(const char * name, glm::mat3 &matrix){

    GLint matrixLocation = glGetUniformLocation( shaderID, name);
    if(matrixLocation >=0){
        glUniformMatrix3fv(matrixLocation, 1, GL_FALSE, &matrix[0][0]);
    }else{
        std::cout << "Could not find \"" << name << "\" maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

}

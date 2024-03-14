#include<iostream>
#include<fstream>
#include<sstream>
#include "Utils/test.hpp"
#include<glad/glad.h>
#include<GLFW/glfw3.h>


#include <filesystem>
using namespace std;
string LoadShaderCode(const string& shader_file_name) {

	namespace fs = std::filesystem;
	fs::path ShaderPath = fs::path(RESOURCES_DIR) / shader_file_name;
	ifstream shaderFile(ShaderPath, ios::binary);
	
	if (shaderFile.is_open()) {
		stringstream shaderData;
		shaderData << shaderFile.rdbuf();
		string shaderSource = shaderData.str();
		shaderFile.close();

		return shaderSource;
	}
	
	
	
	cerr << "Failed open shader file" << endl;
	
	throw runtime_error("Failed open shader file");
		
	

	
	
}
namespace Window {

	int CreateWindow() {
		

		if (!glfwInit()) {

			cerr << "Failed to initialize GLFW" << endl;
			throw runtime_error("Failed to initialize GLFW");
			return -1;
		}
		glfwWindowHint(GLFW_RESIZABLE, 1);
		GLFWwindow* window = glfwCreateWindow(800, 500, "VoxelEngine", nullptr, nullptr);
		if (!window) {
			cerr << "Failed to initialize Window" << endl;
			throw runtime_error("Failed to initialize Window");
			return -1;
		}
		glfwMakeContextCurrent(window);
		
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			cerr << "Failed to initialize GLAD" << endl;
			throw runtime_error("Failed to initialize GLAD");
		}
		// VAO
		GLuint VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		GLshort indices[] = {
			0, 1, 2,
			0, 2, 3
		};
		static const GLfloat verticises[] = {
			-1.0f, -1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			1.0f,  1.0f, 0.0f,
			1.0f,  -1.0f, 0.0f
		};

		// VBO
		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticises), verticises, GL_STATIC_DRAW);

		//IBO
		GLuint ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(0);
		
		

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glVertexAttribPointer(
			0,                 
			3,                  // Размер
			GL_FLOAT,           // Тип
			GL_FALSE,           // Указывает, что значения не нормализованы
			0,                  // Шаг
			(void*)0            // Смещение массива в буфере
		);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);


		// SHADERS

		string vertexShaderSource;
		string fragmentShaderSource;
		
		

		fragmentShaderSource = LoadShaderCode("fragment.glsl");
		vertexShaderSource = LoadShaderCode("vertex.glsl");
		
		const char* fragmentShaderSourcePtr = fragmentShaderSource.data();
		const char* vertexShaderSourcePtr = vertexShaderSource.data();

		GLuint vertexShader, fragmentShader;

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSourcePtr, NULL);
		glCompileShader(vertexShader);

		int success;
		char infolog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		
		
		if (!success) {

			glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
			
			cerr << "ERROR VERTEX SHADER COMPILE FAILED:\n" << infolog << endl;
			cout << vertexShaderSource << endl;
			throw runtime_error("ERROR VERTEX SHADER COMPILE FAILED:\n");

		}

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSourcePtr, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		
		
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
			cerr << "ERROR FRAGMENT SHADER COMPILE FAILED:\n" << infolog << endl;
			throw runtime_error("ERROR FRAGMENT SHADER COMPILE FAILED:\n");
			
		}

		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		

		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
			cerr << "SHADER PROGRAM LINKED FAILED:\n" << infolog << endl;
			throw runtime_error("SHADER PROGRAM LINKED FAILED");
			

		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);


		glUseProgram(shaderProgram);

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {

			glViewport(0, 0, (GLsizei)width, (GLsizei)height);
		});
		
		while (!glfwWindowShouldClose(window)) {
			
			
			
			
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

			

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		glfwTerminate();

		return 0;



	}
}

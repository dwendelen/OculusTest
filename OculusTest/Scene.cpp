#include "Scene.h"

using namespace OVR;
GLuint loadProgram();
Scene::Scene()
{
}

void Scene::init() {
	program = loadProgram();

	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.3f, -1.0f,
		0.5f, -0.3f, -1.0f,
		0.0f, 0.3f, -1.0f,
	};

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

}

void Scene::render(Matrix4f pv) {
	glUseProgram(program);
	
	GLuint matrixIndex = glGetProgramResourceLocation(program, GL_UNIFORM, "pvm");
	glUniformMatrix4fv(matrixIndex, 1, GL_TRUE, (float*)&pv);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

Scene::~Scene()
{
}

GLuint loadProgram() {
	char* vertexShader = "\
		#version 330 core\n\
		layout(location = 0) in vec3 vertex;\n\
		uniform mat4 pvm;\n\
		\n\
		void main() {\n\
			gl_Position = pvm * vec4(vertex, 1);\n\
		}";
	char* fragmentShader = "\
		#version 330 core\n\
		out vec3 color;\n\
		void main() {\n\
			color = vec3(1, 1, 1);\n\
		}";

	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	char errorMsg[2048];

	glShaderSource(VertexShaderID, 1, &vertexShader, NULL);
	glCompileShader(VertexShaderID);

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderInfoLog(VertexShaderID, 2048, NULL, errorMsg);
	printf("Vertex problem: %s\n", errorMsg);
	

	glShaderSource(FragmentShaderID, 1, &fragmentShader, NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderInfoLog(FragmentShaderID, 2048, NULL, errorMsg);
	printf("Fragment problem: %s\n", errorMsg);

	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
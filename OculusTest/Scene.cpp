#include "Scene.h"

using namespace OVR;
GLuint loadProgram();
Scene::Scene()
{
}

void Scene::init() {
	orientation = Quatf(0.0f, 0.0f, 0.0f, 1.0f);
	program = loadProgram();

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	legoBrick = new LegoBrick();
	legoBrick->init();

	vector<Vector3f>* verticesAndNormal = legoBrick->getVertices();
	glGenBuffers(1, &legoBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, legoBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * verticesAndNormal->size(), verticesAndNormal->data(), GL_STATIC_DRAW);

	vector<Vector3ui>* indices = legoBrick->getIndices();
	glGenBuffers(1, &legoElements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, legoElements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3ui) * indices->size(), indices->data(), GL_STATIC_DRAW);
}

void Scene::rotate(Quatf rotation)
{
	orientation = rotation * orientation;
	if (!orientation.IsNormalized()) {
		orientation.Normalize();
	}
}

void Scene::render(Matrix4f pv) {
	glUseProgram(program);
	GLuint matrixIndex = glGetProgramResourceLocation(program, GL_UNIFORM, "pvm");
	GLuint rotIndex = glGetProgramResourceLocation(program, GL_UNIFORM, "rot");

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	Matrix4f pvm = pv * Matrix4f(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, -0.2f,
		0, 0, 0, 1);
	Matrix4f rotation(orientation);
	pvm = pvm * rotation;
	glUniformMatrix4fv(matrixIndex, 1, GL_TRUE, (float*)&pvm);
	glUniformMatrix4fv(rotIndex, 1, GL_TRUE, (float*)&rotation);

	glBindBuffer(GL_ARRAY_BUFFER, legoBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, legoElements);
	glDrawElements(GL_TRIANGLES, legoBrick->getIndices()->size() * 3, GL_UNSIGNED_INT, (void*)0);


	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

}

Scene::~Scene()
{
	glDeleteBuffers(1, &legoBuffer);
	glDeleteBuffers(1, &legoElements);
	glDeleteVertexArrays(1, &vertexArray);

	delete legoBrick;
}

GLuint loadProgram() {
	char* vertexShader = "\
		#version 330 core\n\
		layout(location = 0) in vec3 vertex;\n\
		layout(location = 1) in vec3 nml;\n\
		out vec3 normalO;\n\
		uniform mat4 pvm;\n\
		uniform mat4 rot;\n\
		void main() {\n\
			gl_Position = pvm * vec4(vertex, 1);\n\
			normalO = vec3(rot * vec4(nml, 1));\n\
		}";
	char* fragmentShader = "\
		#version 330 core\n\
		out vec3 color;\n\
		in vec3 normalO;\n\
		void main() {\n\
			color = (0.3 + (1.0+normalO.x)*0.25) * vec3(1, 1, 1);\n\
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
#include "Scene.h"
#include "LegoBrick.h"
#include "ModelInstance.h"
#include "MemoryManager.h"

using namespace OVR;

GLuint loadProgram();
Scene::Scene(MemoryManager& memoryManager):
    memoryManager(memoryManager)
{
}

void Scene::init() {
	orientation = Quatf(0.0f, 0.0f, 0.0f, 1.0f);
    program = loadProgram();

	legoBrick.reset(new LegoBrick());
	legoBrick->init();
	model.reset(new ModelInstance(*legoBrick, Vector3f(0.0f, 0.0f, -0.2f), orientation));
    memoryManager.load(*legoBrick);
}

void Scene::rotate(Quatf rotation)
{
	model->rotate(rotation);
}

void Scene::enableWireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
}

void Scene::disableWireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
}

void Scene::render(Matrix4f pv) {
	glUseProgram(program);
	GLuint matrixIndex = glGetUniformLocation(program, "pvm");
	GLuint rotIndex = glGetUniformLocation(program, "rot");

	Matrix4f pvm = pv * model->getModelMatrix();
    Matrix4f rot = model->getRotationMatrix();
	glUniformMatrix4fv(matrixIndex, 1, GL_TRUE, (float*)&pvm);
	glUniformMatrix4fv(rotIndex, 1, GL_TRUE, (float*)&rot);


    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    memoryManager.bindModel();
	glDrawElements(GL_TRIANGLES, legoBrick->getIndices().size() * 3, GL_UNSIGNED_INT, (void*)0);
	glDrawElements(GL_LINES, legoBrick->getIndices().size() * 6, GL_UNSIGNED_INT, (void*)0);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

Scene::~Scene()
{

}

GLuint loadProgram()
{
	const char* vertexShader = "\
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
	const char* fragmentShader = "\
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
	GLsizei l;

	glShaderSource(VertexShaderID, 1, &vertexShader, NULL);
	glCompileShader(VertexShaderID);
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderInfoLog(VertexShaderID, 2048, &l, errorMsg);
	printf("Vertex problem: %s\n", errorMsg);


	glShaderSource(FragmentShaderID, 1, &fragmentShader, NULL);
	glCompileShader(FragmentShaderID);
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderInfoLog(FragmentShaderID, 2048, &l, errorMsg);
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

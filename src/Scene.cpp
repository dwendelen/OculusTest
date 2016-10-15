#include "Scene.h"
#include "LegoBrick.h"
#include "ModelInstance.h"
#include "MemoryManager.h"
#include <string>
#include <fstream>

using namespace OVR;
using namespace memory;
using namespace std;

GLuint loadProgram();

Scene::Scene(MemoryManager& memoryManager):
    memoryManager(memoryManager),
    colorIndex(0)
{
}

const vector<Vector3f> colors = {
    Vector3f(1.0f, 1.0f, 1.0f),
    Vector3f(1.0f, 0.0f, 0.0f),
    Vector3f(0.0f, 1.0f, 0.0f),
    Vector3f(0.0f, 0.0f, 1.0f)
};

void Scene::init() {
	orientation = Quatf(0.0f, 0.0f, 0.0f, 1.0f);
    program = loadProgram();

	legoBrick.reset(new LegoBrick());
	legoBrick->init();
	model.reset(new ModelInstance(*legoBrick, Vector3f(0.0f, 0.0f, -0.2f), orientation));
    model->setColor(colors[0]);
    memoryManager.load(*legoBrick);
}

void Scene::rotate(Quatf rotation)
{
	//model->rotate(rotation);
}

void Scene::enableWireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
}

void Scene::disableWireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
}

void renderr(const ModelInstance& renderedModel, const Matrix4f& pv, int nbOfIndices, float alpha, MemoryManager& memoryManager) {
    Matrix4f pvm = pv * renderedModel.getModelMatrix();
    Matrix4f rot = renderedModel.getRotationMatrix();

    memoryManager.setUniform(pvm, rot, Vector4f(renderedModel.getColor(), alpha));

    glDrawElements(GL_TRIANGLES, nbOfIndices, GL_UNSIGNED_INT, (void*)0);
}

void Scene::render(Matrix4f pv) {
	glUseProgram(program);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    memoryManager.bindModel();

	int nbOfIndices = legoBrick->getIndices().size() * 3;

	for(ModelInstance renderedModel: placedBlocks) {
        renderr(renderedModel, pv, nbOfIndices, 1, memoryManager);
    }

    renderr(*model, pv, nbOfIndices, 0, memoryManager);
    renderr(*model, pv, nbOfIndices, 0.5f, memoryManager);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

void Scene::move(Vector3f translation){
    model->move(translation);
}
void Scene::moveTo(Vector3f position) {}

void Scene::changeColor() {
    colorIndex = (colorIndex + 1) % colors.size();
    model->setColor(colors[colorIndex]);
}

void Scene::place() {
    placedBlocks.push_back(*model);
}

Scene::~Scene()
{

}

string loadFile(string filename)
{
    ifstream t(filename);
    if(t.bad() || t.fail()) {
        throw new runtime_error("Could not read " + filename);
    }
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);
    t.close();
    return buffer;
}

GLuint loadProgram()
{
    string vertexShader = loadFile("shaders/vert.vert");
    string fragmentShader = loadFile("shaders/frag.frag");

    const char * pVertexShader = vertexShader.c_str();
    const char * pFragmentShader = fragmentShader.c_str();

	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	char errorMsg[2048];
	GLsizei l;

	glShaderSource(VertexShaderID, 1, &pVertexShader, NULL);
	glCompileShader(VertexShaderID);
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderInfoLog(VertexShaderID, 2048, &l, errorMsg);
	printf("Vertex problem: %s\n", errorMsg);


	glShaderSource(FragmentShaderID, 1, &pFragmentShader, NULL);
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

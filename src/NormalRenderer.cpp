#include "NormalRenderer.h"

#include "Scene.h"
#include "ModelInstance.h"
#include "MemoryManager.h"
#include "Model.h"

#include <string>
#include <fstream>

using namespace OVR;
using namespace memory;
using namespace std;

namespace video
{
	GLuint loadProgram();

	NormalRenderer::NormalRenderer(MemoryManager& memoryManager, Model& model) :
		memoryManager(memoryManager),
		model(model)
	{
	}

	void NormalRenderer::init()
	{
		program = loadProgram();
	}

	void renderr(const ModelInstance& renderedModel, const Matrix4f& pv, int nbOfIndices, float alpha, MemoryManager& memoryManager) {
		Matrix4f pvm = pv * renderedModel.getModelMatrix();
		Matrix4f rot = renderedModel.getRotationMatrix();

		memoryManager.setUniform(pvm, rot, Vector4f(renderedModel.getColor(), alpha));

		glDrawElements(GL_TRIANGLES, nbOfIndices, GL_UNSIGNED_INT, (void*)0);
	}

	void NormalRenderer::renderScene(Scene& scene, Matrix4f pv)
	{
		glUseProgram(program);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		GLuint matricesIndex = glGetUniformBlockIndex(program, "Matrices");
		glUniformBlockBinding(program, matricesIndex, 0);
		GLuint colorIndices = glGetUniformBlockIndex(program, "ColorUniform");
		glUniformBlockBinding(program, colorIndices, 1);


		memoryManager.bindModel();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		int nbOfIndices = model.getIndices().size() * 3;

		for (ModelInstance renderedModel : scene.getPlacedBlocks()) {
			renderr(renderedModel, pv, nbOfIndices, 1, memoryManager);
		}

		renderr(*scene.getCurrentBlock(), pv, nbOfIndices, 0, memoryManager);
		renderr(*scene.getCurrentBlock(), pv, nbOfIndices, 0.5f, memoryManager);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
	}
	NormalRenderer::~NormalRenderer()
	{

	}
	string loadFile(string filename)
	{
		ifstream t(filename);
		if (t.bad() || t.fail()) {
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
		string vertexShader = loadFile("shaders/vert_150.vert");
		string fragmentShader = loadFile("shaders/frag_150.frag");

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

}
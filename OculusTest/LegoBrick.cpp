#include "LegoBrick.h"
#define _USE_MATH_DEFINES
#include <math.h>

//using namespace OVR;
//using namespace std;

LegoBrick::LegoBrick()
{
}

void addCilinder(int nbOfSegments, Vector3f center, float radius, float height, vector<Vector3f>& vertices, vector<Vector3ui>& indices) {
	GLuint firstIndex = vertices.size() / 2;

	float y0 = center.y;
	float y1 = y0 + height;
	vertices.push_back(Vector3f(center.x, y1, center.z));
	vertices.push_back(Vector3f(0, 1.0f, 0));
	
	double angleDelta = 2 * M_PI / nbOfSegments;
	double angle = 0.0f;
	for (int i = 0; i < nbOfSegments; i++) {
		Vector3f normal(cos(angle), 0, sin(angle));

		float x = center.x + radius * normal.x;
		float z = center.z + radius * normal.z;

		vertices.push_back(Vector3f(x, y1, z));
		vertices.push_back(normal);
		vertices.push_back(Vector3f(x, y0, z));
		vertices.push_back(normal);

		angle += angleDelta;
	}

	GLuint topRightIndex = firstIndex + 1;
	for (int i = 0; i < nbOfSegments - 1; i++) {
		indices.push_back(Vector3ui(firstIndex, topRightIndex + 2, topRightIndex + 0));
		indices.push_back(Vector3ui(topRightIndex + 0, topRightIndex + 2, topRightIndex + 1));
		indices.push_back(Vector3ui(topRightIndex + 2, topRightIndex + 3, topRightIndex + 1));
		topRightIndex += 2;
	}
	indices.push_back(Vector3ui(firstIndex, firstIndex + 1, topRightIndex + 0));
	indices.push_back(Vector3ui(topRightIndex + 0, firstIndex + 1, topRightIndex + 1));
	indices.push_back(Vector3ui(firstIndex + 1, firstIndex + 2, topRightIndex + 1));
}

void LegoBrick::init() {
	float widthPerStub =	0.008f;
	float heightBlock =		0.0096f;
	float edgePenalty =		0.0001f;
	float stubRadius =		0.0024f;
	float stubHeight =		0.0018f;

	float x = (2 * widthPerStub - edgePenalty);
	float y = heightBlock;
	float z = widthPerStub - edgePenalty;

    float xInner = x - 1.2f;
	float yInner = y - 1.0f;
	float zInner = z - 1.2f;

	vertices = {
		//Front
		Vector3f(-x, y,  z), Vector3f(0, 0,  1.0f), //0
		Vector3f( x, y,  z), Vector3f(0, 0,  1.0f),
		Vector3f( x, 0,  z), Vector3f(0, 0,  1.0f),
		Vector3f(-x, 0,  z), Vector3f(0, 0,  1.0f),
		//Back
		Vector3f(-x, y, -z), Vector3f(0, 0, -1.0f), //4
		Vector3f( x, y, -z), Vector3f(0, 0, -1.0f),
		Vector3f( x, 0, -z), Vector3f(0, 0, -1.0f),
		Vector3f(-x, 0, -z), Vector3f(0, 0, -1.0f),
		//Left
		Vector3f(-x, y, -z), Vector3f(-1.0f, 0, 0), //8
		Vector3f(-x, y,  z), Vector3f(-1.0f, 0, 0),
		Vector3f(-x, 0,  z), Vector3f(-1.0f, 0, 0),
		Vector3f(-x, 0, -z), Vector3f(-1.0f, 0, 0),
		//Top
		Vector3f(-x, y, -z), Vector3f(0,  1.0f, 0), //12
		Vector3f( x, y, -z), Vector3f(0,  1.0f, 0),
		Vector3f( x, y,  z), Vector3f(0,  1.0f, 0),
		Vector3f(-x, y,  z), Vector3f(0,  1.0f, 0),
		//Right
		Vector3f( x, y,  z), Vector3f( 1.0f, 0, 0), //16
		Vector3f( x, y, -z), Vector3f( 1.0f, 0, 0),
		Vector3f( x, 0, -z), Vector3f( 1.0f, 0, 0),
		Vector3f( x, 0,  z), Vector3f( 1.0f, 0, 0),
		//Bottom
		Vector3f(-x, 0,  z), Vector3f(0, -1.0f, 0), //20
		Vector3f( x, 0,  z), Vector3f(0, -1.0f, 0),
		Vector3f( x, 0, -z), Vector3f(0, -1.0f, 0),
		Vector3f(-x, 0, -z), Vector3f(0, -1.0f, 0),
	};
	
	indices = {
		Vector3ui( 0,  2,  1),//front
		Vector3ui( 0,  3,  2),
		Vector3ui( 4,  6,  5),//back
		Vector3ui( 4,  7,  6),
		Vector3ui( 8, 10,  9),//left
		Vector3ui( 8, 11, 10),
		Vector3ui(12, 14, 13),//top
		Vector3ui(12, 15, 14),
		Vector3ui(16, 18, 17),//right
		Vector3ui(16, 19, 18),
		Vector3ui(20, 22, 21),//bottom
		Vector3ui(20, 23, 22)
	};

//	//Seen from bottom
//	vertices2 = {
//        Vector3f(-xInner, yInner,  zInner),	//0
//        Vector3f( xInner, yInner,  zInner),	//1
//        Vector3f( xInner, 0     ,  zInner), //2
//        Vector3f(-xInner, 0     ,  zInner), //3
//        Vector3f(-xInner, yInner, -zInner), //4
//        Vector3f( xInner, yInner, -zInner),	//5
//        Vector3f( xInner, 0     , -zInner),	//6
//        Vector3f(-xInner, 0     , -zInner)	//7
//    };

	       z =  0.004f;
	float x1 = 0.004f;
	float x2 = 0.012f;

	addCilinder(16, Vector3f(-x2, y, -z), stubRadius, stubHeight, vertices, indices);
	addCilinder(16, Vector3f(-x1, y, -z), stubRadius, stubHeight, vertices, indices);
	addCilinder(16, Vector3f( x1, y, -z), stubRadius, stubHeight, vertices, indices);
	addCilinder(16, Vector3f( x2, y, -z), stubRadius, stubHeight, vertices, indices);
	addCilinder(16, Vector3f(-x2, y,  z), stubRadius, stubHeight, vertices, indices);
	addCilinder(16, Vector3f(-x1, y,  z), stubRadius, stubHeight, vertices, indices);
	addCilinder(16, Vector3f( x1, y,  z), stubRadius, stubHeight, vertices, indices);
	addCilinder(16, Vector3f( x2, y,  z), stubRadius, stubHeight, vertices, indices);
}

LegoBrick::~LegoBrick()
{
}

#include "LegoBrick.h"
#define _USE_MATH_DEFINES
#include <math.h>

//using namespace OVR;
//using namespace std;

LegoBrick::LegoBrick()
{
}

void addCilinder(int nbOfSegments, Vector3f center, float radius, float height, vector<Vector3f>& vertices, vector<Vector3ui>& indices) {
	GLuint firstIndex = vertices.size();

	float y0 = center.y;
	float y1 = y0 + height;
	vertices.push_back(Vector3f(center.x, y1, center.y));
	
	double angleDelta = 2 * M_PI / nbOfSegments;
	double angle = 0.0f;
	for (int i = 0; i < nbOfSegments; i++) {
		float x = center.x + radius * cos(angle);
		float z = center.z + radius * sin(angle);
		
		vertices.push_back(Vector3f(x, y1, z));
		vertices.push_back(Vector3f(x, y0, z));
		angle += angleDelta;
	}

	GLuint topRightIndex = firstIndex + 1;
	for (int i = 0; i < nbOfSegments - 1; i++) {
		indices.push_back(Vector3ui(firstIndex, topRightIndex + 0, topRightIndex + 2));
		indices.push_back(Vector3ui(topRightIndex + 0, topRightIndex + 1, topRightIndex + 2));
		indices.push_back(Vector3ui(topRightIndex + 2, topRightIndex + 1, topRightIndex + 3));
		topRightIndex += 2;
	}
	indices.push_back(Vector3ui(firstIndex, topRightIndex + 0, firstIndex + 1));
	indices.push_back(Vector3ui(topRightIndex + 0, topRightIndex + 1, firstIndex + 1));
	indices.push_back(Vector3ui(firstIndex, topRightIndex + 1, firstIndex + 2));
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
		Vector3f(-x, y, z),	//0
		Vector3f(x, y, z),	//1
		Vector3f(x, 0, z),  //2
		Vector3f(-x, 0, z), //3
		Vector3f(-x, y, -z),//4
		Vector3f(x, y, -z),	//5
		Vector3f(x, 0, -z),	//6
		Vector3f(-x, 0, -z)	//7
	};
	
	indices = {
		Vector3ui(0, 1, 2),//front
		Vector3ui(0, 2, 3),
		Vector3ui(4, 5, 6),//back
		Vector3ui(4, 6, 7),
		Vector3ui(4, 0, 3),//left
		Vector3ui(4, 3, 7),
		Vector3ui(4, 5, 1),//top
		Vector3ui(4, 1, 0),
		Vector3ui(1, 5, 6),//right
		Vector3ui(1, 6, 2),
		Vector3ui(3, 2, 6),//bottom
		Vector3ui(3, 6, 7)
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

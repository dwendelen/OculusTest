#include "LegoBrick.h"
#define _USE_MATH_DEFINES
#include <math.h>

//using namespace OVR;
//using namespace std;

LegoBrick::LegoBrick()
{
}
void addStubs(int nbOfSegments, Vector3f center, float radius, float height, vector<Vector3f>& vertices, vector<Vector3ui>& indices);
void addHollowCilinders(int nbOfSegments, Vector3f center, float innerRadius, float outerRadius, float height, vector<Vector3f>& vertices, vector<Vector3ui>& indices);

void LegoBrick::init() {
	float widthPerStub =	0.008f;
	float heightBlock =		0.0096f;
	float edgePenalty =		0.0001f;
	float stubRadius =		0.0024f;
	float stubHeight =		0.0018f;

	float x = (2 * widthPerStub - edgePenalty);
	float y = heightBlock;
	float z = widthPerStub - edgePenalty;

    float xInner = x - 0.0012f;
	float yInner = y - 0.0010f;
	float zInner = z - 0.0012f;

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
		Vector3f(-x     , 0     ,  z     ), Vector3f(0, -1.0f, 0), //20
        Vector3f(-xInner, 0     ,  zInner), Vector3f(0, -1.0f, 0),
        Vector3f(-xInner, 0     , -zInner), Vector3f(0, -1.0f, 0),
        Vector3f(-x     , 0     ,  -z    ), Vector3f(0, -1.0f, 0),
        Vector3f( x     , 0     ,  z     ), Vector3f(0, -1.0f, 0), //24
        Vector3f( xInner, 0     ,  zInner), Vector3f(0, -1.0f, 0),
        Vector3f( xInner, 0     , -zInner), Vector3f(0, -1.0f, 0),
        Vector3f( x     , 0     ,  -z    ), Vector3f(0, -1.0f, 0),

        //Inner Front
		Vector3f(-xInner, yInner,  zInner), Vector3f(0, 0, -1.0f), //28
		Vector3f( xInner, yInner,  zInner), Vector3f(0, 0, -1.0f),
		Vector3f( xInner, 0     ,  zInner), Vector3f(0, 0, -1.0f),
		Vector3f(-xInner, 0     ,  zInner), Vector3f(0, 0, -1.0f),
		//Inner Back
		Vector3f(-xInner, yInner, -zInner), Vector3f(0, 0,  1.0f), //32
		Vector3f( xInner, yInner, -zInner), Vector3f(0, 0,  1.0f),
		Vector3f( xInner, 0     , -zInner), Vector3f(0, 0,  1.0f),
		Vector3f(-xInner, 0     , -zInner), Vector3f(0, 0,  1.0f),
		//Inner Left
		Vector3f(-xInner, yInner, -zInner), Vector3f( 1.0f, 0, 0), //36
		Vector3f(-xInner, yInner,  zInner), Vector3f( 1.0f, 0, 0),
		Vector3f(-xInner, 0     ,  zInner), Vector3f( 1.0f, 0, 0),
		Vector3f(-xInner, 0     , -zInner), Vector3f( 1.0f, 0, 0),
		//Inner Top
		Vector3f(-xInner, yInner, -zInner), Vector3f(0, -1.0f, 0), //40
		Vector3f( xInner, yInner, -zInner), Vector3f(0, -1.0f, 0),
		Vector3f( xInner, yInner,  zInner), Vector3f(0, -1.0f, 0),
		Vector3f(-xInner, yInner,  zInner), Vector3f(0, -1.0f, 0),
		//Inner Right
		Vector3f( xInner, yInner,  zInner), Vector3f(-1.0f, 0, 0), //44
		Vector3f( xInner, yInner, -zInner), Vector3f(-1.0f, 0, 0),
		Vector3f( xInner, 0     , -zInner), Vector3f(-1.0f, 0, 0),
		Vector3f( xInner, 0     ,  zInner), Vector3f(-1.0f, 0, 0),
	};

	indices = {
		Vector3ui( 0,  2,  1),//front
		Vector3ui( 0,  3,  2),
		Vector3ui( 4,  5,  6),//back
		Vector3ui( 4,  6,  7),
		Vector3ui( 8, 10,  9),//left
		Vector3ui( 8, 11, 10),
		Vector3ui(12, 14, 13),//top
		Vector3ui(12, 15, 14),
		Vector3ui(16, 18, 17),//right
		Vector3ui(16, 19, 18),

		Vector3ui(20, 22, 21),//botom left
		Vector3ui(20, 23, 22),
		Vector3ui(20, 21, 24),//botom top
		Vector3ui(21, 25, 24),
		Vector3ui(24, 25, 27),//botom right
		Vector3ui(25, 26, 27),
		Vector3ui(22, 23, 26),//botom bottom
		Vector3ui(23, 27, 26),

		Vector3ui(28, 29, 30),//inner front
		Vector3ui(28, 30, 31),
		Vector3ui(32, 34, 33),//inner back
		Vector3ui(32, 35, 34),
		Vector3ui(36, 37, 38),//inner left
		Vector3ui(36, 38, 39),
		Vector3ui(40, 41, 42),//inner top
		Vector3ui(40, 42, 43),
		Vector3ui(44, 45, 46),//inner right
		Vector3ui(44, 46, 47),
	};

//	//Seen from bottom
//	vertices2 = {
//
//    };

	       z =  0.004f;
	float x1 = 0.004f;
	float x2 = 0.012f;

	addStubs(16, Vector3f(-x2, y, -z), stubRadius, stubHeight, vertices, indices);
	addStubs(16, Vector3f(-x1, y, -z), stubRadius, stubHeight, vertices, indices);
	addStubs(16, Vector3f( x1, y, -z), stubRadius, stubHeight, vertices, indices);
	addStubs(16, Vector3f( x2, y, -z), stubRadius, stubHeight, vertices, indices);
	addStubs(16, Vector3f(-x2, y,  z), stubRadius, stubHeight, vertices, indices);
	addStubs(16, Vector3f(-x1, y,  z), stubRadius, stubHeight, vertices, indices);
	addStubs(16, Vector3f( x1, y,  z), stubRadius, stubHeight, vertices, indices);
	addStubs(16, Vector3f( x2, y,  z), stubRadius, stubHeight, vertices, indices);

	float cilx = 0.008f;
	addHollowCilinders(16, Vector3f(-cilx, 0, 0), 0.0024f, 0.003255f, yInner, vertices, indices);
	addHollowCilinders(16, Vector3f(    0, 0, 0), 0.0024f, 0.003255f, yInner, vertices, indices);
	addHollowCilinders(16, Vector3f( cilx, 0, 0), 0.0024f, 0.003255f, yInner, vertices, indices);
}

void addStubs(int nbOfSegments, Vector3f center, float radius, float height, vector<Vector3f>& vertices, vector<Vector3ui>& indices) {
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

        vertices.push_back(Vector3f(x, y1, z));
		vertices.push_back(Vector3f(0, 1.0f, 0));
		angle += angleDelta;
	}

	GLuint topRightIndex = firstIndex + 1;
	for (int i = 0; i < nbOfSegments - 1; i++) {
		indices.push_back(Vector3ui(firstIndex, topRightIndex + 5, topRightIndex + 2));
		indices.push_back(Vector3ui(topRightIndex + 0, topRightIndex + 3, topRightIndex + 1));
		indices.push_back(Vector3ui(topRightIndex + 3, topRightIndex + 4, topRightIndex + 1));
		topRightIndex += 3;
	}
	indices.push_back(Vector3ui(firstIndex, firstIndex + 3, topRightIndex + 2));
	indices.push_back(Vector3ui(topRightIndex + 0, firstIndex + 1, topRightIndex + 1));
	indices.push_back(Vector3ui(firstIndex + 1, firstIndex + 2, topRightIndex + 1));
}

void addHollowCilinders(int nbOfSegments, Vector3f center, float innerRadius, float outerRadius, float height, vector<Vector3f>& vertices, vector<Vector3ui>& indices)
{
    GLuint firstIndex = vertices.size() / 2;
	float y0 = center.y;
	float y1 = y0 + height;

	double angleDelta = 2 * M_PI / nbOfSegments;
	double angle = 0.0f;
	for (int i = 0; i < nbOfSegments; i++) {
		Vector3f normal(cos(angle), 0, sin(angle));

		float xo = center.x + outerRadius * normal.x;
        float xi = center.x + innerRadius * normal.x;

		float zo = center.z + outerRadius * normal.z;
        float zi = center.z + innerRadius * normal.z;

        vertices.push_back(Vector3f(xo, y0, zo));
		vertices.push_back(normal);
		vertices.push_back(Vector3f(xo, y1, zo));
		vertices.push_back(normal);
		vertices.push_back(Vector3f(xi, y0, zi));
		vertices.push_back(-normal);
		vertices.push_back(Vector3f(xi, y1, zi));
		vertices.push_back(-normal);

		vertices.push_back(Vector3f(xo, y0, zo));
		vertices.push_back(Vector3f(0, -1.0f, 0));
		vertices.push_back(Vector3f(xi, y0, zi));
        vertices.push_back(Vector3f(0, -1.0f, 0));

		angle += angleDelta;
	}

	GLuint lowerRight = firstIndex;
	for (int i = 0; i < nbOfSegments - 1; i++) {
		indices.push_back(Vector3ui(lowerRight +  0, lowerRight +  1, lowerRight +  6)); //outer
		indices.push_back(Vector3ui(lowerRight +  1, lowerRight +  7, lowerRight +  6));
		indices.push_back(Vector3ui(lowerRight +  2, lowerRight +  9, lowerRight +  3)); //inner
		indices.push_back(Vector3ui(lowerRight +  2, lowerRight +  8, lowerRight +  9));
		indices.push_back(Vector3ui(lowerRight + 10, lowerRight +  5, lowerRight +  4)); //bottom
		indices.push_back(Vector3ui(lowerRight + 10, lowerRight + 11, lowerRight +  5));
		lowerRight += 6;
	}
    indices.push_back(Vector3ui(lowerRight +  0, lowerRight +  1, firstIndex +  0)); //outer
    indices.push_back(Vector3ui(lowerRight +  1, firstIndex +  1, firstIndex +  0));
    indices.push_back(Vector3ui(lowerRight +  2, firstIndex +  3, lowerRight +  3)); //inner
    indices.push_back(Vector3ui(lowerRight +  2, firstIndex +  2, firstIndex +  3));
    indices.push_back(Vector3ui(firstIndex +  4, lowerRight +  5, lowerRight +  4)); //bottom
    indices.push_back(Vector3ui(firstIndex +  4, firstIndex +  5, lowerRight +  5));
}

LegoBrick::~LegoBrick()
{
}

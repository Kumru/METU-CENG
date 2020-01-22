#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "Vec4.h"

#include "Matrix4.h"

using namespace std;

#define MAX(a, b) (a>b ? a : b)
#define MIN(a, b) (a<b ? a : b)

class Scene
{
public:
	Color backgroundColor;
	bool cullingEnabled;
	int projectionType;

	vector< vector<Color> > image;
	vector< Camera* > cameras;
	vector< Vec3* > vertices;
	vector< Color* > colorsOfVertices;
	vector< Scaling* > scalings;
	vector< Rotation* > rotations;
	vector< Translation* > translations;
	vector< Model* > models;

	Scene(const char *xmlPath);

	void initializeImage(Camera* camera);
	void forwardRenderingPipeline(Camera* camera);
	int makeBetweenZeroAnd255(double value);
	void writeImageToPPMFile(Camera* camera);
	void convertPPMToPNG(string ppmFileName, int osType);

	/*  MY VARIABLES  */
	const double PI = acos(-1);
	int horRes;
	int verRes;

	/*  MY FUNCTIONS  */
	inline double toRadian(double t) { return t / 180 * PI; }

	Matrix4 GetScalingMatrix(Scaling* scale);
	Matrix4 GetTranslationMatrix(Translation* translation);
	Matrix4 GetRotationMatrix(Rotation* rotation);

	Matrix4 GetCameraTransformation(Camera* camera);
	Matrix4 GetOrthoTransformation(Camera* camera);
	Matrix4 GetP2OTransformation(Camera* camera);
	Matrix4 GetViewportTransformation(Camera* camera);

	Matrix4 GetModelTransformation(Model* model);

	void RenderAsSolid(vector<Vec4>& triVertex, vector<Color>& vertexColor);
	int PointPlaneDistance(int x0, int y0, int x1, int y1, int x, int y);

	void RenderAsWireframe(vector<Vec4>& triVertex, vector<Color>& vertexColor);
	void RasterLine(int x1, int y1, double z1, int x2, int y2, double z2, double* c1, double* c2);

	void CohenSutherlandClip(std::vector<Vec4>& vertices, std::vector<Color>& colors);
};

#endif

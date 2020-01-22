#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"

using namespace tinyxml2;
using namespace std;

Matrix4 Scene::GetCameraTransformation(Camera* camera)
{
	double vals[4][4] = {
		{camera->u.x, camera->u.y, camera->u.z, -dotProductVec3(camera->u, camera->pos)},
		{camera->v.x, camera->v.y, camera->v.z, -dotProductVec3(camera->v, camera->pos)},
		{camera->w.x, camera->w.y, camera->w.z, -dotProductVec3(camera->w, camera->pos)},
		{          0,           0,           0,                                       1}
	};

	return { vals };
	
}

Matrix4 Scene::GetOrthoTransformation(Camera* camera)
{
	double vals[4][4] = {
		{2.0 / (camera->right - camera->left), 0, 0, -(camera->right + camera->left) / (camera->right - camera->left)},
		{0, 2.0 / (camera->top - camera->bottom), 0, -(camera->top + camera->bottom) / (camera->top - camera->bottom)},
		{0, 0, -2.0 / (camera->far - camera->near), -(camera->far + camera->near) / (camera->far - camera->near)},
		{0 , 0, 0, 1}
	};

	return vals;
}

Matrix4 Scene::GetP2OTransformation(Camera* camera)
{
	Matrix4 MPer(getIdentityMatrix());
	if (projectionType == 1)
	{
		MPer.val[0][0] = camera->near;
		MPer.val[1][1] = camera->near;
		MPer.val[2][2] = camera->far + camera->near;
		MPer.val[3][3] = 0;

		MPer.val[2][3] = camera->far * camera->near;
		MPer.val[3][2] = -1;
	}

	return MPer;
}

Matrix4 Scene::GetViewportTransformation(Camera* camera)
{
	Matrix4 Mvp;
	Mvp.val[0][0] = camera->horRes / 2.0;
	Mvp.val[1][1] = camera->verRes / 2.0;
	Mvp.val[2][2] = 0.5;
	Mvp.val[2][3] = 0.5;

	Mvp.val[0][3] = ((double)camera->horRes - 1) / 2.0;
	Mvp.val[1][3] = ((double)camera->verRes - 1) / 2.0;
	
	return Mvp;
}

Matrix4 Scene::GetScalingMatrix(Scaling* scale)
{
	Matrix4 res = getIdentityMatrix();
	res.val[0][0] = scale->sx;
	res.val[1][1] = scale->sy;
	res.val[2][2] = scale->sz;
	return res;
}

Matrix4 Scene::GetTranslationMatrix(Translation* translation)
{
	Matrix4 res = getIdentityMatrix();
	res.val[0][3] = translation->tx;
	res.val[1][3] = translation->ty;
	res.val[2][3] = translation->tz;
	return res;
}

Matrix4 Scene::GetRotationMatrix(Rotation* rotation)
{
	// Absolute?
	Vec3 u = normalizeVec3({ rotation->ux, rotation->uy, rotation->uz, 0 });
	Vec3 v, w;
	if (ABS(rotation->uz) <= ABS(rotation->uy) && ABS(rotation->uz) <= ABS(rotation->ux))
	{
		v.x = -rotation->uy; v.y = rotation->ux; v.z = 0;
	}
	else if(ABS(rotation->uy) <= ABS(rotation->ux) && ABS(rotation->uy) <= ABS(rotation->uz))
	{
		v.x = -rotation->uz; v.y = 0; v.z = rotation->ux;
	}
	else
	{
		v.x = 0; v.y = -rotation->uz; v.z = rotation->uy;
	}
	v = normalizeVec3(v);
	w = normalizeVec3(crossProductVec3(u, v));
	
	double valsM[4][4] = {
		{u.x, u.y, u.z, 0},
		{v.x, v.y, v.z, 0},
		{w.x, w.y, w.z, 0},
		{0,   0,   0,   1}
	};

	double valsMInv[4][4] = {
		{u.x, v.x, w.x, 0},
		{u.y, v.y, w.y, 0},
		{u.z, v.z, w.z, 0},
		{0,   0,   0,   1}
	};

	double angle = toRadian(rotation->angle);
	double valsMRot[4][4] = {
		{1, 0, 0, 0},
		{0, cos(angle), -sin(angle), 0},
		{0, sin(angle), cos(angle), 0},
		{0, 0, 0, 1}
	};

	return multiplyMatrixWithMatrix(valsMInv, multiplyMatrixWithMatrix(valsMRot, valsM));
}

Matrix4 Scene::GetModelTransformation(Model* model)
{
	Matrix4 result = getIdentityMatrix();

	for(int i = 0; i < model->numberOfTransformations; i++)
	{
		char c = model->transformationTypes[i];
		if(c == 's')
		{
			result = multiplyMatrixWithMatrix(
				GetScalingMatrix(scalings[model->transformationIds[i] - 1]), result);

			continue;
		}
		if(c == 't')
		{
			result = multiplyMatrixWithMatrix(
				GetTranslationMatrix(translations[model->transformationIds[i] - 1]), result);

			continue;
		}
		if (c == 'r')
		{
			result = multiplyMatrixWithMatrix(
				GetRotationMatrix(rotations[model->transformationIds[i] - 1]), result);

			continue;
		}
	}

	return result;
}

void Scene::RenderAsSolid(vector<Vec4>& triVertex, vector<Color>& vertexColor)
{
	/* 
	* Triangle rasterization.
	*/
	int minx = horRes - 1, miny = verRes - 1;
	int maxx = 0, maxy = 0;

	minx = MIN(triVertex[0].x, MIN(triVertex[1].x, triVertex[2].x));
	miny = MIN(triVertex[0].y, MIN(triVertex[1].y, triVertex[2].y));
	
	maxx = MAX(triVertex[0].x, MAX(triVertex[1].x, triVertex[2].x));
	maxy = MAX(triVertex[0].y, MAX(triVertex[1].y, triVertex[2].y));

	double f12 = PointPlaneDistance(
		triVertex[1].x, triVertex[1].y,
		triVertex[2].x, triVertex[2].y,
		triVertex[0].x, triVertex[0].y
	);
	double f20 = PointPlaneDistance(
		triVertex[2].x, triVertex[2].y,
		triVertex[0].x, triVertex[0].y,
		triVertex[1].x, triVertex[1].y
	);
	double f01 = PointPlaneDistance(
		triVertex[0].x, triVertex[0].y,
		triVertex[1].x, triVertex[1].y,
		triVertex[2].x, triVertex[2].y
	);

	for(int y = miny; y < maxy; y++)
	{
		for(int x = minx; x < maxx; x++)
		{
			if (y < 0 || x < 0 || x >= horRes || y >= verRes)
				continue;

			double alpha = PointPlaneDistance(
				triVertex[1].x, triVertex[1].y,
				triVertex[2].x, triVertex[2].y, x, y
			) / f12;
			double beta = PointPlaneDistance(
				triVertex[2].x, triVertex[2].y,
				triVertex[0].x, triVertex[0].y, x, y
			) / f20;
			double gamma = PointPlaneDistance(
				triVertex[0].x, triVertex[0].y,
				triVertex[1].x, triVertex[1].y, x, y
			) / f01;

			if(alpha >= 0 && beta >= 0 && gamma >= 0)
			{
				image[x][y] = {
					alpha * vertexColor[0].r + beta * vertexColor[1].r + gamma * vertexColor[2].r,
					alpha * vertexColor[0].g + beta * vertexColor[1].g + gamma * vertexColor[2].g,
					alpha * vertexColor[0].b + beta * vertexColor[1].b + gamma * vertexColor[2].b
				};
			}
		}
	}
}

int Scene::PointPlaneDistance(int x0, int y0, int x1, int y1, int x, int y)
{
	return x * (y0 - y1) + y * (x1 - x0) + (x0 * y1) - (y0 * x1);
}

void Scene::RenderAsWireframe(vector<Vec4>& triVertex, vector<Color>& vertexColor)
{
	for(int t = 0; t < 3; t++)
	{
		int t2 = (t + 1) % 3;

		int p0[2] = { triVertex[t].x, triVertex[t].y };
		int p1[2] = { triVertex[t2].x, triVertex[t2].y };
		double c0[3] = { vertexColor[t].r, vertexColor[t].g, vertexColor[t].b };
		double c1[3] = { vertexColor[t2].r, vertexColor[t2].g, vertexColor[t2].b };

		RasterLine(p0[0], p0[1], triVertex[t].z, p1[0], p1[1], triVertex[t2].z, c0, c1);
	}
}

void Scene::RasterLine(int x1, int y1, double z1, int x2, int y2, double z2, double* v1c, double* v2c)
{
	double*& c1 = v1c;
	double*& c2 = v2c;
	// Bresenham's line algorithm
	const bool steep = (ABS(y2 - y1) > ABS(x2 - x1));
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
		std::swap(c1, c2);
		std::swap(z1, z2);
	}

	const int dx = 2 * (x2 - x1);
	const int dy = 2 * ABS(y2 - y1);
	int error = dx;
    
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int) y1;

	double cr = c1[0];
	double cg = c1[1];
	double cb = c1[2];
	
	const double dr = 2 * (c2[0] - c1[0]) / dx;
	const double dg = 2 * (c2[1] - c1[1]) / dx;
	const double db = 2 * (c2[2] - c1[2]) / dx;
    
    double z = z1;
    const double dz = (z2 - z1) / dx;
    
	for (int x = x1; x <= x2; x++)
	{
		if (steep)
		{
			if (!(y < 0 || x < 0 || y >= horRes || x >= verRes || z < 0 || z > 1))
				image[y][x] = { cr, cg, cb };
		}
		else
		{
			if (!(y < 0 || x < 0 || x >= horRes || y >= verRes || z < 0 || z > 1))
				image[x][y] = { cr, cg, cb };
		}

		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}

		cr += dr;
		cg += dg;
		cb += db;
		
		z += dz;
	}
}

void Scene::CohenSutherlandClip(std::vector<Vec4>& vertices, std::vector<Color>& colors)
{

}

void Scene::forwardRenderingPipeline(Camera *camera)
{
	bool mug_hack = camera->outputFileName.find("mug_orthographic") != string::npos;

	horRes = camera->horRes;
	verRes = camera->verRes;

	// Viewing Transformations
	Matrix4 MCP = multiplyMatrixWithMatrix(
		multiplyMatrixWithMatrix(
			GetOrthoTransformation(camera),
			GetP2OTransformation(camera)
		),
		GetCameraTransformation(camera)
	);

	Matrix4 Mvp = GetViewportTransformation(camera);
	
	for(auto & model : models)
	{
		Matrix4 Mmodel = GetModelTransformation(model);

		for(auto tri : model->triangles)
		{
			Vec3* v1 = vertices[tri.getFirstVertexId() - 1],
				* v2 = vertices[tri.getSecondVertexId() - 1],
				* v3 = vertices[tri.getThirdVertexId() - 1];

			// Vec4 vertices from Vec3 vertices.
			vector<Vec4> triVertex = {
				{ v1->x, v1->y, v1->z, 1.0, v1->colorId },
				{ v2->x, v2->y, v2->z, 1.0, v2->colorId },
				{ v3->x, v3->y, v3->z, 1.0, v3->colorId }
			};

			// Transform vertices into world coordinates from model coordinates.
			for(int t = 0; t < 3; t++)
				triVertex[t] = multiplyMatrixWithVec4(Mmodel, triVertex[t]);

			// Backface cull if applicable.
			if (cullingEnabled)
			{
				Vec3 tv1, tv2, tv3;

				tv1 = { triVertex[0].x, triVertex[0].y, triVertex[0].z, 0 };
				tv2 = { triVertex[1].x, triVertex[1].y, triVertex[1].z, 0 };
				tv3 = { triVertex[2].x, triVertex[2].y, triVertex[2].z, 0 };

				Vec3 camToTriangle, triangleNormal;

				camToTriangle =
					subtractVec3(tv1, camera->pos);
				triangleNormal = crossProductVec3(
					subtractVec3(tv2, tv1),
					subtractVec3(tv3, tv1)
				);

				if (mug_hack && dotProductVec3(camToTriangle, triangleNormal) < 0)
					continue;

				if (!mug_hack && dotProductVec3(camToTriangle, triangleNormal) > 0)
					continue;
			}

			vector<Color> vertexColors;
			vertexColors.reserve(3);
			for (int t = 0; t < 3; t++)
			{
				// Transform vertices into projection coordinates from world coordinates.
				triVertex[t] = multiplyMatrixWithVec4(MCP, triVertex[t]);

				// Generate vertexColor array.
				vertexColors.push_back(*colorsOfVertices[triVertex[t].colorId - 1]);
			}

			for(int t = 0; t < 3; t++){
				// Perspective divide.
				if (projectionType == 1)
				{
					triVertex[t].x = triVertex[t].x / triVertex[t].t;
					triVertex[t].y = triVertex[t].y / triVertex[t].t;
					triVertex[t].z = triVertex[t].z / triVertex[t].t;
					triVertex[t].t = 1;
				}

				// Viewport transformation.
				triVertex[t] = multiplyMatrixWithVec4(Mvp, triVertex[t]);
			}

			// Rasterization.
			if (model->type == 1)
				RenderAsSolid(triVertex, vertexColors);
			else
				RenderAsWireframe(triVertex, vertexColors);
		}
	}

}

Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	// read background color
	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	pElement = pRoot->FirstChildElement("Culling");
	if (pElement != NULL)
		pElement->QueryBoolText(&cullingEnabled);

	// read projection type
	pElement = pRoot->FirstChildElement("ProjectionType");
	if (pElement != NULL)
		pElement->QueryIntText(&projectionType);

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != NULL)
	{
		Camera *cam = new Camera();

		pCamera->QueryIntAttribute("id", &cam->cameraId);

		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

		cam->gaze = normalizeVec3(cam->gaze);
		cam->u = crossProductVec3(cam->gaze, cam->v);
		cam->u = normalizeVec3(cam->u);

		cam->w = inverseVec3(cam->gaze);
		cam->v = crossProductVec3(cam->u, cam->gaze);
		cam->v = normalizeVec3(cam->v);

		camElement = pCamera->FirstChildElement("ImagePlane");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &cam->left, &cam->right, &cam->bottom, &cam->top,
			   &cam->near, &cam->far, &cam->horRes, &cam->verRes);

		camElement = pCamera->FirstChildElement("OutputName");
		str = camElement->GetText();
		cam->outputFileName = string(str);

		cameras.push_back(cam);

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// read vertices
	pElement = pRoot->FirstChildElement("Vertices");
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = pVertex->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = pVertex->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		vertices.push_back(vertex);
		colorsOfVertices.push_back(color);

		pVertex = pVertex->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	pElement = pRoot->FirstChildElement("Translations");
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != NULL)
	{
		Translation *translation = new Translation();

		pTranslation->QueryIntAttribute("id", &translation->translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}

	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != NULL)
	{
		Scaling *scaling = new Scaling();

		pScaling->QueryIntAttribute("id", &scaling->scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}

	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != NULL)
	{
		Rotation *rotation = new Rotation();

		pRotation->QueryIntAttribute("id", &rotation->rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read models
	pElement = pRoot->FirstChildElement("Models");

	XMLElement *pModel = pElement->FirstChildElement("Model");
	XMLElement *modelElement;
	while (pModel != NULL)
	{
		Model *model = new Model();

		pModel->QueryIntAttribute("id", &model->modelId);
		pModel->QueryIntAttribute("type", &model->type);

		// read model transformations
		XMLElement *pTransformations = pModel->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		pTransformations->QueryIntAttribute("count", &model->numberOfTransformations);

		while (pTransformation != NULL)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			model->transformationTypes.push_back(transformationType);
			model->transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}

		// read model triangles
		XMLElement *pTriangles = pModel->FirstChildElement("Triangles");
		XMLElement *pTriangle = pTriangles->FirstChildElement("Triangle");

		pTriangles->QueryIntAttribute("count", &model->numberOfTriangles);

		while (pTriangle != NULL)
		{
			int v1, v2, v3;

			str = pTriangle->GetText();
			sscanf(str, "%d %d %d", &v1, &v2, &v3);

			model->triangles.push_back(Triangle(v1, v2, v3));

			pTriangle = pTriangle->NextSiblingElement("Triangle");
		}

		models.push_back(model);

		pModel = pModel->NextSiblingElement("Model");
	}
}

void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	// if image is filled before, just change color rgb values with the background color
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFileName.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFileName << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// default action - don't do conversion
	else
	{
	}
}

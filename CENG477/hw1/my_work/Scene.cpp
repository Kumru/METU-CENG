#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shape.h"
#include "tinyxml2.h"
#include "Image.h"

using namespace tinyxml2;

void Scene::RunTracing(Camera* cam, Image& image, int min_h, int max_h)
{
	for (int j = min_h; j < max_h; j++)
	{
		for (int i = 0; i < image.width; i++)
		{
			auto ray = cam->getPrimaryRay(i, j);

			auto pixelColor = TracePixel(ray);

			Color col = {
				(pixelColor.r > 255 ? 255 : pixelColor.r),
				(pixelColor.g > 255 ? 255 : pixelColor.g),
				(pixelColor.b > 255 ? 255 : pixelColor.b) };

			image.data[j][i] = col;
		}
	}
}

Vector3D Scene::TracePixel(Ray& ray)
{
	Vector3D finalColor;
	int maxRecursionDepth = pScene->maxRecursionDepth;
	Vector3D accMirrorRef(1);
	bool hit = false;

	for (int i = 0; i <= maxRecursionDepth; i++)
	{
		ReturnVal ret, tmpRet;

		float tmin = INF;
		for (auto o : objects)
		{
			tmpRet = o->intersect(ray);
			if (tmpRet.matIndex == -1) continue;
			if (tmpRet.t < tmin && tmpRet.t > 0) {
				tmin = tmpRet.t;
				ret = tmpRet;
			}
		}

		if (ret.matIndex != -1)
		{
			hit = true;

			//Ambient
			Vector3D tmpColor = ambientLight * materials[ret.matIndex - 1]->ambientRef;

			bool shadow;
			for (auto l : lights) {
				shadow = false;

				Vector3D lightDirection = l->position - ret.point;
				Vector3D lDNormal = lightDirection.Normalized();
				Ray rayToLight(ret.point + ret.normal * shadowRayEps, lDNormal);

				ReturnVal tmpRet;
				for (auto o : objects) {
					tmpRet = o->intersect(rayToLight);
					if (tmpRet.matIndex != -1 && rayToLight.gett(l->position) > tmpRet.t) {
						shadow = true;
						break;
					}
				}

				if (shadow) continue;

				//Diffuse
				float dSqr = lightDirection.Dot(lightDirection);
				float l_n = lDNormal.Dot(ret.normal);

				//Specular
				Vector3D half = (lDNormal - ray.direction).Normalized();
				float n_h = ret.normal.Dot(half);
				n_h = pow(n_h, materials[ret.matIndex - 1]->phongExp);

				Vector3D colWOLight = materials[ret.matIndex - 1]->diffuseRef * (l_n > .0 ? l_n : .0) / dSqr;
				colWOLight += materials[ret.matIndex - 1]->specularRef * (n_h > .0 ? n_h : .0) / dSqr;
				tmpColor += l->computeLightContribution(colWOLight);
			}

			finalColor += tmpColor * accMirrorRef;
			accMirrorRef *= materials[ret.matIndex - 1]->mirrorRef;

			if (materials[ret.matIndex - 1]->mirrorRef.IsZero()) { break; }

			ray.origin = ret.point + ret.normal * shadowRayEps;
			ray.direction = (ray.direction + ret.normal * -2 * ret.normal.Dot(ray.direction)).Normalized();
		}
		else
		{
			maxRecursionDepth = 0;
		}
	}

	return hit ? finalColor : backgroundColor;
}

void Scene::renderScene(void)
{
	for (auto cam : cameras) {

		auto image = Image(cam->imgPlane.nx, cam->imgPlane.ny);
		
		int numCore = (std::thread::hardware_concurrency() == 0) ? 8 : std::thread::hardware_concurrency() * 4;
		std::thread* threads = new thread[numCore];

		int step = image.height / numCore;
		if (!step) step = 1;

		for (int i = 0; i < numCore; ++i) {
			int min_h = i * step;
			int max_h = i == numCore - 1 ? image.height : (i + 1) * step;

			threads[i] = std::thread(&Scene::RunTracing, this, cam, std::ref(image), min_h, max_h);
		}

		for (int i = 0; i < numCore; i++) {
			threads[i].join();
		}
		delete[] threads;

		image.saveImage(cam->imageName);
	}
}


// Parses XML file. 
Scene::Scene(const char* xmlPath)
{
	const char* str;
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLElement* pElement;

	maxRecursionDepth = 1;
	shadowRayEps = 0.001;

	eResult = xmlDoc.LoadFile(xmlPath);

	XMLNode* pRoot = xmlDoc.FirstChild();

	pElement = pRoot->FirstChildElement("MaxRecursionDepth");
	if (pElement != nullptr)
		pElement->QueryIntText(&maxRecursionDepth);

	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%f %f %f", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	pElement = pRoot->FirstChildElement("ShadowRayEpsilon");
	if (pElement != nullptr)
		pElement->QueryFloatText(&shadowRayEps);

	pElement = pRoot->FirstChildElement("IntersectionTestEpsilon");
	if (pElement != nullptr)
		eResult = pElement->QueryFloatText(&intTestEps);

	// Parse cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement* pCamera = pElement->FirstChildElement("Camera");
	XMLElement* camElement;
	while (pCamera != nullptr)
	{
		int id;
		char imageName[64];
		Vector3D pos, gaze, up;
		ImagePlane imgPlane;

		eResult = pCamera->QueryIntAttribute("id", &id);
		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &pos.x, &pos.y, &pos.z);
		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &gaze.x, &gaze.y, &gaze.z);
		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &up.x, &up.y, &up.z);
		camElement = pCamera->FirstChildElement("NearPlane");
		str = camElement->GetText();
		sscanf(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);
		camElement = pCamera->FirstChildElement("NearDistance");
		eResult = camElement->QueryFloatText(&imgPlane.distance);
		camElement = pCamera->FirstChildElement("ImageResolution");
		str = camElement->GetText();
		sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);
		camElement = pCamera->FirstChildElement("ImageName");
		str = camElement->GetText();
		strcpy(imageName, str);

		cameras.push_back(new Camera(id, imageName, pos, gaze, up, imgPlane));

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// Parse materals
	pElement = pRoot->FirstChildElement("Materials");
	XMLElement* pMaterial = pElement->FirstChildElement("Material");
	XMLElement* materialElement;
	while (pMaterial != nullptr)
	{
		materials.push_back(new Material());

		int curr = materials.size() - 1;

		eResult = pMaterial->QueryIntAttribute("id", &materials[curr]->id);
		materialElement = pMaterial->FirstChildElement("AmbientReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->ambientRef.r, &materials[curr]->ambientRef.g, &materials[curr]->ambientRef.b);
		materialElement = pMaterial->FirstChildElement("DiffuseReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->diffuseRef.r, &materials[curr]->diffuseRef.g, &materials[curr]->diffuseRef.b);
		materialElement = pMaterial->FirstChildElement("SpecularReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->specularRef.r, &materials[curr]->specularRef.g, &materials[curr]->specularRef.b);
		materialElement = pMaterial->FirstChildElement("MirrorReflectance");
		if (materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->mirrorRef.r, &materials[curr]->mirrorRef.g, &materials[curr]->mirrorRef.b);
		}
		else
		{
			materials[curr]->mirrorRef.r = 0.0;
			materials[curr]->mirrorRef.g = 0.0;
			materials[curr]->mirrorRef.b = 0.0;
		}
		materialElement = pMaterial->FirstChildElement("PhongExponent");
		if (materialElement != nullptr)
			materialElement->QueryIntText(&materials[curr]->phongExp);

		pMaterial = pMaterial->NextSiblingElement("Material");
	}

	// Parse vertex data
	pElement = pRoot->FirstChildElement("VertexData");
	int cursor = 0;
	Vector3D tmpPoint;
	str = pElement->GetText();
	while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
		cursor++;
	while (str[cursor] != '\0')
	{
		for (int cnt = 0; cnt < 3; cnt++)
		{
			if (cnt == 0)
				tmpPoint.x = atof(str + cursor);
			else if (cnt == 1)
				tmpPoint.y = atof(str + cursor);
			else
				tmpPoint.z = atof(str + cursor);
			while (str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
				cursor++;
			while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
		}
		vertices.push_back(tmpPoint);
	}

	// Parse objects
	pElement = pRoot->FirstChildElement("Objects");

	// Parse spheres
	XMLElement* pObject = pElement->FirstChildElement("Sphere");
	XMLElement* objElement;
	while (pObject != nullptr)
	{
		int id;
		int matIndex;
		int cIndex;
		float R;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		objects.push_back(new Sphere(id, matIndex, cIndex, R, &vertices));

		pObject = pObject->NextSiblingElement("Sphere");
	}

	// Parse triangles
	pObject = pElement->FirstChildElement("Triangle");
	while (pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Indices");
		str = objElement->GetText();
		sscanf(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

		objects.push_back(new Triangle(id, matIndex, p1Index, p2Index, p3Index, &vertices));

		pObject = pObject->NextSiblingElement("Triangle");
	}

	// Parse meshes
	pObject = pElement->FirstChildElement("Mesh");
	while (pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		vector<Triangle> faces;
		vector<int>* meshIndices = new vector<int>;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");
		objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
		str = objElement->GetText();
		while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while (str[cursor] != '\0')
		{
			for (int cnt = 0; cnt < 3; cnt++)
			{
				if (cnt == 0)
					p1Index = atoi(str + cursor) + vertexOffset;
				else if (cnt == 1)
					p2Index = atoi(str + cursor) + vertexOffset;
				else
					p3Index = atoi(str + cursor) + vertexOffset;
				while (str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++;
				while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			faces.push_back(*(new Triangle(-1, matIndex, p1Index, p2Index, p3Index, &vertices)));
			meshIndices->push_back(p1Index);
			meshIndices->push_back(p2Index);
			meshIndices->push_back(p3Index);
		}

		objects.push_back(new Mesh(id, matIndex, faces, meshIndices, &vertices));

		pObject = pObject->NextSiblingElement("Mesh");
	}

	// Parse lights
	int id;
	Vector3D position;
	Vector3D intensity;
	pElement = pRoot->FirstChildElement("Lights");

	XMLElement* pLight = pElement->FirstChildElement("AmbientLight");
	XMLElement* lightElement;
	str = pLight->GetText();
	sscanf(str, "%f %f %f", &ambientLight.r, &ambientLight.g, &ambientLight.b);

	pLight = pElement->FirstChildElement("PointLight");
	while (pLight != nullptr)
	{
		eResult = pLight->QueryIntAttribute("id", &id);
		lightElement = pLight->FirstChildElement("Position");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &position.x, &position.y, &position.z);
		lightElement = pLight->FirstChildElement("Intensity");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &intensity.r, &intensity.g, &intensity.b);

		lights.push_back(new PointLight(position, intensity));

		pLight = pLight->NextSiblingElement("PointLight");
	}
}


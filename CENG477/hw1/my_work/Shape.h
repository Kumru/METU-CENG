#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <vector>
#include "Ray.h"
#include "defs.h"

using namespace std;

// Base class for any shape object
class Shape
{
public:
	int id;	        // Id of the shape
	int matIndex;	// Material index of the shape

	virtual ReturnVal intersect(const Ray& ray) const = 0; // Pure virtual method for intersection test. You must implement this for sphere, triangle, and mesh. 

	Shape(void);
	Shape(int id, int matIndex); // Constructor

private:
	// Write any other stuff here
};

// Class for sphere
class Sphere : public Shape
{
public:
	Sphere(void);	// Constructor
	Sphere(int id, int matIndex, int cIndex, float R, vector<Vector3D>* vertices);	// Constructor
	ReturnVal intersect(const Ray& ray) const;	// Will take a ray and return a structure related to the intersection information. You will implement this. 

private:
	// Write any other stuff here
	Vector3D center;
	float radius;
};

// Class for triangle
class Triangle : public Shape
{
public:
	Triangle(void);	// Constructor
	Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3D>* vertices);	// Constructor
	ReturnVal intersect(const Ray& ray) const; // Will take a ray and return a structure related to the intersection information. You will implement this. 

private:
	// Write any other stuff here
	Vector3D a, b, c;
	Vector3D ab, ac;
	Vector3D normal;
};

class BoundingBox
{
public:
	Vector3D min = Vector3D(INF), max = Vector3D(-INF);

	BoundingBox *left = nullptr, *right = nullptr;
	Triangle* obj = nullptr;

	inline bool intersect(const Ray& ray) const
	{
		Vector3D invRayDir = ray.direction.Reciprocal();
		Vector3D t0 = (min - ray.origin) * invRayDir;
		Vector3D t1 = (max - ray.origin) * invRayDir;
		Vector3D tmin(
			t0.x < t1.x ? t0.x : t1.x,
			t0.y < t1.y ? t0.y : t1.y,
			t0.z < t1.z ? t0.z : t1.z
		);
		Vector3D tmax(
			t0.x > t1.x ? t0.x : t1.x,
			t0.y > t1.y ? t0.y : t1.y,
			t0.z > t1.z ? t0.z : t1.z
		);
		float max_component =
			(tmin.x > tmin.y
				? (tmin.x > tmin.z ? tmin.x : tmin.z)
				: (tmin.y > tmin.z ? tmin.y : tmin.z));
		float min_component =
			(tmax.x < tmax.y
				? (tmax.x < tmax.z ? tmax.x : tmax.z)
				: (tmax.y < tmax.z ? tmax.y : tmax.z));

		return max_component <= min_component;
	}
};

class BVH
{
public:
	vector<int>* pIndices;
	vector<Vector3D>* vertices;
	vector<int> tIndices;
	vector<Triangle>* faces;

	BoundingBox* root = nullptr;

	BVH() {};
	BVH(vector<int>* pIndices, vector<Vector3D>* vertices, int len) : pIndices(pIndices), vertices(vertices)
	{
		tIndices.reserve(len);
		for (int i = 0; i < len; i++) tIndices.push_back(i);
	}
	
	void BuildBVH(BoundingBox* node, int start, int end);
	void intersect(const Ray& ray, ReturnVal& ret, BoundingBox* node) const;

	inline float minimumOfFour(float a, float b, float c, float d)
	{
		return (a < b ? (a < c ? (a < d ? a : d) : (c < d ? c : d)) :
			(b < c ? (b < d ? b : d) : (c < d ? c : d)));
	}
	inline float maximumOfFour(float a, float b, float c, float d)
	{
		return (a > b ? (a > c ? (a > d ? a : d) : (c > d ? c : d)) :
			(b > c ? (b > d ? b : d) : (c > d ? c : d)));
	}
};

// Class for mesh
class Mesh : public Shape
{
public:
	Mesh(void);	// Constructor
	Mesh(int id, int matIndex, const vector<Triangle>& faces, vector<int>* pIndices, vector<Vector3D>* vertices);	// Constructor
	ReturnVal intersect(const Ray& ray) const; // Will take a ray and return a structure related to the intersection information. You will implement this. 

private:
	// Write any other stuff here
	vector<Triangle> tris;

	BVH hierarchy;
};

#endif

#include "Shape.h"
#include "Scene.h"
#include <cstdio>

#define GEN(i, v, k) ((*vertices)[(*pIndices)[i * 3 + v] - 1].elements[k])
#define   X(i, v)    ((*vertices)[(*pIndices)[i * 3 + v] - 1].elements[0])
#define   Y(i, v)    ((*vertices)[(*pIndices)[i * 3 + v] - 1].elements[1])
#define   Z(i, v)    ((*vertices)[(*pIndices)[i * 3 + v] - 1].elements[2])


Shape::Shape(void)
{
}

Shape::Shape(int id, int matIndex)
    : id(id), matIndex(matIndex)
{
}

Sphere::Sphere(void)
{}

Sphere::Sphere(int id, int matIndex, int cIndex, float R, vector<Vector3D>* vertices)
    : Shape(id, matIndex), center((*vertices)[cIndex - 1]), radius(R)
{

}

ReturnVal Sphere::intersect(const Ray & ray) const
{
	ReturnVal ret;
	
	Vector3D oc = ray.origin - center;

	float a, b, c;
	a = ray.direction.Dot(ray.direction);
	b = oc.Dot(ray.direction);
	c = oc.Dot(oc) - radius * radius;
	float discriminant = b * b - a * c;
	
	if (discriminant <= 0) return ret;
	discriminant = std::sqrt(discriminant);

	float root2 = (-b + discriminant) / a;
	if (root2 < 0) return ret;
	float root1 = (-b - discriminant) / a;
	
	ret.matIndex = matIndex;
	ret.t = (root1 <= root2 && root1 >= 0) ? root1 : root2;
	ret.point = ray.getPoint(ret.t);
	ret.normal = (ray.origin + ray.direction * ret.t - center).Normalized();
	return ret;
}

Triangle::Triangle(void)
{}

Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3D>* vertices)
    : Shape(id, matIndex), a((*vertices)[p1Index - 1]), b((*vertices)[p2Index - 1]), c((*vertices)[p3Index - 1])
{
	ab = b - a;
	ac = c - a;
	normal = ab.Cross(ac).Normalized();
}

ReturnVal Triangle::intersect(const Ray & ray) const
{
	ReturnVal ret;

	Vector3D p = ray.direction.Cross(ac);
	float determinant = ab.Dot(p);
	if (determinant < 0) return ret;

	determinant = 1 / determinant;

	Vector3D tvec = ray.origin - a;
	float u = tvec.Dot(p) * determinant;
	if (u < 0 || u > 1) return ret;

	Vector3D qvec = tvec.Cross(ab);
	float v = ray.direction.Dot(qvec) * determinant;
	if (v < 0 || u + v > 1) return ret;

	ret.t = ac.Dot(qvec) * determinant;
	ret.matIndex = matIndex;	
	ret.point = ray.getPoint(ret.t);
	ret.normal = normal;
	return ret;
}

void BVH::BuildBVH(BoundingBox* node, int start, int end)
{
	if (start >= end) return;
	
	if (start + 1 == end) {
		node->obj = &(*faces)[start];
		return;
	}

	Vector3D min(INF), max(-INF);
	for (int i = start; i < end; i++)
	{
		min.x = minimumOfFour(min.x, X(i, 0), X(i, 1), X(i, 2));
		min.y = minimumOfFour(min.y, Y(i, 0), Y(i, 1), Y(i, 2));
		min.z = minimumOfFour(min.z, Z(i, 0), Z(i, 1), Z(i, 2));

		max.x = maximumOfFour(max.x, X(i, 0), X(i, 1), X(i, 2));
		max.y = maximumOfFour(max.y, Y(i, 0), Y(i, 1), Y(i, 2));
		max.z = maximumOfFour(max.z, Z(i, 0), Z(i, 1), Z(i, 2));
	}
	node->min = min; node->max = max;

	Vector3D delta = node->max - node->min;
	int maxDim = (delta.x > delta.y ? (delta.x > delta.z ? 0 : 2) : (delta.y > delta.z ? 1 : 2));
	float mean = delta.elements[maxDim] / 2;

	int mid = start;
	for (int i = start; i < end; i++) {
		float center = (GEN(i, 0, maxDim) +
						GEN(i, 1, maxDim) +
						GEN(i, 2, maxDim)) / 3;
		if (center < mean) {
			std::swap(tIndices[i], tIndices[mid++]);
		}
	}

	if (mid == start || mid == end) {
		mid = (start + end) / 2;
	}

	if (start < mid) {
		node->left = new BoundingBox;
		BuildBVH(node->left, start, mid);
	}
	if (mid < end) {
		node->right = new BoundingBox;
		BuildBVH(node->right, mid, end);
	}
}

void BVH::intersect(const Ray& ray, ReturnVal& ret, BoundingBox* node) const
{
	if (node->obj != nullptr)
	{
		ReturnVal tmp = node->obj->intersect(ray);
		if (tmp.t < ret.t && tmp.t > 0) { ret = tmp; }
	}
	else if (node->intersect(ray))
	{
		if (node->left != nullptr) intersect(ray, ret, node->left);
		if (node->right != nullptr) intersect(ray, ret, node->right);
	}
}

Mesh::Mesh()
{}

Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces, vector<int>* pIndices, vector<Vector3D>* vertices)
    : Shape(id, matIndex), tris(faces), hierarchy(pIndices, vertices, pIndices->size() / 3)
{
	int len = pIndices->size() / 3;

	hierarchy.faces = &tris;

	hierarchy.root = new BoundingBox;
	hierarchy.BuildBVH(hierarchy.root, 0, len);
}

ReturnVal Mesh::intersect(const Ray & ray) const
{
	ReturnVal ret;
	hierarchy.intersect(ray, ret, hierarchy.root);
	return ret;
}

#include "the3.h"


// You  can define extra functions here

void DFSinNode(int node, int**& edgeList, std::vector<int>& component, std::vector<bool>& visited)
{
	visited[node] = true;
	component.push_back(node);

	for (int i = 0; i < visited.size(); i++)
	{
		if (edgeList[node][i] && !visited[i])
			DFSinNode(i, edgeList, component, visited);
	}
}

void getConnectedComponentVector(int n, int**& edgeList, std::vector<std::vector<int>>& result)
{
	std::vector<bool> visited(n, false);
	for (int i = 0; i < n; i++)
	{
		if (!visited[i])
		{
			std::vector<int> component;
			DFSinNode(i, edgeList, component, visited);

			result.push_back(component);
		}
	}
}

int** FloydWarshallAPSP(int n, int** edgeList)
{
	int** lst = new int* [n];
	for (int i = 0; i < n; i++)
	{
		int* tmp = new int[n];
		lst[i] = tmp;
		for (int j = 0; j < n; j++)
		{
			if (edgeList[i][j] != 0 || i == j)
				lst[i][j] = edgeList[i][j];
			else
				lst[i][j] = INT_MAX;
		}
	}

	for (int k = 0; k < n; k++)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				int newCost = lst[i][k] + lst[k][j];
				if (newCost >= 0 && newCost < lst[i][j])
					lst[i][j] = newCost;
			}
		}
	}

	return lst;
}

// INPUT :
//            n         : number of nodes in the graph
//            edgeList  : edges in the graph
//            scores    : importance scores
// return value : 
//                 number of disconnected components
int Important(int n, int**& edgeList, double*& scores)
{
	std::vector<std::vector<int>> components;
	getConnectedComponentVector(n, edgeList, components);

	int** APSP = FloydWarshallAPSP(n, edgeList);

	for (auto& component : components)
	{
		for (int v : component)
		{
			double score = 0;

			for (int s : component)
			{
				for (int t : component)
				{
					int st = 0;
					if (s == t) st = 1;
					else st = APSP[s][t];

					int stv = 0;
					if (v == s || v == t) stv = 0;
					else stv = APSP[s][v] + APSP[v][t];

					score += (double)stv / st;
				}
			}

			scores[v] = score;
		}
	}

	return components.size();
}
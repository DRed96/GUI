#ifndef __j1PATHFINDING__
#define __j1PATHFINDING__

#include "j1Module.h"
#include "p2DynArray.h"
#include "p2Point.h"
#include "p2List.h"

#include "j1Map.h"

#include <math.h>


struct pathNode
{
	pathNode(pathNode* _parent, iPoint _position, iPoint destination, float movementCost = 1.0f)
	{
		parent = _parent;
		position = _position;
		if (parent)
		{
			cost = parent->cost + movementCost;
		}
		else
		{
			cost = movementCost;
		}
		//Manhattan distance 
		//stepsLeft = abs((float)position.x - (float)destination.x) + abs((float)position.y - (float)destination.y);

		// The other distance
		int deltaX = (position.x - destination.x)*(position.x - destination.x);
		int deltaY = (position.y - destination.y)*(position.y - destination.y);
		stepsLeft = sqrt(deltaX + deltaY);
		n = cost + stepsLeft;
	}
	pathNode()
	{}

	void Initialize(pathNode* _parent, iPoint _position, iPoint destination, float movementCost = 1.0f)
	{
		parent = _parent;
		position = _position;
		if (parent)
		{
			cost = parent->cost + movementCost;
		}
		else
		{
			cost = movementCost;
		}
		//Manhattan distance 
		//stepsLeft = abs((float)position.x - (float)destination.x) + abs((float)position.y - (float)destination.y);

		// The other distance
		int deltaX = (position.x - destination.x)*(position.x - destination.x);
		int deltaY = (position.y - destination.y)*(position.y - destination.y);
		stepsLeft = (float)sqrt(deltaX + deltaY);
		n = cost + stepsLeft;
	}

	pathNode* parent;
	iPoint position;
	float n;
	float cost;
	float stepsLeft;
};

class j1Pathfinding : public j1Module
{
public:

	j1Pathfinding();

	// Destructor
	virtual ~j1Pathfinding();

	// Called before render is available
	bool Awake();

	p2DynArray<iPoint>* FindPath(iPoint start, iPoint end);

private:
	p2List_item<MapLayer*>* j1Pathfinding::GetWalkabilityMap();
	bool CheckWalkability(iPoint tile);
	p2List_item<pathNode*>* FindMinorN();
	bool CheckForDuplicates(pathNode* fatherNode, iPoint positionToCheck, iPoint end, float n);
	p2DynArray <iPoint>* TrackBack();
	void ReleaseNodes();
	float CalculateN(iPoint positionToCheck, iPoint end, pathNode* fatherNode, float movementCost);


	p2List_item<MapLayer*>* walkabilityMap;
	p2List<pathNode*> openNodes;
	p2List<pathNode*> closedNodes;

public:
	bool diagonalsAllowed;

};

#endif // __j1PATHFINDING__
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Pathfinding.h"

j1Pathfinding::j1Pathfinding() : j1Module()
{
	name.create("pathfinding");
}

// Destructor
j1Pathfinding::~j1Pathfinding()
{}

// Called before render is available
bool j1Pathfinding::Awake()
{
	bool ret = true;

	return ret;
}

p2List_item<MapLayer*>* j1Pathfinding::GetWalkabilityMap()
{
	p2List_item<MapLayer*>* walkabilityMap = App->map->data.layers.start;
	while (walkabilityMap->data->properties.Get("Nodraw") == 0)
	{
		walkabilityMap = walkabilityMap->next;
	}
	return walkabilityMap;
}

bool j1Pathfinding::CheckWalkability(iPoint tile)
{
	int a = walkabilityMap->data->Get(tile.x, tile.y);
	bool b = App->map->InBoundaries(tile);
	if (App->map->InBoundaries(tile) && walkabilityMap->data->Get(tile.x, tile.y) != 26)
	{
		return true;
	}
	return false;
}

void j1Pathfinding::ReleaseNodes()
{
	for (p2List_item<pathNode*>* item = openNodes.start; item; item = item->next)
	{
		RELEASE(item->data);
	}
	openNodes.clear();
	for (p2List_item<pathNode*>* item = closedNodes.start; item; item = item->next)
	{
		RELEASE(item->data);
	}
	closedNodes.clear();
}

p2List_item<pathNode*>* j1Pathfinding::FindMinorN()
{
	p2List_item<pathNode*>* currentlyChecking = openNodes.start;
	for (p2List_item<pathNode*>* item = currentlyChecking; item; item = item->next)
	{
		if (item->data->n < currentlyChecking->data->n)
			currentlyChecking = item;
	}
	return currentlyChecking;
}

float j1Pathfinding::CalculateN(iPoint positionToCheck, iPoint end, pathNode* fatherNode, float movementCost)
{
	float deltaX = (positionToCheck.x - end.x)*(positionToCheck.x - end.x);
	float deltaY = (positionToCheck.y - end.y)*(positionToCheck.y - end.y);
	float stepsLeft = sqrt(deltaX + deltaY);
	//float stepsLeft = positionToCheck.x - end.x + positionToCheck.y - end.y;
	return(stepsLeft + (1.0f * movementCost + fatherNode->cost));
}

//Check if a certain tile is already in the Open or the closed list. If it is, check the N value and asign as parent the node with minor N
bool j1Pathfinding::CheckForDuplicates(pathNode* fatherNode, iPoint positionToCheck, iPoint end, float n)
{
	bool duplicated = false;
	for (p2List_item<pathNode*>* item = openNodes.start; item && duplicated == false; item = item->next)
	{
		if (item->data->position == positionToCheck)
		{
			if (item->data->parent)
			{
				if (item->data->n > n)
				{
					item->data->Initialize(fatherNode, positionToCheck, end);
				}
			}
			duplicated = true;
		}
	}
	for (p2List_item<pathNode*>* item = closedNodes.start; item && duplicated == false; item = item->next)
	{
		if (item->data->position == positionToCheck)
		{
			if (item->data->parent)
			{
				if (item->data->n > n)
				{
					item->data->Initialize(fatherNode, positionToCheck, end);
				}
			}
			duplicated = true;
		}
	}

	return duplicated;
}

p2DynArray<iPoint>* j1Pathfinding::FindPath(iPoint start, iPoint end)
{
	walkabilityMap = GetWalkabilityMap();
	p2DynArray<iPoint>* ret = NULL;

	pathNode* node = new pathNode(NULL, start, end);
	node->cost = 0;
	openNodes.add(node);
	bool ended = false;

	if (CheckWalkability(start) == false || CheckWalkability(end) == false)
	{
		ended = true;
		closedNodes.add(node);
		openNodes.del(openNodes.At(0));
	}

	while (openNodes.count() > 0 && ended == false)
	{
		p2List_item<pathNode*>* currentlyChecking = FindMinorN();
		int nSons = 4;
		//if (diagonalsAllowed) { nSons = 8; }
		for (int m = 0; m < nSons && !ended; m++)
		{
			iPoint position = currentlyChecking->data->position;
			switch (m)
			{
			case 0: { position.x += 1; break; }
			case 1: { position.y += 1; break; }
			case 2: { position.x -= 1; break; }
			case 3: { position.y -= 1; break; }

			case 4: { position.x += 1; position.y += 1; break; }
			case 5: { position.x -= 1; position.y += 1; break; }
			case 6: { position.x -= 1; position.y -= 1; break; }
			case 7: { position.x += 1; position.y -= 1; break; }
			}
			if (position == end)
			{
				pathNode* toAdd = new pathNode(currentlyChecking->data, position, end);
				closedNodes.add(toAdd);
				ended = true;
			}
			if (CheckWalkability(position) && !ended)
			{
				float movementCost = 1.0f;
				if (m >= 4){ movementCost = 1.4f; }
				float n = CalculateN(position, end, currentlyChecking->data, movementCost);

				if (CheckForDuplicates(currentlyChecking->data, position, end, n) == false)
				{
					pathNode* toAdd = new pathNode(currentlyChecking->data, position, end, movementCost);
					openNodes.add(toAdd);
				}
			}
		}
		if (!ended)
		{
			closedNodes.add(currentlyChecking->data);
			openNodes.del(currentlyChecking);
		}
	}

	if (ended)
	{
		ret = TrackBack();
	}

	ReleaseNodes();

	return ret;

}

p2DynArray <iPoint>* j1Pathfinding::TrackBack()
{
	p2DynArray <iPoint>* ret = new p2DynArray <iPoint>;
	pathNode* trackBack = closedNodes.end->data;
	while (trackBack != NULL)
	{
		ret->PushBack(trackBack->position);
		trackBack = trackBack->parent;
	}
	return ret;
}
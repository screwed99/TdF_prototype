#include "Controller.h"

Controller::Controller(Model* model, View* view)
{
	for(int i=0; i < CUBE_ALLOCATION; ++i)
	{
		cubeStates[i] = NOT_CONNECTED;
	}
	for (CubeID cube : CubeSet::connected())
    {
    	cubeStates[cube] = ROAD_VIEW;
    }

    int cyc=0;
    while(cubeStates[cyc] == NOT_CONNECTED && cyc < CUBE_ALLOCATION)
    {
    	++cyc;
    }
    cubeStates[cyc] = CYCLIST;

    int act=cyc+1;
    while(cubeStates[act] == NOT_CONNECTED && act < CUBE_ALLOCATION)
    {
    	++act;
    }
    cubeStates[act] = ACTION;

    int cen=act+1;
    while(cubeStates[cen] == NOT_CONNECTED && cen < CUBE_ALLOCATION)
    {
    	++cen;
    }

    myModel = model;
	myView = view;
}

Controller::Controller()
{

}

void Controller::onNeighbourAdd(unsigned int cube0Id, unsigned int side0,
	unsigned int cube1Id, unsigned int side1)
{
	if(cubeStates[cube0Id] == ROAD_VIEW && cubeStates[cube1Id] == ROAD_VIEW)
	{
		//LOG("Road-road Neighbour add with cubes %d and %d\n",cube0Id, cube1Id);
		myView->roadNeighbourAdd(cube0Id, side0, cube1Id, side1);
	}
	else if(cubeStates[cube0Id] == ROAD_VIEW && cubeStates[cube1Id] == CYCLIST)
	{
		//LOG("Road-cyclist Neighbour add with cubes %d and %d\n",cube0Id, cube1Id);
		myView->cyclistRoadNeighbourAdd(cube0Id, side0, side1);
	}
	else if(cubeStates[cube1Id] == ROAD_VIEW && cubeStates[cube0Id] == CYCLIST)
	{
		//LOG("Road-cyclist Neighbour add with cubes %d and %d\n",cube1Id, cube0Id);
		myView->cyclistRoadNeighbourAdd(cube1Id, side1, side0);
	}
	else if(cubeStates[cube0Id] == ACTION && cubeStates[cube1Id] == CYCLIST)
	{
		//LOG("Action-cyclist Neighbour add with cubes %d and %d\n",cube0Id, cube1Id);
		myView->cyclistActionNeighbourAdd(side0);
	}
	else if(cubeStates[cube1Id] == ACTION && cubeStates[cube0Id] == CYCLIST)
	{
		//LOG("Action-cyclist Neighbour add with cubes %d and %d\n",cube1Id, cube0Id);
		myView->cyclistActionNeighbourAdd(side1);
	}
}

void Controller::onNeighbourRemove(unsigned int cube0Id, unsigned int side0,
	unsigned int cube1Id, unsigned int side1)
{
	if(cubeStates[cube0Id] == ROAD_VIEW && cubeStates[cube1Id] == ROAD_VIEW)
	{
		myView->roadNeighbourRemove(cube0Id, side0, cube1Id, side1);
	}
	else if(cubeStates[cube0Id] == ACTION && cubeStates[cube1Id] == CYCLIST)
	{
		myView->cyclistActionNeighbourRemove(side0);
	}
	else if(cubeStates[cube1Id] == ACTION && cubeStates[cube0Id] == CYCLIST)
	{
		myView->cyclistActionNeighbourRemove(side1);
	}
}

void Controller::onTouch(unsigned int cube)
{

}

void Controller::onConnect(unsigned int cube)
{

}

void Controller::onDisconnect(unsigned int cube)
{

}

void Controller::updateTime(TimeDelta delta)
{
	myModel->updateMove(delta);
	myView->updateCyclist();
}
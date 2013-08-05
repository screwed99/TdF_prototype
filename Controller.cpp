#include "Controller.h"

Controller::Controller(Model* model)
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
	myView = View(myModel, cen, cyc, act);
}

Controller::Controller()
{

}

void Controller::onNeighbourAdd(unsigned int cube0Id, unsigned int side0,
	unsigned int cube1Id, unsigned int side1)
{
	if(cube0Id == ROAD_VIEW && cube1Id == ROAD_VIEW)
	{
		myView.roadNeighbourAdd(cube0Id, side0, cube1Id, side1);
	}
	else if(cube0Id == ROAD_VIEW && cube1Id == CYCLIST)
	{
		myView.cyclistRoadNeighbourAdd(cube0Id, side0, side1);
	}
	else if(cube1Id == ROAD_VIEW && cube0Id == CYCLIST)
	{
		myView.cyclistRoadNeighbourAdd(cube1Id, side1, side0);
	}
	else if(cube0Id == ACTION && cube1Id == CYCLIST)
	{
		myView.cyclistActionNeighbourAdd(side0);
	}
	else if(cube1Id == ACTION && cube0Id == CYCLIST)
	{
		myView.cyclistActionNeighbourAdd(side1);
	}
}

void Controller::onNeighbourRemove(unsigned int cube0Id, unsigned int side0,
	unsigned int cube1Id, unsigned int side1)
{
	if(cube0Id == ROAD_VIEW && cube1Id == ROAD_VIEW)
	{
		myView.roadNeighbourRemove(cube0Id, side0, cube1Id, side1);
	}
	else if(cube0Id == ACTION && cube1Id == CYCLIST)
	{
		myView.cyclistActionNeighbourRemove(side0);
	}
	else if(cube1Id == ACTION && cube0Id == CYCLIST)
	{
		myView.cyclistActionNeighbourRemove(side1);
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
}
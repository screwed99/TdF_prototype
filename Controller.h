#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <sifteo.h>
#include "Model.h"
#include "View.h"
using namespace Sifteo;

class Controller
{

enum CubeState{
    NOT_CONNECTED,
    CONNECTED,
    ROAD_VIEW,
    CYCLIST,
    ACTION,
};

private:	
	CubeState cubeStates[CUBE_ALLOCATION];

	Model* myModel;
	View* myView;

public:
	Controller(Model* model, View* view);
	Controller();

	void onNeighbourAdd(unsigned int cube0Id, unsigned int side0,
    	unsigned int cube1Id, unsigned int side1);
	void onNeighbourRemove(unsigned int cube0Id, unsigned int side0,
    	unsigned int cube1Id, unsigned int side1);
	void onTouch(unsigned int cube);
	void onConnect(unsigned int cube);
	void onDisconnect(unsigned int cube);

	void updateTime(TimeDelta delta);

};

#endif
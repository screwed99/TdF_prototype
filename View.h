#ifndef VIEW_H
#define VIEW_H
#include <sifteo.h>
#include "Model.h"
#include "assets.gen.h"
using namespace Sifteo;

class View
{

private:
	VideoBuffer gVideo[CUBE_ALLOCATION];

	Int2 topLeftRoadCubes[CUBE_ALLOCATION];
	bool alreadyUpdated[CUBE_ALLOCATION];

	Model* myModel;

	CubeID centreCube;

	CubeID cyclistCube;
	CubeID actionCube;
    
public:
	View(Model* model, CubeID cCube, CubeID cycCube, CubeID aCube);
	View();

	void roadNeighbourAdd(unsigned int cube0Id, unsigned int side0,
		unsigned int cube1Id, unsigned int side1);
	void roadNeighbourRemove(unsigned int cube0Id, unsigned int side0,
		unsigned int cube1Id, unsigned int side1);
	void cyclistActionNeighbourAdd(unsigned int actionSide);
	void cyclistActionNeighbourRemove(unsigned int actionSide);
	void cyclistRoadNeighbourAdd(unsigned int roadID, unsigned int roadSide, unsigned int cyclistSide);

	void setActionCube(CubeID cube);
	void setCyclistCube(CubeID cube);

	void updateCyclist();//Calls Model

private:
	void findNewTopLefts(unsigned int roadID, Int2 topLeftImage);
	void redrawRoad(unsigned int roadID, Int2 currPosition);
	void drawCyclist(Int2 Position);
	void drawSingleRoadCube(unsigned int roadID);

};

#endif
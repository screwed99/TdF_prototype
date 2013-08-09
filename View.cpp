#include "View.h"

View::View(Model* model, CubeID cCube, CubeID cycCube, CubeID aCube)
{
	myModel = model;
	centreCube = cCube;
	cyclistCube = cycCube;
	actionCube = aCube;

	for (CubeID cube : CubeSet::connected())
    {
    	gVideo[cube].initMode(BG0_BG1);
	    gVideo[cube].attach(cube);
	    gVideo[cube].bg0.image(vec(0,0), BlackImage);
    }

    gVideo[(unsigned int) cyclistCube].bg0.image(vec(0,0), Bradley);
    gVideo[(unsigned int) actionCube].bg0.image(vec(0,0), Movement);

	Int2 currPosition = myModel->getPosition();

	gVideo[(unsigned int) centreCube].bg1.setMask(BG1Mask::filled(vec(7,7),vec(3,3)));

	Int2 currTopLeft = currPosition - vec(56,56);

	redrawRoad(currPosition, currTopLeft);
}

View::View()
{
	
}

void View::roadNeighbourAdd(unsigned int cube0Id, unsigned int side0,
	unsigned int cube1Id, unsigned int side1)
{
	//LOG("alreadyUpdated[cube0Id] = %d, alreadyUpdated[cube1Id] = %d\n",alreadyUpdated[cube0Id],alreadyUpdated[cube1Id]);
	if(alreadyUpdated[cube0Id] && !alreadyUpdated[cube1Id])
	{
		Neighborhood neighbourhood = Neighborhood(cube0Id);

		gVideo[cube1Id].orientTo(gVideo[cube0Id]);

		switch(gVideo[cube0Id].physicalToVirtual(neighbourhood).sideOf(cube1Id))
		{
			case(TOP) :
			{
				topLeftRoadCubes[cube1Id] = topLeftRoadCubes[cube0Id] + vec(0,-128);
				break;
			}
			case(LEFT) :
			{
				//LOG("Road neighbour added onthe left\n");
				topLeftRoadCubes[cube1Id] = topLeftRoadCubes[cube0Id] + vec(-128,0);
				break;
			}
			case(BOTTOM) :
			{
				topLeftRoadCubes[cube1Id] = topLeftRoadCubes[cube0Id] + vec(0,128);
				break;
			}
			case(RIGHT) :
			{
				topLeftRoadCubes[cube1Id] = topLeftRoadCubes[cube0Id] + vec(128,0);
				break;
			}
			default :
				break;
		}

		drawSingleRoadCube(cube1Id);
	}
	else if(!alreadyUpdated[cube0Id] && alreadyUpdated[cube1Id])
	{
		Neighborhood neighbourhood = Neighborhood(cube1Id);

		gVideo[cube0Id].orientTo(gVideo[cube1Id]);

		switch(gVideo[cube1Id].physicalToVirtual(neighbourhood).sideOf(cube0Id))
		{
			case(TOP) :
			{
				topLeftRoadCubes[cube0Id] = topLeftRoadCubes[cube1Id] + vec(0,-128);
				break;
			}
			case(LEFT) :
			{
				//LOG("Road neighbour added on the left\n");
				topLeftRoadCubes[cube0Id] = topLeftRoadCubes[cube1Id] + vec(-128,0);
				break;
			}
			case(BOTTOM) :
			{
				topLeftRoadCubes[cube0Id] = topLeftRoadCubes[cube1Id] + vec(0,128);
				break;
			}
			case(RIGHT) :
			{
				topLeftRoadCubes[cube0Id] = topLeftRoadCubes[cube1Id] + vec(128,0);
				break;
			}
			default :
				break;
		}
		
		drawSingleRoadCube(cube0Id);
	}
}

void View::roadNeighbourRemove(unsigned int cube0Id, unsigned int side0,
	unsigned int cube1Id, unsigned int side1)
{
	//LOG("alreadyUpdated[cube0Id] = %d, alreadyUpdated[cube1Id] = %d\n",alreadyUpdated[cube0Id],alreadyUpdated[cube1Id]);
	if(alreadyUpdated[cube0Id] && alreadyUpdated[cube1Id])
	{
		Int2 currPosition = myModel->getPosition();
		redrawRoad(currPosition, topLeftRoadCubes[(unsigned int) centreCube]);
	}
}

void View::cyclistActionNeighbourAdd(unsigned int actionSide)
{
	myModel->userMove(actionSide);

	switch(actionSide)
	{
		case(TOP) :
		{
			gVideo[(unsigned int) actionCube].bg0.image(vec(6,0), vec(4,3), MovementHighlight, vec(6,0));
			break;
		}
		case(LEFT) :
		{
			gVideo[(unsigned int) actionCube].bg0.image(vec(0,6), vec(4,3), MovementHighlight, vec(0,6));
			break;
		}
		case(BOTTOM) :
		{
			gVideo[(unsigned int) actionCube].bg0.image(vec(6,13), vec(4,3), MovementHighlight, vec(6,13));
			break;
		}
		case(RIGHT) :
		{
			gVideo[(unsigned int) actionCube].bg0.image(vec(13,6), vec(4,3), MovementHighlight, vec(13,6));
			break;
		}
		default :
			break;
	}
}

void View::cyclistActionNeighbourRemove(unsigned int actionSide)
{
	switch(actionSide)
	{
		case(TOP) :
		{
			gVideo[(unsigned int) actionCube].bg0.image(vec(6,0), vec(4,3), Movement, vec(6,0));
			break;
		}
		case(LEFT) :
		{
			gVideo[(unsigned int) actionCube].bg0.image(vec(0,6), vec(4,3), Movement, vec(0,6));
			break;
		}
		case(BOTTOM) :
		{
			gVideo[(unsigned int) actionCube].bg0.image(vec(6,13), vec(4,3), Movement, vec(6,13));
			break;
		}
		case(RIGHT) :
		{
			gVideo[(unsigned int) actionCube].bg0.image(vec(13,6), vec(4,3), Movement, vec(13,6));
			break;
		}
		default :
			break;
	}
}

void View::cyclistRoadNeighbourAdd(unsigned int roadID, unsigned int roadSide, unsigned int cyclistSide)
{
	//LOG("RoadNeighbourAdd called with roadID = %d, centreCube = %d, cyclistCube = %d\n",roadID,(unsigned int) centreCube,(unsigned int) cyclistCube);
	gVideo[centreCube].bg1.eraseMask();
	centreCube = roadID;
	gVideo[centreCube].bg1.setMask(BG1Mask::filled(vec(7,7),vec(3,3)));

	gVideo[roadID].orientTo(gVideo[(unsigned int) cyclistCube]);

	//LOG("done orientTo() RoadNeighbourAdd with roadID = %d, centreCube = %d, cyclistCube = %d\n",roadID,(unsigned int) centreCube,(unsigned int) cyclistCube);

	Int2 currPosition = myModel->getPosition();
	Int2 currTopLeft = currPosition - vec(56,56);
	redrawRoad(currPosition, currTopLeft);
}

void View::setActionCube(CubeID cube)
{
	actionCube = cube;
}

void View::setCyclistCube(CubeID cube)
{
	cyclistCube = cube;
}

void View::updateCyclist()
{
	Int2 currPosition = myModel->getPosition();
	//Int2 currTopLeft =  currPosition - vec(56,56);

	if(topLeftRoadCubes[(unsigned int) centreCube].x > currPosition.x + 12)
	{
		//LOG("Failed first updateCyclist test, currPosition.x = %d, topLeft.x = %d\n", currPosition.x, topLeftRoadCubes[(unsigned int) centreCube].x);
		Neighborhood neighbourhood = Neighborhood(centreCube);

		CubeID neighbour = gVideo[centreCube].physicalToVirtual(neighbourhood).cubeAt(LEFT);
		if(neighbour.isDefined() && neighbour != cyclistCube && neighbour != actionCube)
		{
			gVideo[(unsigned int) centreCube].bg1.eraseMask();
			centreCube = neighbour;
			gVideo[(unsigned int) centreCube].bg1.setMask(BG1Mask::filled(vec(7,7),vec(3,3)));
			//updateCyclist();
		}
		else
		{
			Int2 currTopLeft =  topLeftRoadCubes[(unsigned int) centreCube] - vec(128,0);
			redrawRoad(currPosition, currTopLeft);
		}
	}
	else if(topLeftRoadCubes[(unsigned int) centreCube].x + 128 < currPosition.x + 12)
	{
		//LOG("Failed second updateCyclist test\n");
		Neighborhood neighbourhood = Neighborhood(centreCube);
		CubeID neighbour = gVideo[(unsigned int) centreCube].physicalToVirtual(neighbourhood).cubeAt(RIGHT);
		if(neighbour.isDefined() && neighbour != cyclistCube && neighbour != actionCube)
		{
			gVideo[(unsigned int) centreCube].bg1.eraseMask();
			centreCube = neighbour;
			gVideo[(unsigned int) centreCube].bg1.setMask(BG1Mask::filled(vec(7,7),vec(3,3)));
			//updateCyclist();
		}
		else
		{
			Int2 currTopLeft =  topLeftRoadCubes[(unsigned int) centreCube] + vec(128,0);
			redrawRoad(currPosition, currTopLeft);
		}
	}
	else if(topLeftRoadCubes[(unsigned int) centreCube].y > currPosition.y + 12)
	{
		//LOG("Failed third updateCyclist test\n");
		Neighborhood neighbourhood = Neighborhood(centreCube);
		CubeID neighbour = gVideo[(unsigned int) centreCube].physicalToVirtual(neighbourhood).cubeAt(TOP);
		if(neighbour.isDefined() && neighbour != cyclistCube && neighbour != actionCube)
		{
			gVideo[(unsigned int) centreCube].bg1.eraseMask();
			centreCube = neighbour;
			gVideo[(unsigned int) centreCube].bg1.setMask(BG1Mask::filled(vec(7,7),vec(3,3)));
			//updateCyclist();
		}
		else
		{
			Int2 currTopLeft =  topLeftRoadCubes[(unsigned int) centreCube] - vec(0,128);
			redrawRoad(currPosition, currTopLeft);
		}
	}
	else if(topLeftRoadCubes[(unsigned int) centreCube].y + 128 < currPosition.y + 12)
	{
		//LOG("Failed fourth updateCyclist test\n");
		Neighborhood neighbourhood = Neighborhood(centreCube);
		CubeID neighbour = gVideo[(unsigned int) centreCube].physicalToVirtual(neighbourhood).cubeAt(BOTTOM);
		if(neighbour.isDefined() && neighbour != cyclistCube && neighbour != actionCube)
		{
			gVideo[(unsigned int) centreCube].bg1.eraseMask();
			centreCube = neighbour;
			gVideo[(unsigned int) centreCube].bg1.setMask(BG1Mask::filled(vec(7,7),vec(3,3)));
			//updateCyclist();
		}
		else
		{
			Int2 currTopLeft =  topLeftRoadCubes[(unsigned int) centreCube] + vec(0,128);
			redrawRoad(currPosition, currTopLeft);
		}
	}
	else
	{
		//LOG("Drawing cyclist from updateCyclist()\n");
		drawCyclist(currPosition);
	}
}

void View::redrawRoad(Int2 currPosition, Int2 currTopLeft)
{
	for(int i=0; i < CUBE_ALLOCATION; ++i)
	{
		alreadyUpdated[i] = false;
	}
	alreadyUpdated[(unsigned int) cyclistCube] = true;
	alreadyUpdated[(unsigned int) actionCube] = true;

	for(int i=0; i < CUBE_ALLOCATION; ++i)
	{
		if(!alreadyUpdated[i])
		{
			gVideo[i].bg0.image(vec(0,0), BlackImage);
		}
	}

	alreadyUpdated[(unsigned int) centreCube] = true;

	

	findNewTopLefts((unsigned int) centreCube, currTopLeft);

	drawCyclist(currPosition);
}

void View::drawCyclist(Int2 position)
{
	Int2 Pan = topLeftRoadCubes[(unsigned int) centreCube] - position;
	//LOG("position - (%d,%d), topLeftRoadCubes[centreCube] = (%d,%d)\n",position.x,position.y,topLeftRoadCubes[centreCube].x,topLeftRoadCubes[centreCube].y);
	Pan += vec(56,56);
	//LOG("Drawing cyclist in cube %d\n",(unsigned int) centreCube);
	gVideo[(unsigned int) centreCube].bg1.image(vec(7,7), Cyclist);
	//LOG("Pan = (%d,%d)\n",Pan.x,Pan.y);
	gVideo[(unsigned int) centreCube].bg1.setPanning(Pan);
}

void View::drawSingleRoadCube(unsigned int roadID)
{
	//LOG("Drawing cube %d, topLeft = (%d,%d)\n",roadID, topLeftRoadCubes[roadID].x, topLeftRoadCubes[roadID].y);
	Int2 bottomRightDraw = vec(128,128);
	if(topLeftRoadCubes[roadID].x + bottomRightDraw.x > myModel->getRoadImageWidth())
	{
		bottomRightDraw.x = myModel->getRoadImageWidth() - topLeftRoadCubes[roadID].x;
	}
	if(topLeftRoadCubes[roadID].y + bottomRightDraw.y > myModel->getRoadImageHeight())
	{
		bottomRightDraw.y = myModel->getRoadImageHeight() - topLeftRoadCubes[roadID].y;
	}

	Int2 topLeftDraw = vec(0,0);
	if(topLeftRoadCubes[roadID].x < 0)
	{
		topLeftDraw.x = -topLeftRoadCubes[roadID].x;
	}
	if(topLeftRoadCubes[roadID].y < 0)
	{
		topLeftDraw.y = -topLeftRoadCubes[roadID].y;
	}

	//LOG("topLeftDraw = (%d,%d), bottomRightDraw = (%d,%d)\n",topLeftDraw.x, topLeftDraw.y, bottomRightDraw.x, bottomRightDraw.y);
	Int2 imageSize = bottomRightDraw - topLeftDraw;

	imageSize.x = (imageSize.x / 8) + 2;
	imageSize.y = (imageSize.y / 8) + 2;

	gVideo[roadID].bg0.image(vec(0,0), BlackImage);
	if(topLeftRoadCubes[roadID].x + bottomRightDraw.x > 0 && topLeftRoadCubes[roadID].y + bottomRightDraw.y > 0)
	{
		gVideo[roadID].bg0.image(vec(topLeftDraw.x / 8,topLeftDraw.y / 8),
			imageSize, RoadView,
			vec((topLeftRoadCubes[roadID].x / 8) + (topLeftDraw.x / 8),
				(topLeftRoadCubes[roadID].y / 8) + (topLeftDraw.y / 8)));

		int xPan = topLeftRoadCubes[roadID].x % 8;
		int yPan = topLeftRoadCubes[roadID].y % 8;
		gVideo[roadID].bg0.setPanning(vec(xPan, yPan));
	}
	alreadyUpdated[roadID] = true;
}

void View::findNewTopLefts(unsigned int roadID, Int2 topLeftImage)
{
	//LOG("setting topLeftRoadCubes[%d] = (%d,%d)\n",roadID,topLeftImage.x,topLeftImage.y);
	topLeftRoadCubes[roadID] = topLeftImage;

	drawSingleRoadCube(roadID);

	Neighborhood neighbourhood = Neighborhood(roadID);

	CubeID neighbour = gVideo[roadID].physicalToVirtual(neighbourhood).cubeAt(TOP);
	if(neighbour.isDefined() && !alreadyUpdated[(unsigned int) neighbour])
	{
		gVideo[(unsigned int) neighbour].orientTo(gVideo[roadID]);
		alreadyUpdated[(unsigned int) neighbour] = true;
		findNewTopLefts((unsigned int) neighbour, topLeftRoadCubes[roadID] + vec(0,-128));
	}
	neighbour = gVideo[roadID].physicalToVirtual(neighbourhood).cubeAt(BOTTOM);
	if(neighbour.isDefined() && !alreadyUpdated[(unsigned int) neighbour])
	{
		gVideo[(unsigned int) neighbour].orientTo(gVideo[roadID]);
		alreadyUpdated[(unsigned int) neighbour] = true;
		findNewTopLefts((unsigned int) neighbour, topLeftRoadCubes[roadID] + vec(0,128));
	}
	neighbour = gVideo[roadID].physicalToVirtual(neighbourhood).cubeAt(LEFT);
	if(neighbour.isDefined() && !alreadyUpdated[(unsigned int) neighbour])
	{
		gVideo[(unsigned int) neighbour].orientTo(gVideo[roadID]);
		alreadyUpdated[(unsigned int) neighbour] = true;
		//LOG("Calling findNewTopLefts() to neighbour on the left\n");
		findNewTopLefts((unsigned int) neighbour, topLeftRoadCubes[roadID] + vec(-128,0));
	}
	neighbour = gVideo[roadID].physicalToVirtual(neighbourhood).cubeAt(RIGHT);
	if(neighbour.isDefined() && !alreadyUpdated[(unsigned int) neighbour])
	{
		gVideo[(unsigned int) neighbour].orientTo(gVideo[roadID]);
		alreadyUpdated[(unsigned int) neighbour] = true;
		findNewTopLefts((unsigned int) neighbour, topLeftRoadCubes[roadID] + vec(128,0));
	}
}
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

    gVideo[cyclistCube].bg0.image(vec(0,0), Bradley);
    gVideo[actionCube].bg0.image(vec(0,0), Movement);

	Int2 currPosition = myModel->getPosition();

	gVideo[centreCube].bg1.setMask(BG1Mask::filled(vec(7,7),vec(3,3)));
	redrawRoad(centreCube, currPosition);
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
		Neighborhood roadOldN = Neighborhood(cube0Id);
		Neighborhood roadNewN = Neighborhood(cube1Id);

		gVideo[cube1Id].orientTo(roadNewN, gVideo[cube0Id], roadOldN);

		switch(side0)
		{
			case(TOP) :
			{
				topLeftRoadCubes[cube1Id] = topLeftRoadCubes[cube0Id] + vec(0,-128);
				break;
			}
			case(LEFT) :
			{
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
		Neighborhood roadOldN = Neighborhood(cube1Id);
		Neighborhood roadNewN = Neighborhood(cube0Id);

		gVideo[cube0Id].orientTo(roadNewN, gVideo[cube1Id], roadOldN);

		switch(side1)
		{
			case(TOP) :
			{
				topLeftRoadCubes[cube0Id] = topLeftRoadCubes[cube1Id] + vec(0,-128);
				break;
			}
			case(LEFT) :
			{
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
		redrawRoad(centreCube, currPosition);
	}
}

void View::cyclistActionNeighbourAdd(unsigned int actionSide)
{
	myModel->userMove(actionSide);

	switch(actionSide)
	{
		case(TOP) :
		{
			gVideo[actionCube].bg0.image(vec(6,0), vec(4,3), MovementHighlight, vec(6,0));
			break;
		}
		case(LEFT) :
		{
			gVideo[actionCube].bg0.image(vec(0,6), vec(4,3), MovementHighlight, vec(0,6));
			break;
		}
		case(BOTTOM) :
		{
			gVideo[actionCube].bg0.image(vec(6,13), vec(4,3), MovementHighlight, vec(6,13));
			break;
		}
		case(RIGHT) :
		{
			gVideo[actionCube].bg0.image(vec(13,6), vec(4,3), MovementHighlight, vec(13,6));
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
			gVideo[actionCube].bg0.image(vec(6,0), vec(4,3), Movement, vec(6,0));
			break;
		}
		case(LEFT) :
		{
			gVideo[actionCube].bg0.image(vec(0,6), vec(4,3), Movement, vec(0,6));
			break;
		}
		case(BOTTOM) :
		{
			gVideo[actionCube].bg0.image(vec(6,13), vec(4,3), Movement, vec(6,13));
			break;
		}
		case(RIGHT) :
		{
			gVideo[actionCube].bg0.image(vec(13,6), vec(4,3), Movement, vec(13,6));
			break;
		}
		default :
			break;
	}
}

void View::cyclistRoadNeighbourAdd(unsigned int roadID, unsigned int roadSide, unsigned int cyclistSide)
{
	centreCube = roadID;

	Neighborhood roadN = Neighborhood(roadID);
	Neighborhood cyclistN = Neighborhood((unsigned int) cyclistCube);

	gVideo[roadID].orientTo(roadN, gVideo[cyclistCube], cyclistN);

	Int2 currPosition = myModel->getPosition();
	redrawRoad(centreCube, currPosition);
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

	if(topLeftRoadCubes[centreCube].x > currPosition.x + 12)
	{
		Neighborhood neighbourhood = Neighborhood(centreCube);
		CubeID neighbour = neighbourhood.cubeAt(LEFT);
		if(neighbour.isDefined())
		{
			gVideo[centreCube].bg1.eraseMask();
			centreCube = neighbour;
			gVideo[centreCube].bg1.setMask(BG1Mask::filled(vec(7,7),vec(3,3)));
			updateCyclist();
		}
		else
		{
			redrawRoad(centreCube, currPosition);
		}
	}
	else if(topLeftRoadCubes[centreCube].x + 128 < currPosition.x + 12)
	{
		Neighborhood neighbourhood = Neighborhood(centreCube);
		CubeID neighbour = neighbourhood.cubeAt(RIGHT);
		if(neighbour.isDefined())
		{
			gVideo[centreCube].bg1.eraseMask();
			centreCube = neighbour;
			gVideo[centreCube].bg1.setMask(BG1Mask::filled(vec(7,7),vec(3,3)));
			updateCyclist();
		}
		else
		{
			redrawRoad(centreCube, currPosition);
		}
	}
	else if(topLeftRoadCubes[centreCube].y > currPosition.y + 12)
	{
		Neighborhood neighbourhood = Neighborhood(centreCube);
		CubeID neighbour = neighbourhood.cubeAt(TOP);
		if(neighbour.isDefined())
		{
			gVideo[centreCube].bg1.eraseMask();
			centreCube = neighbour;
			gVideo[centreCube].bg1.setMask(BG1Mask::filled(vec(7,7),vec(3,3)));
			updateCyclist();
		}
		else
		{
			redrawRoad(centreCube, currPosition);
		}
	}
	else if(topLeftRoadCubes[centreCube].y + 128 < currPosition.y + 12)
	{
		LOG("Going into cube below, topLeft = %d, currPosition.y = %d\n"
			, topLeftRoadCubes[centreCube].y, currPosition.y);
		Neighborhood neighbourhood = Neighborhood(centreCube);
		CubeID neighbour = neighbourhood.cubeAt(BOTTOM);
		if(neighbour.isDefined())
		{
			LOG("Erasing mask\n");
			gVideo[centreCube].bg1.eraseMask();
			centreCube = neighbour;
			gVideo[centreCube].bg1.setMask(BG1Mask::filled(vec(7,7),vec(3,3)));
			LOG("New centreCube has topLeft = %d\n", topLeftRoadCubes[centreCube].y);
			LOG("Going to update cyclist\n");
			//updateCyclist();
		}
		else
		{
			redrawRoad(centreCube, currPosition);
		}
	}
	else
	{
		drawCyclist(currPosition);
	}
}

void View::redrawRoad(unsigned int roadID, Int2 currPosition)
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

	alreadyUpdated[roadID] = true;

	Int2 topLeftImage = currPosition - vec(56,56);

	findNewTopLefts(roadID, topLeftImage);

	drawCyclist(currPosition);
}

void View::drawCyclist(Int2 position)
{
	Int2 Pan = topLeftRoadCubes[centreCube] - position;
	//LOG("position - (%d,%d), topLeftRoadCubes[centreCube] = (%d,%d)\n",position.x,position.y,topLeftRoadCubes[centreCube].x,topLeftRoadCubes[centreCube].y);
	Pan += vec(56,56);
	gVideo[centreCube].bg1.image(vec(7,7), Cyclist);
	//LOG("Pan = (%d,%d)\n",Pan.x,Pan.y);
	gVideo[centreCube].bg1.setPanning(Pan);
}

void View::drawSingleRoadCube(unsigned int roadID)
{
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

	Int2 imageSize = bottomRightDraw - topLeftDraw;

	imageSize.x = (imageSize.x / 8) + 2;
	imageSize.y = (imageSize.y / 8) + 2;

	gVideo[roadID].bg0.image(vec(0,0), BlackImage);
	if(topLeftRoadCubes[roadID].x + bottomRightDraw.x > 0 && topLeftRoadCubes[roadID].y + bottomRightDraw.y > 0)
	{
		gVideo[roadID].bg0.image(vec(topLeftDraw.x / 8,topLeftDraw.y / 8),
			imageSize, RoadView, vec(topLeftRoadCubes[roadID].x / 8,topLeftRoadCubes[roadID].y / 8));

		int xPan = topLeftRoadCubes[roadID].x % 8;
		int yPan = topLeftRoadCubes[roadID].y % 8;
		gVideo[roadID].bg0.setPanning(vec(xPan, yPan));
	}
	alreadyUpdated[roadID] = true;
}

void View::findNewTopLefts(unsigned int roadID, Int2 topLeftImage)
{
	topLeftRoadCubes[roadID] = topLeftImage;

	drawSingleRoadCube(roadID);

	Neighborhood neighbourhood = Neighborhood(roadID);

	CubeID neighbour = neighbourhood.cubeAt(TOP);
	if(neighbour.isDefined() && !alreadyUpdated[(unsigned int) neighbour])
	{
		Neighborhood thisN = Neighborhood((unsigned int) neighbour);
		gVideo[(unsigned int) neighbour].orientTo(thisN, gVideo[roadID], neighbourhood);
		alreadyUpdated[(unsigned int) neighbour] = true;
		findNewTopLefts((unsigned int) neighbour, topLeftRoadCubes[roadID] + vec(0,-128));
	}
	neighbour = neighbourhood.cubeAt(BOTTOM);
	if(neighbour.isDefined() && !alreadyUpdated[(unsigned int) neighbour])
	{
		Neighborhood thisN = Neighborhood((unsigned int) neighbour);
		gVideo[(unsigned int) neighbour].orientTo(thisN, gVideo[roadID], neighbourhood);
		alreadyUpdated[(unsigned int) neighbour] = true;
		findNewTopLefts((unsigned int) neighbour, topLeftRoadCubes[roadID] + vec(0,128));
	}
	neighbour = neighbourhood.cubeAt(LEFT);
	if(neighbour.isDefined() && !alreadyUpdated[(unsigned int) neighbour])
	{
		Neighborhood thisN = Neighborhood((unsigned int) neighbour);
		gVideo[(unsigned int) neighbour].orientTo(thisN, gVideo[roadID], neighbourhood);
		alreadyUpdated[(unsigned int) neighbour] = true;
		findNewTopLefts((unsigned int) neighbour, topLeftRoadCubes[roadID] + vec(-128,0));
	}
	neighbour = neighbourhood.cubeAt(RIGHT);
	if(neighbour.isDefined() && !alreadyUpdated[(unsigned int) neighbour])
	{
		Neighborhood thisN = Neighborhood((unsigned int) neighbour);
		gVideo[(unsigned int) neighbour].orientTo(thisN, gVideo[roadID], neighbourhood);
		alreadyUpdated[(unsigned int) neighbour] = true;
		findNewTopLefts((unsigned int) neighbour, topLeftRoadCubes[roadID] + vec(128,0));
	}
}
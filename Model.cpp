#include "Model.h"

Model::Model()
{
	currPosition = vec(135,135);
	futurePosition = currPosition;

	topLeftMap = vec(55,0);
	bottomRightMap = vec(215,275);

	roadImageWidth = 296;
	roadImageHeight = 296;
}

void Model::updateMove(TimeDelta delta)
{
	int milliSecs = delta.milliseconds();

	Float2 movementVec = futurePosition - currPosition;
	int movementLen = movementVec.len();

	double maxMovement = PIXELS_PER_SECOND * (milliSecs / 1000.0);

	if(maxMovement > movementLen)
	{
		currPosition = futurePosition;
	}
	else
	{
		currPosition = currPosition + (Int2) (movementVec.normalize() * maxMovement);
	}
}

void Model::userMove(int direction)
{
	Int2 attPosition;
	switch(direction)
	{
		case(TOP) :
		{
			attPosition = currPosition + vec(0,-50);
			break;
		}
		case(LEFT) :
		{
			attPosition = currPosition + vec(-50,0);
			break;
		}
		case(BOTTOM) :
		{
			attPosition = currPosition + vec(0,50);
			break;
		}
		case(RIGHT) :
		{
			attPosition = currPosition + vec(50,0);
			break;
		}
		default :
			break;
	}
}

Int2 Model::getPosition()
{
	return currPosition;
}

int Model::getRoadImageWidth()
{
	return roadImageWidth;
}

int Model::getRoadImageHeight()
{
	return roadImageHeight;
}
#ifndef MODEL_H
#define MODEL_H
#include <sifteo.h>
#define PIXELS_PER_SECOND 100
using namespace Sifteo;

class Model
{

private:
    Int2 currPosition;
    Int2 futurePosition;

    Int2 topLeftMap;
    Int2 bottomRightMap;

    int roadImageWidth;
    int roadImageHeight;
    
public:
    Model();

    void updateMove(TimeDelta delta);
    void userMove(int direction);

    Int2 getPosition();
    int getRoadImageWidth();
    int getRoadImageHeight();
};

#endif
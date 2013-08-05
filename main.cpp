#include <sifteo.h>
#include "Model.h"
#include "View.h"
#include "Controller.h"
using namespace Sifteo;

static AssetSlot MainSlot = AssetSlot::allocate()
	.bootstrap(GameAssets);

static Metadata M = Metadata()
	.title("TdF_prototype")
	.package("max.steele","1.0")
	.icon(Icon)
	.cubeRange(1,CUBE_ALLOCATION);

static AssetLoader loader;
static AssetConfiguration<1> assetConfig;

Controller controller = Controller();

void onDisconnect(void *x, unsigned int id)
{
	controller.onDisconnect(id);
}

void onConnect(void *x, unsigned int id)
{
	controller.onConnect(id);
}

void onNeighbourAdd(void *x,unsigned int cube0Id, unsigned int side0,
    unsigned int cube1Id, unsigned int side1)
{
	controller.onNeighbourAdd(cube0Id, side0, cube1Id, side1);
}
void onNeighbourRemove(void *x,unsigned int cube0Id, unsigned int side0,
    unsigned int cube1Id, unsigned int side1)
{
	controller.onNeighbourRemove(cube0Id, side0, cube1Id, side1);
}
void onTouch(void *x,unsigned int cube)
{
	controller.onTouch(cube);
}

void main()
{
    assetConfig.append(MainSlot, GameAssets);
    loader.init();

    Events::cubeConnect.set(&onConnect);
    Events::cubeDisconnect.set(&onDisconnect);
    Events::cubeTouch.set(&onTouch);
    Events::neighborAdd.set(&onNeighbourAdd);
    Events::neighborRemove.set(&onNeighbourRemove);

    Model model = Model();

    controller = Controller(&model);

    TimeStep ts;
	while(1)
    {
    	controller.updateTime(ts.delta());

    	ts.next();
    	System::paint();
    }
}

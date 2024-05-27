#include "Map.h"
#include "Path.h"


Path checkPointPath;
Map mapPathCheckpoint;
CheckPointDirection checkpointDirection;





void setMap()
{
	Checkpoint checkPoint;

	checkPoint.id = 1;
	checkPoint.front_id = 2;
	checkPoint.back_id = 0;
	checkPoint.left_id = 0;
	checkPoint.right_id = 0;
	mapPathCheckpoint.addCheckPoint(checkPoint);

	checkPoint.id = 2;
	checkPoint.front_id = 4;
	checkPoint.back_id = 1;
	checkPoint.left_id = 0;
	checkPoint.right_id = 3;
	mapPathCheckpoint.addCheckPoint(checkPoint);

	checkPoint.id = 3;
	checkPoint.front_id = 0;
	checkPoint.back_id = 2;
	checkPoint.left_id = 0;
	checkPoint.right_id = 0;
	mapPathCheckpoint.addCheckPoint(checkPoint);

	checkPoint.id = 4;
	checkPoint.front_id = 6;
	checkPoint.back_id = 2;
	checkPoint.left_id = 5;
	checkPoint.right_id = 0;
	mapPathCheckpoint.addCheckPoint(checkPoint);

	checkPoint.id = 5;
	checkPoint.front_id = 0;
	checkPoint.back_id = 4;
	checkPoint.left_id = 0;
	checkPoint.right_id = 0;
	mapPathCheckpoint.addCheckPoint(checkPoint);

	checkPoint.id = 6;
	checkPoint.front_id = 0;
	checkPoint.back_id = 4;
	checkPoint.left_id = 0;
	checkPoint.right_id = 0;
	mapPathCheckpoint.addCheckPoint(checkPoint);

	mapPathCheckpoint.setPreviousCheckPoint(5);
	mapPathCheckpoint.setNextCheckPoint(4);

	checkPointPath = mapPathCheckpoint.findPath(3);
}


int main() {
	setMap();
	checkpointDirection = checkPointPath.getNextDirection();
	checkPointPath.goNextCheckPoint();
	checkpointDirection = checkPointPath.getNextDirection();
	checkPointPath.goNextCheckPoint();
	checkpointDirection = checkPointPath.getNextDirection();
	checkPointPath.goNextCheckPoint();
	checkpointDirection = checkPointPath.getNextDirection();
	checkPointPath.goNextCheckPoint();
	return 0;
}

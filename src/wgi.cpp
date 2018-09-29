#include "stdafx.h"
#include "wgi.h"

void wgi::SetCamera(WGICamera* targetCam, float hFov, float vFov, float dist)
{
	targetCam->horizFOV = hFov;
	targetCam->vertFOV = vFov;
	targetCam->viewDist = dist;
}
void wgi::GetCamRay(const WGICamera* camData, int scrX, int scrY, int scrWidth, int scrHeight, Vec3f* vecOut)
{
	float alpha = camData->horizFOV / 2.0f;
	float beta = camData->vertFOV / 2.0f;
	float xPart = camData->viewDist * tan(PI * alpha / 180.0f);
	float zPart = camData->viewDist * tan(PI * beta / 180.0f);
	(*vecOut)[0] = xPart + ((xPart * 2) * ((float)scrX / scrWidth));
	(*vecOut)[1] = camData->viewDist;
	(*vecOut)[2] = zPart + ((zPart * 2) * ((float)scrY / scrHeight));
}
void wgi::GetMeshBounds(const WGIMesh* meshData, WGIBounds* meshBounds)
{
	Vec3f minVals;
	Vec3f maxVals;
	Vec3f vertSums;

	vertSums = minVals = maxVals = meshData->vertices[0];

	for (int i = 1; i < meshData->vertices.size(); i++)
	{
		Vec3f vertIter = meshData->vertices[i];
		if (vertIter[0] < minVals[0])
			minVals[0] = vertIter[0];
		else if (vertIter[0] > maxVals[0])
			maxVals[0] = vertIter[0];
		if (vertIter[1] < minVals[1])
			minVals[1] = vertIter[1];
		else if (vertIter[1] > maxVals[1])
			maxVals[1] = vertIter[1];
		if (vertIter[2] < minVals[2])
			minVals[2] = vertIter[2];
		else if (vertIter[2] > maxVals[2])
			maxVals[2] = vertIter[2];

		vertSums[0] += vertIter[0];
		vertSums[1] += vertIter[1];
		vertSums[2] += vertIter[2];
	}

	meshBounds->min = minVals;
	meshBounds->max = maxVals;
	meshBounds->deltas[0] = maxVals[0] - minVals[0];
	meshBounds->deltas[1] = maxVals[1] - minVals[1];
	meshBounds->deltas[2] = maxVals[2] - minVals[2];
	meshBounds->sums = vertSums;
}
bool wgi::IsBoundsVisible(const WGIRay* rayData, const WGIBounds* boundsData)
{
	return (rayData->rayVec[0] >= boundsData->min[0] &&
		rayData->rayVec[1] >= boundsData->min[1] &&
		rayData->rayVec[2] >= boundsData->min[2] &&

		rayData->rayVec[0] <= boundsData->max[0] &&
		rayData->rayVec[2] <= boundsData->max[2]);
}

bool wgi::CheckRayCollision(const WGIRay* rayData, Vec3f vertex)
{
	return (ceil(rayData->rayVec[0]) == vertex[0] && rayData->rayVec[1] >= vertex[1] && ceil(rayData->rayVec[2]) == vertex[2]);
}
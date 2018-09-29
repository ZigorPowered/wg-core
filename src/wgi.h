#pragma once
/*
wgi.h
Header for implementations of Windows Graphical Interface
*/

#define PI 3.141592f

typedef std::array<float, 2> Vec2f;
typedef std::array<float, 3> Vec3f;
typedef std::array<float, 4> Vec4f;
typedef std::array<int, 2> Vec2i;
typedef std::array<int, 3> Vec3i;
typedef std::array<int, 4> Vec4i;

enum WGIRayState;
struct WGIRay;
struct WGICamera;
struct WGIBounds;
struct WGIMesh;
struct WGIPtLight;

enum WGIRayState
{
	CAMERA,
	NON_CAMERA,
	SHADOW,
	REFLECT,
	AMBIENT
};

struct WGIRay
{
	Vec3f rayVec;
	int rayState;
};

struct WGICamera
{
	float horizFOV;
	float vertFOV;
	float viewDist;
};

struct WGIBounds
{
	Vec3f min;
	Vec3f max;
	Vec3f deltas;
	Vec3f sums;
};

struct WGIMesh
{
	std::vector<Vec3f> vertices;
};

struct WGILight
{
	Vec3f pos;
	float strength;
	float radius;
};

namespace wgi
{
	/*
	SetCamera: sets camera settings
	@params:
		WGICamera* targetCam: ptr to target cam
		float hFov: horizontal fov
		float vFov: vertical fov
		float dist: cam's max viewing dist
	@returns: 
		N/A
	*/
	void SetCamera(WGICamera* targetCam, float hFov, float vFov, float dist);
	/*
	GetCamRay: returns calculated ray vector
	@params:
		WGICamera* camData: ptr to window cam
		int scrX: x origin of ray in terms of screen
		int scrY: y origin of ray in terms of screen
	@returns:
		Vec3f* rayVec: resulting vector
	*/
	void GetCamRay(const WGICamera* camData, int scrX, int scrY, int scrWidth, int scrHeight, Vec3f* rayVec);
	/*
	GetMeshBounds: returns calculated bounds of mesh using vertex data
	@params:
		WGIMesh* meshData: ptr to target mesh
	@returns:
		WGIBounds* meshBounds: resulting bounds
	*/
	void GetMeshBounds(const WGIMesh* meshData, WGIBounds* meshBounds);

	bool IsBoundsVisible(const WGIRay* rayData, const WGIBounds* boundsData);

	bool CheckRayCollision(const WGIRay* rayData, Vec3f vertex);
}
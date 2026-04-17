#pragma once
#include "DrawDebugHelpers.h"

//Define function macro
#define DRAW_DEBUG_SPHERE(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(), Location, 5.f, 12, Color, false, 5.f);
#define DRAW_SPHERE_SingleFrame(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f);
#define DRAW_DEBUG_LINE(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f);
#define DRAW_DEBUG_POINT(Location) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false);
#define DRAW_DEBUG_VECTOR(StartLocation, EndLocation) if(GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, true); \
	} \

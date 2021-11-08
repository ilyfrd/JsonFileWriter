// JsonFileWriter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include "json/json.h"
#include "c_map.h"

using namespace std;
using namespace Json;

struct Rotator
{
	float pitch;
	float yaw;
	float roll;

	Rotator(float pitch, float yaw, float roll)
	{
		this->pitch = pitch;
		this->yaw = yaw;
		this->roll = roll;
	}
};

struct Vector3d
{
	double x;
	double y;
	double z;

	Vector3d() :x(0), y(0), z(0)
	{}

	Vector3d(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

};

Vector3d operator - (const Vector3d& first, const Vector3d& second)
{
	Vector3d result;
	result.x = first.x - second.x;
	result.y = first.y - second.y;
	result.z = first.z - second.z;

	return result;
}

Value ConstructMapOffsetObject(Vector3d mapOffset);
Value ConstructRoadLineObject(Vector3d mapOffset);
Value ConstructTrafficLightData(Vector3d mapOffset);
Value ConstructTrafficLightPoleData(Vector3d mapOffset);




int main()
{
	CMapInit();

	struct MapInfo
	{
		string Name;
		Vector3d Offset;
	};

	const Vector3d DefaultOffset = { 784500.0, 3352800.0, 0.0 };
	vector<MapInfo> vtsMapInfos =
	{
		{string("AITownReconstructed_V0103_200518.xodr"), Vector3d(784533, 3352831, 0.0)},
		{string("ZhejiangLabPark_V0101_210531.xodr"), Vector3d(778196.9573, 3351544.2202, 1.9713)},
		{string("SM_Road_Straight_1_Lane.xodr"), DefaultOffset},
		{string("SM_Road_Straight_2_Lane.xodr"), DefaultOffset},
		{string("SM_Y_Intersection.xodr"), DefaultOffset},
		{string("SM_CrossRoad01.xodr"), DefaultOffset},
		{string("SM_CrossRoad02.xodr"), DefaultOffset},
		{string("SM_T_Intersection.xodr"), DefaultOffset},
		{string("SM_X_Intersection.xodr"), DefaultOffset},
		{string("SM_WindingRoad01.xodr"), DefaultOffset},
		//{string("SM_GradeCrossingRoad01.xodr"), DefaultOffset},
		//{string("SM_GradeCrossingRoad02.xodr"), DefaultOffset},
		{string("SM_RotaryIntersection.xodr"), DefaultOffset},
		{string("SM_RoadWithParkingLane.xodr"), DefaultOffset},
		{string("SM_Left-turnWaitingArea.xodr"), DefaultOffset},
		{string("SM_DecelerationLane.xodr"), DefaultOffset},
		{string("SM_Gradient.xodr"), DefaultOffset},
		{string("SM_Converging01.xodr"), DefaultOffset},
		{string("SM_Diverging01.xodr"), DefaultOffset},
		{string("SM_Diverging02.xodr"), DefaultOffset},
		{string("SM_Converging02.xodr"), DefaultOffset}
	};

	for (const auto& vtsMapInfo : vtsMapInfos)
	{
		string xodr_file_path = string("./xodr_files/") + vtsMapInfo.Name;
		CErrorCode Ret = CMapLoad(xodr_file_path.c_str());
		if (Ret != 0)
		{
			continue;
		}

		Json::Value rootObject;
		rootObject["offset"] = ConstructMapOffsetObject(vtsMapInfo.Offset);
		rootObject["roadLine"] = ConstructRoadLineObject(vtsMapInfo.Offset);
		rootObject["trafficLight"] = ConstructTrafficLightData(vtsMapInfo.Offset);
		rootObject["trafficLightPole"] = ConstructTrafficLightPoleData(vtsMapInfo.Offset);

		std::ofstream fileStream;
		string json_file_path = string("./json_files/") + vtsMapInfo.Name + string(".json");
		fileStream.open(json_file_path);
		Json::StyledWriter styledWriter;
		fileStream << styledWriter.write(rootObject);
		fileStream.close();
	}

    std::cout << "Hello World!\n";
}

Value ConstructMapOffsetObject(Vector3d mapOffset)
{
	Value offsetObject;
	offsetObject["x"] = mapOffset.x;
	offsetObject["y"] = mapOffset.y;
	offsetObject["z"] = mapOffset.z;

	return offsetObject;
}

Value ConstructRoadLineObject(Vector3d mapOffset)
{
	CLaneUIdArray AllLaneIds = EmptyLaneUIdArray;
	CMapGetAllLaneIds(&AllLaneIds);

	Value VtsMapData(arrayValue);

	for (uint32_t i = 0; i < AllLaneIds.length; i++)
	{
		CSLZArray LeftBoundary = EmptySLZArray;
		CSLZArray RightBoundary = EmptySLZArray;
		double SamplingSpacing = 0.5;
		CMapQueryLaneBoundaries(AllLaneIds.array[i], SamplingSpacing, &LeftBoundary, &RightBoundary);

		auto FillVertexData = [&VtsMapData, &mapOffset](const CSLZArray& Boundary, bool IsLeftBoundary)
		{
			Value BoundaryPoints(arrayValue);

			for (uint32_t j = 0; j < Boundary.length; j++)
			{
				// 获取车道线坐标
				CXYZ Coordinate;
				CMapCalcXYZ(Boundary.array[j], &Coordinate);

				Vector3d CurrentVertex =  Vector3d(Coordinate.x, Coordinate.y, 0) - mapOffset;
				Value Point;
				Point["x"] = CurrentVertex.x;
				Point["y"] = CurrentVertex.y;

				// 获取车道线类型信息
				CSLZ BoundaryPoint = Boundary.array[j];
				char* LeftRoadMark = nullptr;
				int LeftLength = 0;
				char* RightRoadMark = nullptr;
				int RightLength = 0;
				CMapQueryLaneRoadMark(BoundaryPoint.lane_uid, BoundaryPoint.s, &LeftRoadMark, &LeftLength, &RightRoadMark, &RightLength);

				if (IsLeftBoundary)
				{
					if (LeftLength != 0)
					{
						std::stringstream Mark(LeftRoadMark);
						std::vector<std::string> seglist;
						std::string segment;
						while (getline(Mark, segment, ' '))
						{
							seglist.push_back(segment);
						}

						Point["LineType"] = seglist[0];
						Point["LineColor"] = seglist[1];
					}
				}
				else
				{
					if (RightLength != 0)
					{
						std::stringstream Mark(RightRoadMark);
						std::vector<std::string> seglist;
						std::string segment;
						while (getline(Mark, segment, ' '))
						{
							seglist.push_back(segment);
						}

						Point["LineType"] = seglist[0];
						Point["LineColor"] = seglist[1];
					}
				}

				BoundaryPoints.append(Point);
			}

			Value BoundaryJsonObject;
			BoundaryJsonObject["RoadId"] = Boundary.array[Boundary.length / 2].lane_uid.road_id; // 获取边界线中间点所对应的RoadId
			BoundaryJsonObject["BoundaryPoints"] = BoundaryPoints;

			VtsMapData.append(BoundaryJsonObject);
		};

		FillVertexData(LeftBoundary, true);
		FillVertexData(RightBoundary, false);
	}

	return VtsMapData;
}


Value ConstructTrafficLightData(Vector3d mapOffset)
{
	CTrafficLightGroupArray TrafficLightGroupArray = { 0, nullptr };
	CErrorCode Ret = CMapQueryAllTrafficLightGroups(&TrafficLightGroupArray);
	if (Ret != 0) Value();

	Value TrafficLight;

	for (uint32_t i = 0; i < TrafficLightGroupArray.length; ++i)
	{
		CTrafficLightGroup TrafficLightGroup = TrafficLightGroupArray.array[i];
		CTrafficLightIdArray TrafficLightIdArray = TrafficLightGroup.group_members;
		for (uint32_t j = 0; j < TrafficLightIdArray.length; ++j)
		{
			CTrafficLightId TrafficLightId = TrafficLightIdArray.array[j];
			CTrafficLight TrafficLightInfo;
			CMapGetTrafficLightInfoById(TrafficLightId, &TrafficLightInfo);

			CLaneUId lane_uid;
			lane_uid.road_id = TrafficLightInfo.position_road_id;
			CSLZ slz;
			slz.lane_uid = lane_uid;
			slz.s = TrafficLightInfo.s;
			slz.l = TrafficLightInfo.l;

			CXYZ Coordinate;
			CMapCalcXYZ(slz, &Coordinate);
			Vector3d Location = Vector3d(Coordinate.x, Coordinate.y, 0) - mapOffset;
			Location.z = TrafficLightInfo.zOffset;

			Rotator Rotation = Rotator(TrafficLightInfo.pitch, TrafficLightInfo.hdg, TrafficLightInfo.roll);

			Value TrafficLightData;
			TrafficLightData["x"] = Location.x;
			TrafficLightData["y"] = Location.y;
			TrafficLightData["z"] = Location.z;

			TrafficLightData["pitch"] = Rotation.pitch;
			TrafficLightData["yaw"] = Rotation.yaw;
			TrafficLightData["roll"] = Rotation.roll;

			TrafficLight[TrafficLightId] = TrafficLightData;
		}
	}

	return TrafficLight;
}

Value ConstructTrafficLightPoleData(Vector3d mapOffset)
{
	CTrafficLightGroupArray TrafficLightGroupArray = { 0, nullptr };
	CErrorCode Ret = CMapQueryAllTrafficLightGroups(&TrafficLightGroupArray);
	if (Ret != 0) return Value();

	Value TrafficLightPole;

	for (uint32_t i = 0; i < TrafficLightGroupArray.length; ++i)
	{
		CTrafficLightGroup TrafficLightGroup = TrafficLightGroupArray.array[i];
		CObjectId PoleId = TrafficLightGroup.pole_id;
		CPole PoleInfo;
		Ret = CMapGetPoleInfoById(PoleId, &PoleInfo);
		if (Ret != 0) continue;

		CXYZ Coordinate = PoleInfo.xyz;
		Vector3d Location = Vector3d(Coordinate.x, Coordinate.y, 0) - mapOffset;

		Location.z = PoleInfo.height;

		Rotator Rotation = Rotator(PoleInfo.pitch, PoleInfo.hdg, PoleInfo.roll);

		Value TrafficLightPoleData;
		TrafficLightPoleData["x"] = Location.x;
		TrafficLightPoleData["y"] = Location.y;
		TrafficLightPoleData["z"] = Location.z;

		TrafficLightPoleData["pitch"] = Rotation.pitch;
		TrafficLightPoleData["yaw"] = Rotation.yaw;
		TrafficLightPoleData["roll"] = Rotation.roll;

		TrafficLightPole[TrafficLightGroup.group_id] = TrafficLightPoleData;
	}

	return TrafficLightPole; 
}




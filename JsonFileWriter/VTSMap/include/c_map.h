#pragma once
#ifndef CMAP_H
#define CMAP_H

#ifdef _WIN32
#ifdef DLL_EXPORT
#define MAP_API __declspec(dllexport)
#else
#define MAP_API __declspec(dllimport)
#endif
#include <cstdint>
#include <cstddef>
#include <cfloat>
#include <climits>
#elif __linux__
#define MAP_API
#include <stdint.h>
#include <stddef.h>
#include <float.h>
#include <limits.h>
#endif


typedef double CRoadS;
typedef double CRouteS;

typedef char* CJunctionId;
typedef const char* CAnchorId;

typedef char* CRoadId ;
typedef uint32_t CSectionIndex;
typedef int32_t CLocalLaneId;

typedef char* CObjectId;


typedef uint32_t CErrorCode;

typedef enum CLaneChangeType
{
	kNoChange = 0,
	kLeftChange = 1,
	kRightChange = 2,
	kBothChange = 4,
	kUnknownChangeType = -1
}CLaneChangeType;



typedef struct CLaneUId {
	CRoadId road_id;
	CSectionIndex section_index;
	CLocalLaneId local_id;
}CLaneUId;

typedef struct CXYZ {
	double x;
	double y;
	double z;
} CXYZ;

typedef struct CSLZ {

	CLaneUId lane_uid;
	CRoadS s;
	double l;
	double z;
} CSLZ;

typedef struct CAnchor {
	CAnchorId anchor_id;
	CSLZ slz;
} CAnchor;

typedef struct CLaneInfo {
	CLaneUId lane_uid;
	CRoadS  begin;
	CRoadS  end;
	CRoadS length;

}CLaneInfo;


typedef struct CLaneUIdArray
{
	uint32_t length;
	CLaneUId* array;
}CLaneUIdArray;

typedef struct CObjectIdArray
{
	uint32_t length;
	CObjectId* array;
}CObjectIdArray;

typedef struct CXYZArray
{
	uint32_t length;
	CXYZ* array;
}CXYZArray;

typedef struct CSLZArray
{
	uint32_t length;
	CSLZ* array;
}CSLZArray;

typedef struct CAnchorArray
{
	uint32_t length;
	CAnchor* array;
}CAnchorArray;

typedef struct CLaneLinkage 
{
	CLaneUIdArray predecessor_lanes;
	CLaneUIdArray successor_lanes;
	CLaneUId left_neighbor;
	CLaneUId right_neighbor;
}CLaneLinkage;

typedef struct CRoute
{
	CLaneUIdArray lane_uids;
	CSLZ 		begin;
	CSLZ 		end;
	CRouteS 	distance;
}CRoute;

typedef struct CTracePoint {
	double x;
	double y;
	double z;
	CLaneUId lane_id;
	CRoadS s;
	double l;
	double hdg;
	double curv;
	double curv_deriv;
}CTracePoint;

typedef struct CTracePointArray {
	uint32_t length;
	CTracePoint* array;
}CTracePointArray;


typedef char* CTrafficLightId;
typedef char* CTrafficLightGroupId;
typedef char* CTrafficLightType;

typedef struct CTrafficLightIdArray
{
	uint32_t length;
	CTrafficLightId* array;
}CTrafficLightIdArray;

/**
	 * @enum LaneTurnType
	 * @brief enumrate the possible lane turning options of a lane, compositions can be refelected by bits
	 *      //	kUnkonwnTurnType	= -1,
			//	kNoTurn				= 0x0,
			//	kForward			= 0x1,
			//	kLeftTurn 			= 0x2,
			//	kRightTurn 			= 0x4,
			//	kUTurn 				= 0x8
	*/
typedef int32_t CLaneTurnType;// NOTE: Standard OpenDrives do not have Turn Type attributes

typedef struct CStopLine
{
	CObjectId id;
	CRoadId road_id;
	CTrafficLightGroupId group_id;
	CXYZArray line_xyz;		// 
	CSLZArray line_slz;		//	
}CStopLine;

typedef struct CPole
{
	CObjectId id;
	CSLZ slz; // Global lane id
	CXYZ xyz;
	double height;
	double hdg;
	double pitch;
	double roll;

}CPole;

typedef struct CTrafficLight {
	CTrafficLightId  id;
	CTrafficLightType type;	// ????????????????????????????????????????????????????????????????????????????????????
	CRoadId position_road_id; // ??????????????????????????????????????????Iddouble zOffset;			// ????????????????????????????????????????????????????
	double s;			// ??????????????????????????????????????????S????????????
	double l;			// ??????????????????????????????????????????l????????????
	double zOffset;			// ????????????????????????????????????????????????????
	uint32_t orientation;   // 1: forward as lane direction; -1,backwards;0:both;
	double hdg;				// ?????????????????????????????????????????yaw??????
	double pitch;			// ????????????????????????????????????????????
	double roll;			// ????????????????????????????????????????????

	CRoadId logical_road_id;	// ????????????????????????????????????????????Id
	CLocalLaneId valid_from;	// ??????????????????????????????????Id???????????????????????????????????
	CLocalLaneId valid_to;	// ??????????????????????????????????Id??????????????????????????????????

	CObjectIdArray reference_list; // ?????????????????????????Object??????id??????list???????????????????????????
}CTrafficLight;

typedef struct CTrafficLightGroup {
	char* group_id;
	CTrafficLightIdArray group_members;
	CObjectId pole_id;
}CTrafficLightGroup;

typedef struct CTrafficLightGroupArray {
	uint32_t length;
	CTrafficLightGroup* array;
}CTrafficLightGroupArray;

const CLaneUId			EmptyLaneUId = { NULL,INT_MAX,0 };
const CJunctionId		EmptyJunctionUId = NULL;
const CSLZ				EmptySLZ = { EmptyLaneUId, DBL_MIN, DBL_MAX };
const CAnchor			EmptyAnchor = { NULL, EmptySLZ };
const CLaneInfo			EmptyLaneInfo = { EmptyLaneUId, 0.0, 0.0, 0.0 };
const CLaneUIdArray		EmptyLaneUIdArray = { 0, NULL };
const CSLZArray			EmptySLZArray = { 0, NULL };
const CXYZArray			EmptyXYZArray = { 0, NULL };
const CAnchorArray		EmptyAnchorArray = { 0, NULL };
const CLaneLinkage		EmptyLaneLinkage = { EmptyLaneUIdArray,EmptyLaneUIdArray ,EmptyLaneUId, EmptyLaneUId };
const CTracePointArray	EmptyTracePointArray = { 0, NULL};
const CTrafficLightGroupArray EmptyTrafficLightGroupArray = {0, NULL};

#ifdef __cplusplus
extern "C" {
#endif

	MAP_API  void		 CMapInit();
	MAP_API  CErrorCode  CMapLoad(const char* file_path);
	MAP_API  CErrorCode  CMapUnload();


	MAP_API  CErrorCode  CMapFindSLZ(CXYZ xyz, CLaneUId hint, CSLZ* slz);
	MAP_API  CErrorCode  CMapFindSLZWithOutHInt(CXYZ xyz, CSLZ* slz);
	MAP_API  CErrorCode  CMapCalcSLZWithRoadId(CRoadId croad_id, CXYZ cxyz, CSLZ* slz);

	MAP_API  CErrorCode  CMapCalcXYZ(CSLZ slz, CXYZ* xyz);


	MAP_API  CAnchor     CCreateAnchor(CAnchorId id, CSLZ pos);
	MAP_API  CLaneUId	 CCreateLaneUId(const char* road_id, CSectionIndex secton_index, CLocalLaneId local_id);
	MAP_API  CSLZ		 CCreateSLZ(CLaneUId* lane_uid, double s, double l);

	MAP_API  CErrorCode  CAddAnchor(CAnchorArray* anchor_list, CAnchor anchor_to_add);
	MAP_API  CErrorCode  CDeleteAnchor(CAnchorArray* anchor_list, CAnchorId id_anchor_to_delete);
	MAP_API  CErrorCode  CFindAnchor(const char* pattern, CAnchorArray anchor_list, CAnchorArray* found_anchors);


	MAP_API CErrorCode  CMapPlanRoute(CAnchorArray anchor_list, CRoute* route);
	MAP_API CErrorCode  CMapQueryLaneInfo(CLaneUId lane_uid, CLaneInfo* lane_info);
	MAP_API CErrorCode  CMapQueryLaneSpeedAt(CLaneUId lane_uid, CRoadS s, double* speed_limit);
	MAP_API CErrorCode  CMapQueryLaneChangeTypeAt(CLaneUId lane_uid, CRoadS s, CLaneChangeType* lane_change_type);
	MAP_API CErrorCode  CMapQueryLaneBoundaries(CLaneUId lane_uid, double sampling_spacing, CSLZArray* left_boundary, CSLZArray* right_boundary);

	MAP_API CErrorCode  CMapCalcLaneCenterLine(CLaneUId lane_uid, double sampling_spacing, CSLZArray* result);
	MAP_API CErrorCode  CMapCalcLaneCenterLineInterval(CLaneUId lane_uid, CRoadS s1, CRoadS s2, double sampling_spacing, CSLZArray* result);
	MAP_API CErrorCode  CMapQueryRoadJunctionId(const char* road_id, CJunctionId* junction_id);
	MAP_API CErrorCode  CMapFindJunctionBoundary(CJunctionId junction_id, CXYZArray* junction_boundary);
	MAP_API bool		CMapIsPointOnRoad(const char* road_id, CXYZ xyz);


	MAP_API void CMapDestory();
	MAP_API void CDestroyLaneUId(CLaneUId* cid_);
	MAP_API void CDestroyJunctionId(CJunctionId* cjid_);
	MAP_API void CDestroySLZ(CSLZ* cslz_);
	MAP_API void CDestroyRoute(CRoute* croute_);
	MAP_API void CDestroySLZArray(CSLZArray* cslz_array_);
	MAP_API void CDestroyXYZArray(CXYZArray* cxyz_array_);
	MAP_API void CDestroyLaneUIdArray(CLaneUIdArray* cid_array_);

	// //--------------------------- ???????????????????????????????????????????????????????????????????????????
	MAP_API CErrorCode  CMapQueryLaneWidthAt(struct CLaneUId id, double s, double* lane_width);
	MAP_API CErrorCode  CMapQueryRoadWidthAt(struct CLaneUId id, double s, double* lane_width);
	MAP_API CErrorCode  CMapCalcRoadHeadingAngle(struct CLaneUId id, double s, double* angle);
	MAP_API CErrorCode  CMapCalcRoadEndHeadingAngle(struct CLaneUId id, double* angle);
	MAP_API CErrorCode  CMapFindRoadMarksBetween(struct CSLZ slz1, struct CSLZ slz2, char** road_mark_list_, int* length_);


	MAP_API bool		CMapIsPointInJunction(CJunctionId junc_id, CXYZ xyz);
	// Check if a point is in any of the junction in the map, search the junction with hint laneUid first
	MAP_API CErrorCode	CMapQueryRoadJunctionIdWithHint(CLaneUId hint, CXYZ xyz, CJunctionId* junction_id);

	// ----------------------------- ????????????????????????????????????????????
	/*MAP_API CErrorCode CMapQueryObjectsInRoad(CRoadId road_id, CObjectInRoadArray* result);*/
	MAP_API CErrorCode CMapQueryLaneLinkage(struct CLaneUId lane_uid, struct CLaneLinkage* lane_linkage);
	MAP_API CErrorCode CMapCalcLaneCenterLineCurv(CLaneUId lane_uid, CRoadS s1, CRoadS s2, double sampling_spacing, CTracePointArray* result);
	// ----------------------------- ????????????????????????????
	MAP_API CErrorCode  CMapGetAllLaneIds(CLaneUIdArray* all_lane_ids);
	MAP_API CErrorCode  CMapQueryLaneRoadMark(CLaneUId lane_uid, double s, char** left_road_mark, int* left_length, char** right_road_mark, int* right_length);
	MAP_API CErrorCode  CMapGetAllBoundaries(CXYZ* xyz, int* length);

	//----------------------------------------------MS8 Traffic Light

	/**
	* @brief ????????????Road Id????????????????????????????????????????????????????Id
	* @param road_id
	* @param traffic_lights_ids ????????????????????????????id vecor
	* @return ErrorCode: 0 success; otherwise error occurs;
	*/
	MAP_API CErrorCode CMapQueryTrafficLightsInRoad(const char* road_id, CTrafficLightIdArray* traffic_lights_ids);


	/**
		* @brief ????????????????????????????????????????????Id
		* @param road_id
		* @param signal_ids ????????????????????????????id vecor
		* @return ErrorCode: 0 success; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapQueryAllTrafficLightGroups(CTrafficLightGroupArray* tlg_array);

	/**
		* @brief ????????????????????SLZ????????????????????????????????????????????????????????????????????????????????????????
		* @param slz
		* @param turn_type ????????????????????????????????????????????????????????????????????????????????junction????????????????LaneTurnType::k;??????????????????????kUnkonwnTurnType??????????????ErrorCode ?????? kOK ?????????????? turn_type = kUnkonwnTurnType??????
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapQueryLaneTurnType(CSLZ slz, CLaneTurnType* turn_type);

	/**
		* @brief ????????????????????SLZ??????????????????????????????????????????????????????????junction??????Id??????????????????????????????????????????????????????????????????????????????????????????????????????
				1. ??????junction?????????????????????????????????????????????distance????????????????????????????junction????????????????????JunctionId = -1??????????????????????????						junctionId
				2. ????????????????junction????????????????????????????junction????????????????????JunctionId = -1??????
		* @param slz
		* @param distance ????????????????????????????
		* @param junc_id ??????????????????junction id
		* @return ErrorCode: 0 success; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapQueryForwardJunctionBySLZ(CSLZ slz, double distance, CJunctionId* cjunc_id);

	/**
		* @brief ????????????????????SLZ???????????????????????????????????????????????????????????????????????????????????????????????
		* @param slz
		* @param distance ????????????????????????????
		* @param type ??????????????????????????????????????????????????????????????????????????????????????????????
		* @param traffic_light_group ????????????????????????????????????????????????distance????????????????????????????????????????????????????EmptyTrafficLightGroup
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapQueryForwardTrafficLightGroupBySLZ(CSLZ slz, double distance, CTrafficLightType type, CTrafficLightGroup* traffic_light_group);

	
	/**
		* @brief ????????????????????SLZ??????????????????????????????????????????????????????????????????????Id??????????????????,????????????EmptyObjectId????????????????????????????????
		* @param slz
		* @param distance ????????????????????????????
		* @param stop_line_id ??????????????????stopline id
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapQueryForwardStopLineBySLZ(CSLZ slz, double distance, CObjectId* stop_line_id);
	
	/**
	* @brief ????????id??????????????????????????????
	* @param traffic_light_id
	* @param traffic_light ??????????????????????????????????????????????????id??????????????????????????????????????EmptyTrafficLight{id = "-1"}
	* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
	*/
	MAP_API CErrorCode CMapGetTrafficLightInfoById(const CTrafficLightId traffic_light_id, CTrafficLight* traffic_light);

	/**
		* @brief ????????id??????????????????????????
		* @param stopline_id
		* @param stopline ??????????????????????????????????????????????????id??????????????????????????????????????EmptyStopLine{id = "-1"}
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapGetStopLineInfoById(const CObjectId stopline_id, CStopLine* stopline);

	/**
		* @brief ????????id????????Pole????????????
		* @param pole_id
		* @param pole ????????????????????????????????????????????id????????????????????????????????EmptyPole{id = "-1"}
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapGetPoleInfoById(const CObjectId pole_id, CPole* pole);

	/**
		* @brief ????????????????????????????id?????????????????????????????????????
		* @param traffic_light_group_id
		* @return TrafficLightGroup?????? ??????????????????????????????????????EmptyTrafficLightGroup
		*/
	MAP_API CErrorCode CMapGetTrafficLightGroupInfoById(const CTrafficLightGroupId traffic_light_group_id, CTrafficLightGroup* tlg_);

	/**
		* @brief ??????????????????????id?????????????????????????????????????????id
		* @param traffic_light_id
		* @param traffic_light_group_id
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapGetTrafficLightGroupIdByMember(const CTrafficLightId traffic_light_id, CTrafficLightGroupId* traffic_light_group_id);

	/**
		* @brief ???????????????????????????????????????????????????????????????????????????????????????????????????
		* @param origin_lane_id ?????????????????????
		* @param target_lane_id ????????????
		* @param turn_type ????????????????????????????????????????????????????????????????????????????????????LaneTurnType::k????????????ErrorCode???0??????kUnkonwnTurnType???
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapQueryLane2LaneTurnType(CLaneUId origin_lane_id, CLaneUId target_lane_id, CLaneTurnType* turn_type);

	/**
		* @brief ??????????????????id????????????????????? id
		* @param traffic_light_group_id
		* @param stopline ????????????????????????????????????id???????????????????????????EmptyStopLine{id = "-1"}
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
	*/
	MAP_API CErrorCode CMapQueryStoplineByTrafficLightGroupId(const CTrafficLightGroupId traffic_light_group_id, CObjectId* stopline_id);

	MAP_API void CDestroyCharPointer(char** cptr_);

	MAP_API void CDestroyObjectId(CObjectId* ccobjid_);

	MAP_API void CDestroyTrafficLightGroup(CTrafficLightGroup* traffic_light_group);

	MAP_API void CDestroyStopLine(CStopLine* stop_line);

	MAP_API void CDestroyPole(CPole* pole);

	MAP_API void CDestroyTrafficLight(CTrafficLight* traffic_light_);

	MAP_API void CDestroyTrafficLightIdArray(CTrafficLightIdArray* traffic_light_ids_);

	MAP_API void CDestroyTrafficLightGroupArray(CTrafficLightGroupArray* traffic_light_groups_);

#ifdef __cplusplus
}
#endif

#endif


// TODO: 2. ??????????????????????ErrorCode??????????

// TODO: 3. ??????????????????????????????????????????int, char*??????????????
// TODO: 8. ????????????????????????????->????????????????????????->????????????????????
// TODO: 9. ??????????????????????char* ??????const char*


// TODO: Astar find_path ?????????????????????????????????????

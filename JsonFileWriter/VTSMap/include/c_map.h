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
	CTrafficLightType type;	// ���̵����ͣ����ֺ��̵����ö�����̬��
	CRoadId position_road_id; // ���̵�ģ��������·Iddouble zOffset;			// ���̵�ģ�����ĵ����ڸ߶�
	double s;			// ���̵�ģ������λ��S����
	double l;			// ���̵�ģ������λ��l����
	double zOffset;			// ���̵�ģ�����ĵ����ڸ߶�
	uint32_t orientation;   // 1: forward as lane direction; -1,backwards;0:both;
	double hdg;				// ���̵�ģ��ƽ�泯��ǣ�yaw��
	double pitch;			// ���̵�ģ��ǰ����б��
	double roll;			// ���̵�ģ�����ҷ�ת��

	CRoadId logical_road_id;	// ���̵���Ч�����õ�·Id
	CLocalLaneId valid_from;	// ���̵���Ч����Id������߽磨����
	CLocalLaneId valid_to;	// ���̵���Ч����Id�����ұ߽磨����

	CObjectIdArray reference_list; // ���̵���ص�Object��id��list���������
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

	// //--------------------------- �������򷢲���������������ӿ�
	MAP_API CErrorCode  CMapQueryLaneWidthAt(struct CLaneUId id, double s, double* lane_width);
	MAP_API CErrorCode  CMapQueryRoadWidthAt(struct CLaneUId id, double s, double* lane_width);
	MAP_API CErrorCode  CMapCalcRoadHeadingAngle(struct CLaneUId id, double s, double* angle);
	MAP_API CErrorCode  CMapCalcRoadEndHeadingAngle(struct CLaneUId id, double* angle);
	MAP_API CErrorCode  CMapFindRoadMarksBetween(struct CSLZ slz1, struct CSLZ slz2, char** road_mark_list_, int* length_);


	MAP_API bool		CMapIsPointInJunction(CJunctionId junc_id, CXYZ xyz);
	// Check if a point is in any of the junction in the map, search the junction with hint laneUid first
	MAP_API CErrorCode	CMapQueryRoadJunctionIdWithHint(CLaneUId hint, CXYZ xyz, CJunctionId* junction_id);

	// ----------------------------- �ڲ����۶�ģ�鿪��ʹ��
	/*MAP_API CErrorCode CMapQueryObjectsInRoad(CRoadId road_id, CObjectInRoadArray* result);*/
	MAP_API CErrorCode CMapQueryLaneLinkage(struct CLaneUId lane_uid, struct CLaneLinkage* lane_linkage);
	MAP_API CErrorCode CMapCalcLaneCenterLineCurv(CLaneUId lane_uid, CRoadS s1, CRoadS s2, double sampling_spacing, CTracePointArray* result);
	// ----------------------------- С��ͼ������
	MAP_API CErrorCode  CMapGetAllLaneIds(CLaneUIdArray* all_lane_ids);
	MAP_API CErrorCode  CMapQueryLaneRoadMark(CLaneUId lane_uid, double s, char** left_road_mark, int* left_length, char** right_road_mark, int* right_length);
	MAP_API CErrorCode  CMapGetAllBoundaries(CXYZ* xyz, int* length);

	//----------------------------------------------MS8 Traffic Light

	/**
	* @brief ����Road Id����ѯ��·�����к��̵Ƶ�Id
	* @param road_id
	* @param traffic_lights_ids �����ĺ��̵�id vecor
	* @return ErrorCode: 0 success; otherwise error occurs;
	*/
	MAP_API CErrorCode CMapQueryTrafficLightsInRoad(const char* road_id, CTrafficLightIdArray* traffic_lights_ids);


	/**
		* @brief ��ѯ��ͼ�����к��̵�Id
		* @param road_id
		* @param signal_ids �����ĺ��̵�id vecor
		* @return ErrorCode: 0 success; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapQueryAllTrafficLightGroups(CTrafficLightGroupArray* tlg_array);

	/**
		* @brief ����һ��SLZλ�ã�������λ������������ת�����͡�
		* @param slz
		* @param turn_type ����ת�����ͣ������ǰ������̲���junction���򷵻�LaneTurnType::k;Ŀǰ������kUnkonwnTurnType��ֻ��ErrorCode �� kOK ʱ���� turn_type = kUnkonwnTurnType��
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapQueryLaneTurnType(CSLZ slz, CLaneTurnType* turn_type);

	/**
		* @brief ����һ��SLZλ�ã�������λ��ǰ����һ��junction��Id����ǰ������һ����·���������ѯ��̣�ֱ����
				1. �鵽junction����ʱ���ۼƾ������distance����Ϊǰ����junction�����ؿ�JunctionId = -1�����򷵻ض�Ӧ						junctionId
				2. ʼ��δ�鵽junction����Ϊǰ����junction�����ؿ�JunctionId = -1��
		* @param slz
		* @param distance ǰ����ѯ����
		* @param junc_id ������junction id
		* @return ErrorCode: 0 success; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapQueryForwardJunctionBySLZ(CSLZ slz, double distance, CJunctionId* cjunc_id);

	/**
		* @brief ����һ��SLZλ�ã�������λ��ǰ�������һ���г����̵���
		* @param slz
		* @param distance ǰ����ѯ����
		* @param type ָ����ѯ�ĺ��̵����ͣ��г������ˣ����г���
		* @param traffic_light_group �����ĺ��̵��飬��ǰ��distance�������޺��̵��飬����EmptyTrafficLightGroup
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapQueryForwardTrafficLightGroupBySLZ(CSLZ slz, double distance, CTrafficLightType type, CTrafficLightGroup* traffic_light_group);

	
	/**
		* @brief ����һ��SLZλ�ã�������λ��ǰ����һ��ֹͣ�ߵ�Id������,����EmptyObjectId�����ַ�����
		* @param slz
		* @param distance ǰ����ѯ����
		* @param stop_line_id ������stopline id
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapQueryForwardStopLineBySLZ(CSLZ slz, double distance, CObjectId* stop_line_id);
	
	/**
	* @brief ͨ��id��ѯ���̵ƶ���
	* @param traffic_light_id
	* @param traffic_light �����ĺ��̵ƶ������޸�id��Ӧ�ĺ��̵ƣ�����EmptyTrafficLight{id = "-1"}
	* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
	*/
	MAP_API CErrorCode CMapGetTrafficLightInfoById(const CTrafficLightId traffic_light_id, CTrafficLight* traffic_light);

	/**
		* @brief ͨ��id��ѯֹͣ�߶���
		* @param stopline_id
		* @param stopline ������ֹͣ�߶������޸�id��Ӧ��ֹͣ�ߣ�����EmptyStopLine{id = "-1"}
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapGetStopLineInfoById(const CObjectId stopline_id, CStopLine* stopline);

	/**
		* @brief ͨ��id��ѯPole����
		* @param pole_id
		* @param pole �����ĵƸ˶������޸�id��Ӧ�ĵƸˣ�����EmptyPole{id = "-1"}
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapGetPoleInfoById(const CObjectId pole_id, CPole* pole);

	/**
		* @brief ͨ�����̵���id��ȡ���̵������
		* @param traffic_light_group_id
		* @return TrafficLightGroup�� ��δ��ѯ�����򷵻�EmptyTrafficLightGroup
		*/
	MAP_API CErrorCode CMapGetTrafficLightGroupInfoById(const CTrafficLightGroupId traffic_light_group_id, CTrafficLightGroup* tlg_);

	/**
		* @brief ͨ�����̵�id��ѯ�����������id
		* @param traffic_light_id
		* @param traffic_light_group_id
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapGetTrafficLightGroupIdByMember(const CTrafficLightId traffic_light_id, CTrafficLightGroupId* traffic_light_group_id);

	/**
		* @brief 输入两个车道，源车道与目标车道，给出从源车道到目标车道的转向类型。
		* @param origin_lane_id 源（起始）车道
		* @param target_lane_id 目标车道
		* @param turn_type 传出转向类型，只有当目标车道是源车道的后继车道时，才返回LaneTurnType::k，否则以ErrorCode非0表示kUnkonwnTurnType。
		* @return ErrorCode: 0 nothing wrong; otherwise error occurs;
		*/
	MAP_API CErrorCode CMapQueryLane2LaneTurnType(CLaneUId origin_lane_id, CLaneUId target_lane_id, CLaneTurnType* turn_type);

	/**
		* @brief 通过红绿灯组id查询停止线对象 id
		* @param traffic_light_group_id
		* @param stopline 传出的停止线对象，若无该id对应的停止线，返回EmptyStopLine{id = "-1"}
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


// TODO: 2. ����дһ��ErrorCodeͷ�ļ�

// TODO: 3. �ж��û��������ͣ�int, char*���֣�
// TODO: 8. �������ӹ�ϵ->�ĸ����ӹ�ϵ->�ڲ��汾��
// TODO: 9. Ԥ�����е�char* ��const char*


// TODO: Astar find_path ע����ܶ��̲߳���ȫ

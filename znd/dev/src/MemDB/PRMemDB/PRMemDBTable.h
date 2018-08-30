#pragma once

#include "PRMemDBDefine.h"
#include "PRMemDBField.h"

// ������		Switching station
// ������		Switchgear Cupboard
// �����		Distribution Room
// ��ʽ���վ	Box-type substation
// ���·�֧��	Cable Branch Box

//	�����а���������ȫ���ݵĽṹ���塣
//	�����а���������ȫ���ݵ��ֵ䶨�塣
//	�ڽ������ݽṹ����ʱ��Ҫ���ģ�
//		�ṹ����
//		�ֵ䶨��
//		�궨��
//	��Ҫ���ģ�
//		д����
//		��ӷ���
//		չʾ����

//	�����ݽṹ��������һЩ�м�������������ֵ��б�������ӳ�ڽṹ�����С�

#if defined(__GNUG__) || defined(__GNUC__)	// GCC������Ԥ����ĺ�
#	ifndef DISALIGN
#		define DISALIGN __attribute__((packed))
#	endif
#else
#	define DISALIGN
#	if (defined(_AIX) || defined(AIX))
#		pragma align(packed)
#	else
#		if (!defined(sun) && !defined(__sun) && !defined(__sun__))
#			pragma pack(push)
#		endif
#	endif
#	pragma pack(1)
#endif

//	������Ҫע�����RESTRICT�еĹؼ��ֱ���ΪCHAR���������CHAR��ͨ��CHARҲ�ܱȽ�
static tagMemDBTable	g_PRTableDictArray[]=
{
	{	PR_SYSTEM,			"System"		,	"", 		"ϵͳ"					, 0	, sizeof(g_PRSystemFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRSystem)		, 1						, PRSystem_Offset			, {	0,	0,							0,					0,						0,	},	g_PRSystemFieldArray,			},
	{	PR_ZONE,			"Zone"			,	"", 		"����"					, 0	, sizeof(g_PRZoneFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRZone)			, PR_MaxZone			, PRZone_Offset				, {	1,	PR_ZONE_NAME,				0,					0,						0,	},	g_PRZoneFieldArray,				},
	{	PR_RADIAL,			"Radial"		,	"", 		"������"				, 0	, sizeof(g_PRRadFieldArray)				/sizeof(tagMemDBField)	, sizeof(tagPRRadial)		, PR_MaxRadial			, PRRadial_Offset			, {	0,	0,							0,					0,						0,	},	g_PRRadFieldArray,				},
	{	PR_ACBUS,			"ACBus"			,	"", 		"����ĸ��"				, 0	, sizeof(g_PRACBusFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRACBus)		, PR_MaxACBus			, PRACBus_Offset			, {	2,	PR_ACBUS_NAME,				PR_ACBUS_KV,		0,						0,	},	g_PRACBusFieldArray,			},
	{	PR_ACLINE,			"ACLine"		,	"", 		"������·"				, 0	, sizeof(g_PRACLineFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRACLine)		, PR_MaxACLine			, PRACLine_Offset			, {	1,	PR_ACLINE_NAME,				0,					0,						0,	},	g_PRACLineFieldArray,			},
	{	PR_WIND,			"Wind"			,	"", 		"��ѹ������"			, 0	, sizeof(g_PRWindFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRWind)			, PR_MaxWind			, PRWind_Offset				, {	1,	PR_WIND_NAME,				0,					0,						0,	},	g_PRWindFieldArray,				},
	{	PR_GENERATOR,		"Generator"		,	"Gen", 		"�����"				, 0	, sizeof(g_PRGeneratorFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRGenerator)	, PR_MaxGenerator		, PRGenerator_Offset		, {	1,	PR_GENERATOR_NAME,			0,					0,						0,	},	g_PRGeneratorFieldArray,		},
	{	PR_POWERLOAD,		"PowerLoad"		,	"Load", 	"����"					, 0	, sizeof(g_PRPowerLoadFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRPowerLoad)	, PR_MaxPowerLoad		, PRPowerLoad_Offset		, {	1,	PR_POWERLOAD_NAME,			0,					0,						0,	},	g_PRPowerLoadFieldArray,		},
	{	PR_HVDC,			"HVDC"			,	"", 		"��ѹֱ��ϵͳ"			, 0	, sizeof(g_PRHVDCFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRHVDC)			, PR_MaxHVDC			, PRHVDC_Offset				, {	1,	PR_HVDC_NAME,				0,					0,						0,	},	g_PRHVDCFieldArray,				},
	{	PR_CONVERTER,		"Converter"		,	"", 		"�任��"				, 0	, sizeof(g_PRConverterFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRConverter)	, PR_MaxConverter		, PRConverter_Offset		, {	1,	PR_CONVERTER_NAME,			0,					0,						0,	},	g_PRConverterFieldArray,		},
	{	PR_DCBUS,			"DCBus"			,	"", 		"ֱ��ĸ��"				, 0	, sizeof(g_PRDCBusFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRDCBus)		, PR_MaxDCBus			, PRDCBus_Offset			, {	1,	PR_DCBUS_NAME,				0,					0,						0,	},	g_PRDCBusFieldArray,			},
	{	PR_DCLINE,			"DCLine"		,	"", 		"ֱ����·"				, 0	, sizeof(g_PRDCLineFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRDCLine)		, PR_MaxDCLine			, PRDCLine_Offset			, {	1,	PR_DCLINE_NAME,				0,					0,						0,	},	g_PRDCLineFieldArray,			},
	{	PR_TCSC,			"TCSC"			,	"", 		"�ɿش�������װ��"		, 0	, sizeof(g_PRTCSCFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRTCSC)			, PR_MaxTCSC			, PRTCSC_Offset				, {	1,	PR_TCSC_NAME,				0,					0,						0,	},	g_PRTCSCFieldArray,				},
	{	PR_UPFC,			"UPFC"			,	"", 		"ͳһ����������"		, 0	, sizeof(g_PRUPFCFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRUPFC)			, PR_MaxUPFC			, PRUPFC_Offset				, {	1,	PR_UPFC_NAME,				0,					0,						0,	},	g_PRUPFCFieldArray,				},
	{	PR_DEVMSTATE,		"DevMState"		,	"", 		"�豸��״̬"			, 0	, sizeof(g_PRDevMStateFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRDevMState)	, PR_MaxDevMState		, PRDevMState_Offset		, {	2,	PR_DEVMSTATE_TYPE,			PR_DEVMSTATE_NAME,	0,						0,	},	g_PRDevMStateFieldArray,		},
	{	PR_ACISLAND,		"ACIsland"		,	"", 		"�����絺"				, 0	, sizeof(g_PRACIslandFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRACIsland)		, PR_MaxACIsland		, PRACIsland_Offset			, {	0,	0,							0,					0,						0,	},	g_PRACIslandFieldArray,			},
	{	PR_ACLINEEDGE,		"ACLineEdge"	,	"", 		"������·��"			, 0	, sizeof(g_PRACLineEdgeFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRACLineEdge)	, PR_MaxACLineEdge		, PRACLineEdge_Offset		, {	3,	PR_ACLINEEDGE_BUS,			PR_ACLINEEDGE_KV,	PR_ACLINEEDGE_NAME,		0,	},	g_PRACLineEdgeFieldArray,		},
	{	PR_WINDEDGE,		"WindEdge"		,	"", 		"��ѹ�������"			, 0	, sizeof(g_PRWindEdgeFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRWindEdge)		, PR_MaxWindEdge		, PRWindEdge_Offset			, {	3,	PR_WINDEDGE_BUS,			PR_WINDEDGE_KV,		PR_WINDEDGE_NAME,		0,	},	g_PRWindEdgeFieldArray,			},
	//{	PR_LOADCURVE,		"LoadCurve"		,	"", 		"��������"				, 0	, sizeof(g_PRLoadCurveFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRLoadCurve)	, PR_MaxLoadCurve		, PRLoadCurve_Offset		, {	2,	PR_LOADCURVE_BUS,			PR_LOADCURVE_ID,	0,						0,	},	g_PRLoadCurveFieldArray,		},
													
	{	PR_TRIP,			"Trip"			,	"", 		"���Ϲ�����Ϣ"			, 0	, sizeof(g_PRTripFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRTrip)			, PR_MaxTrip			, PRTrip_Offset				, {	0,	0,							0,					0,						0,	},	g_PRTripFieldArray,				},
	{	PR_TRIPMISLAND,		"TripMIsland"	,	"", 		"���Ϲ����¹µ���Ϣ"	, 0	, sizeof(g_PRTripMIslandFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRTripMIsland)	, PR_MaxTripMIsland		, PRTripMIsland_Offset		, {	1,	PR_TRIPMISLAND_TRIP,		0,					0,						0,	},	g_PRTripMIslandFieldArray,		},
	{	PR_TRIPOVLDEV,		"TripOvlDev"	,	"", 		"���Ϲ�����Խ����Ϣ"	, 0	, sizeof(g_PRTripOvlDevFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRTripOvlDev)	, PR_MaxTripOvlDev		, PRTripOvlDev_Offset		, {	1,	PR_TRIPOVLDEV_TRIP,			0,					0,						0,	},	g_PRTripOvlDevFieldArray,		},
													
	{	PR_COMMRPARAM,		"CommRParam"	,	"", 		"�ɿ��Թ�������"		, 0	, sizeof(g_PRCommRParamFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRCommRParam)	, PR_MaxCommRParam		, PRCommRParam_Offset		, {	0,	0,							0,					0,						0,	},	g_PRCommRParamFieldArray,		},
	{	PR_MANUALFAULT,		"ManualFault"	,	"", 		"�˹��¼�"				, 0	, sizeof(g_PRManualFaultFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRManualFault)	, PR_MaxManualFault		, PRManualFault_Offset		, {	0,	0,							0,					0,						0,	},	g_PRManualFaultFieldArray,		},
	{	PR_COMMONFAULT,		"CommonFault"	,	"", 		"�������"				, 0	, sizeof(g_PRCommonFaultFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRCommonFault)	, PR_MaxCommonFault		, PRCommonFault_Offset		, {	0,	PR_COMMONFAULT_NAME,		0,					0,						0,	},	g_PRCommonFaultFieldArray,		},
	{	PR_R2STATECOMP,		"R2StateComp"	,	"", 		"��״̬�ɿ����豸"		, 0	, sizeof(g_PR2StateCompFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPR2StateComp)	, PR_MaxR2StateComp		, PR2StateComp_Offset		, {	0,	0,							0,					0,						0,	},	g_PR2StateCompFieldArray,		},
	{	PR_RMSTATECOMP,		"RMStateComp"	,	"", 		"��״̬�ɿ����豸"		, 0	, sizeof(g_PRMStateCompFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRMStateComp)	, PR_MaxRMStateComp		, PRMStateComp_Offset		, {	0,	0,							0,					0,						0,	},	g_PRMStateCompFieldArray,		},
	//{	PR_MCSAMPLESTATE,	"MCSampleState"	,	"", 		"���ؿ��޳���״̬"		, 0	, sizeof(g_PRMCSampleStateFieldArray)	/sizeof(tagMemDBField)	, sizeof(tagPRMCSampleState), PR_MaxMCSampleState	, PRMCSampleState_Offset	, {	0,	0,							0,					0,						0,	},	g_PRMCSampleStateFieldArray,	},

	{	PR_FSTATE,			"FState"		,	"", 		"����״̬"				, 0	, sizeof(g_PRFStateFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRFState)		, PR_MaxFState			, PRFState_Offset			, {	1,	PR_FSTATE_ID,				0,					0,						0,	},	g_PRFStateFieldArray,			},
	{	PR_FSTATEFDEV,		"FStateFDev"	,	"", 		"����״̬�¹����豸"	, 0	, sizeof(g_PRFStateFDevFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRFStateFDev)	, PR_MaxFStateFDev		, PRFStateFDev_Offset		, {	1,	PR_FSTATEFDEV_FSTATENO,		0,					0,						0,	},	g_PRFStateFDevFieldArray,		},
	{	PR_FSTATEMSTATE,	"FStateMState"	,	"", 		"����״̬�¶�״̬"		, 0	, sizeof(g_PRMStateFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRFStateMState)	, PR_MaxFStateMState	, PRFStateMState_Offset		, {	0,	0,							0,					0,						0,	},	g_PRMStateFieldArray,			},
	{	PR_FSTATEMISLAND,	"FStateMIsland"	,	"", 		"����״̬�¹µ���ʧ"	, 0	, sizeof(g_PRFStateMIslandFieldArray)	/sizeof(tagMemDBField)	, sizeof(tagPRFStateMIsland), PR_MaxFStateMIsland	, PRFStateMIsland_Offset	, {	1,	PR_FSTATEMISLAND_FSTATENO,	0,					0,						0,	},	g_PRFStateMIslandFieldArray,	},
	{	PR_FSTATEOVLDEV,	"FStateOvlDev"	,	"", 		"����״̬��Խ���豸"	, 0	, sizeof(g_PRFStateOvlDevFieldArray)	/sizeof(tagMemDBField)	, sizeof(tagPRFStateOvlDev)	, PR_MaxFStateOvlDev	, PRFStateOvlDev_Offset		, {	1,	PR_FSTATEOVLDEV_FSTATENO,	0,					0,						0,	},	g_PRFStateOvlDevFieldArray,		},
	{	PR_FSTATEOVLAD,		"FStateOvlAd"	,	"", 		"����״̬��Խ�޵���"	, 0	, sizeof(g_PRFStateOvlAdFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRFStateOvlAd)	, PR_MaxFStateOvlAd		, PRFStateOvlAd_Offset		, {	1,	PR_FSTATEOVLAD_FSTATENO,	0,					0,						0,	},	g_PRFStateOvlAdFieldArray,		},

	{	PR_ADSNODE,			"AdsNode"		,	"", 		"����ڵ��"			, 0	, sizeof(g_PRAdsNodeFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRAdsNode)		, PR_MaxAdsNode			, PRAdsNode_Offset			, {	1,	PR_ADSNODE_ID,				0,					0,						0,	},	g_PRAdsNodeFieldArray,			},
	{	PR_CALCJOBTYPE,		"CalcJobType"	,	"", 		"�������ͱ�"			, 0	, sizeof(g_PRCalcJobTypeFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRCalcJobType)	, PR_MaxCalcJobType		, PRCalcJobType_Offset		, {	1,	PR_CALCJOBTYPE_ID,			0,					0,						0,	},	g_PRCalcJobTypeFieldArray,		},
	{	PR_CALCJOB,			"CalcJob"		,	"", 		"���㹤����"			, 0	, sizeof(g_PRCalcJobFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRCalcJob)		, PR_MaxCalcJob			, PRCalcJob_Offset			, {	1,	PR_CALCJOB_ID,				0,					0,						0,	},	g_PRCalcJobFieldArray,			},
	{	PR_CALCTASK,		"CalcTask"		,	"", 		"���������"			, 0	, sizeof(g_PRCalcTaskFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRCalcTask)		, PR_MaxCalcTask		, PRCalcTask_Offset			, {	1,	PR_CALCTASK_ID,				0,					0,						0,	},	g_PRCalcTaskFieldArray,			},
	{	PR_CALCFILE,		"CalcFile"		,	"", 		"�����ļ���"			, 0	, sizeof(g_PRCalcFileFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRCalcFile)		, PR_MaxCalcFile		, PRCalcFile_Offset			, {	1,	PR_CALCFILE_JOBID,			0,					0,						0,	},	g_PRCalcFileFieldArray,			},

	{	PR_FSECURITY,		"FSecurity"		,	"", 		"��ȫ����"				, 0	, sizeof(g_PRFSecurityFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRFSecurity)	, PR_MaxFSecurity		, PRFSecurity_Offset		, {	0,	0,							0,					0,						0,	},	g_PRFSecurityFieldArray,		},
	{	PR_COPGEN,			"CopGen"		,	"", 		"ͣ�˷���"				, 0	, sizeof(g_PRCopGenFieldArray)			/sizeof(tagMemDBField)	, sizeof(tagPRCopGen)		, PR_MaxCopGen			, PRCopGen_Offset			, {	0,	0,							0,					0,						0,	},	g_PRCopGenFieldArray,			},
	{	PR_COPTABLE,		"CopTable"		,	"", 		"ͣ�˸���"				, 0	, sizeof(g_PRCopTableFieldArray)		/sizeof(tagMemDBField)	, sizeof(tagPRCopTable)		, PR_MaxCopTable		, PRCopTable_Offset			, {	0,	0,							0,					0,						0,	},	g_PRCopTableFieldArray,			},
};

static	tagMDBTableFieldContainer	g_PRTableFieldContainerArray[]=
{
	{	MDBFieldCrit,	PR_FSTATEMISLAND,		PR_FSTATEMISLAND_FSTATENO,			-1,								-1,							-1,		PR_FSTATE,			},
};

#if !defined(__GNUG__) && !defined(__GNUC__)
#	pragma pack()
#	if (defined(_AIX) || defined(AIX))
#		pragma align(fPower)
#	else
#		if (!defined(sun) && !defined(__sun) && !defined(__sun__))
#			pragma pack(pop)
#		endif
#	endif
#endif

#pragma once
#include <math.h>
#include <float.h>
#include "../../MemDB/PGMemDB/PGMemDB.h"
using namespace PGMemDB;

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

//////////////////////////////////////////////////////////////////////////
//	�����㷨�õ����ݽṹ����
typedef	struct	_PGEdge_
{
	unsigned char	nDevType;
	int		nDevIndex;

	float	fRerr;		//	�豸������
	float	fTrep;		//	�豸�޸�ʱ��
	float	fRchk;		//	�豸������
	float	fTchk;		//	�豸����ʱ��

	float	fRSwitch;	//	�豸�л��ɹ���
	float	fTSwitch;	//	�豸�л�ʱ��

	float	fLength;	//	·������
	//std::string	strDevSub;
	//std::string	strDevVolt;
	//std::string	strDevName;
}	tagPGEdge;

typedef	struct	_PGRouter_
{
	unsigned char	bMarked;	//	���λ��������ʾ
	unsigned char	bRouter;	//	·�����
	int		nDeep;				//	·����ʼ������
	int		nFrNode;			//	���ɱ�·����ʼ�ڵ�
	int		nToNode;			//	���ɱ�·����ֹ�ڵ�
	float	fLength;			//	·������
	double	fR;					//	��Чͣ����
	double	fT;					//	��Чͣ��ʱ��
	std::vector<tagPGEdge>	edgeArray;	//	����FrNode��ToNode�ı߼���
}	tagPGRouter;

typedef	struct	_PGSource_
{
	int		nEndNode;					//	ĩ�˽ڵ�
	float	fEndLength;					//	����㡫ĩ�˵��ܳ���
	unsigned char	nEndingType;		//	ĩ���豸����
	int		nDevIndex;					//	ĩ���豸����
}	tagPGSource;					//	·���յ�

//////////////////////////////////////////////////////////////////////////
//	���ɵ�Դ׷�ݺ͵�Դ����·�������ݽṹ����
typedef	struct _PGLoadTraceSource		//	����׷�ݵ�Դ
{
	std::string	strLoad;
	float		fLoadMva;
	float		fRadius;
	std::vector<tagPGRouter>	sRArray;
	std::vector<tagPGSource>	sSArray;
}	tagPGLoadTraceSource;

typedef	struct _PGSourceRouteLoad		//	��Դ~���ɹ���·��
{
	std::string					strSource;		//	��Դ����
	tagPGSource					infSource;		//	��Դ��Ϣ
	float						fRadius;
	std::vector<tagPGRouter>	sRArray;		//	��Դ�����ɵ�·�������������

	double	fR;	//	��Դ·��������
	double	fT;	//	��Դ·��ÿ��ͣ��ƽ������ʱ��
	double	fU;	//	��Դ·���޸�ʱ��
}	tagPGSourceRouteLoad;

typedef	struct _PGLoadSourceRouter		//	���ɵĵ�Դ����·������
{
	std::string	strLoad;
	float		fLoadMva;
	float		fRadius;
	std::vector<tagPGSourceRouteLoad>	sSArray;
}	tagPGLoadSourceRouter;

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

//	PGTraceSource
//		1��������������㼯�ϣ�
//		2���γɵ����֮��߼���
//		3����������Դ���߽߱�㷵��
class CPGTraceAlgorithm
{
public:
	CPGTraceAlgorithm(void);
	~CPGTraceAlgorithm(void);

//////////////////////////////////////////////////////////////////////////
//	�����㷨
public:
	void PGTraceSource	(tagPGBlock* pPGBlock, const int bCheckOpen, const int nStartNode, std::vector<int>& nBoundNodeArray, std::vector<tagPGRouter>& routerArray, std::vector<tagPGSource>& endingArray);
	void PGTraceRange	(tagPGBlock* pPGBlock, const int bCheckOpen, const int nStartNode, std::vector<int>& nBoundNodeArray, std::vector<tagPGRouter>& routerArray, std::vector<tagPGSource>& endingArray);

private:
	int	IsNodeSource(tagPGBlock* pPGBlock, const int nJudgeNode);
	void RouterRecursive(tagPGBlock* pPGBlock, const int bCheckOpen, tagPGSource& routerEnd, std::vector<tagPGRouter>& routerArray);
	void TraceJoint(tagPGBlock* pPGBlock, const int bCheckOpen, const int nIniNode, int nDeep, std::vector<tagPGRouter>& routerArray, std::vector<tagPGSource>& endingArray);
	//	�����������
private:
	//	�γ�nNodeI��nNodeJ֮������б��豸���ϣ����ر߳���(ȡ��Сֵ)
	float FormRouterEdge(tagPGBlock* pPGBlock, const unsigned char bCheckOpen, const int nNodeI, const int nNodeJ, std::vector<tagPGEdge>& edgeArray);

	void InitializePGRouterSet(tagPGRouter& rBuf)
	{
		rBuf.bMarked = 0;	//	������ݱ��
		rBuf.bRouter = 0;	//	������ݱ��
		rBuf.nDeep = 0;		//	·������ʼ������
		rBuf.nFrNode = -1;	//	���ɱ�·����ʼ�ڵ�
		rBuf.nToNode = -1;	//	���ɱ�·����ֹ�ڵ�
		rBuf.fLength = 0;
		rBuf.fR = 0;		//	��Чͣ����
		rBuf.fT = 0;		//	��Чͣ��ʱ��

		rBuf.edgeArray.clear();
	}

private:
	std::vector<unsigned char>	m_bNodeProcArray;

//////////////////////////////////////////////////////////////////////////
//	�����㷨
public:
	void FormLoadSourceTracer(IN tagPGBlock* pPGBlock, IN const unsigned char bCheckOpen, OUT std::vector<tagPGLoadTraceSource>& sLoadTraceSourceArray);
	void FormSourceLoadRouter(IN tagPGBlock* pPGBlock, IN std::vector<tagPGLoadTraceSource>& sLoadTraceSourceArray, OUT std::vector<tagPGLoadSourceRouter>& sLoadSourceRouterArray);
	void SourceLoadRouterReliable(IN OUT std::vector<tagPGLoadSourceRouter>& sLoadSourceRouterArray);

private:
	int FindSourceRouter(const int nSourceNode, std::vector<tagPGRouter>& sRArray);
};

#pragma once
#pragma warning( disable: 4251 )

#include <float.h>
#include "../../MemDB/PGMemDB/PGMemDB.h"
using	namespace	PGMemDB;

#include "../../MemDB/PRMemDB/PRMemDB.h"
using	namespace	PRMemDB;

#define	UPFC_MODE_NO		0
#define	UPFC_MODE_UPFC		1

#include "dcnetworkexp.h"

namespace DCNetwork
{
	const int	MaxBPerBus=40;
	const int	g_bWriteDebug=0;

	const	double	g_fDCNMvaBase_500=1082;
	const	double	g_fDCNMvaBase_330=686;
	const	double	g_fDCNMvaBase_220=305;
	const	double	g_fDCNMvaBase_110=114;

	const	double	g_fTripDeltPThreshold=1;

	class  DCNETWORK_EXP	CDCNetwork
	{
	public:
		CDCNetwork(void);
		~CDCNetwork(void);

	public:
		//	ֱ������
		void	PGDCFlow(tagPGBlock* pPGBlock);																	//	����������ݿ��

		void	PRDCFlow(tagPRBlock* pPRBlock, const unsigned char nUPFCControlMode, const int nRadial=0);		//	����ɿ������ݿ��
		void	PRDCFlowSolve(tagPRBlock* pPRBlock);
		void	PRDCMatZ(tagPRBlock* pPRBlock, const int nRadial, std::vector<float>& fMatZArray);				//	�迹����

		//	���������ϼ���
		void	PRDCTrip(tagPRBlock* pPRBlock, const int nDevType, const char* lpszDevName);
		void	PRDCTrip(tagPRBlock* pPRBlock, const int nDevType, const int nDevIndex);

	public:
		void	PRDCNetwork_SaveDataFile(tagPRBlock* pPRBlock, const char* lpszFileName);
		int		PRDCNetwork_ReadDataFile(tagPRBlock* pPRBlock, const char* lpszFileName);

	private:	//	���ɾ������ɡ�����ǰ���ش���
		int		PGDCNetwork_ReadData(tagPGBlock* pPGBlock, const int nIsland);
		void	PGDCIslandFlowOut(tagPGBlock* pPGBlock, const int nIsland=1, const unsigned char bTripFlow=0);

	private:	//	ֱ�������ӳ���
		int		PRDCNetwork_ReadData(tagPRBlock* pPRBlock, const unsigned char nUPFCControlMode, const int nIsland);
		int		PRDCNetwork_ReadRadial(tagPRBlock* pPRBlock, const int nRadial);

		void	PRIslandMatZ(tagPRBlock* pPRBlock, const int nIsland=1);
		void	PRRadialMatZ(tagPRBlock* pPRBlock, const int nRadial);
		void	PRCheckMatZ(tagPRBlock* pPRBlock, const int nIsland);

		void	PRDCIslandFlowOut(tagPRBlock* pPRBlock, const unsigned char nUPFCControlMode, const int nIsland=1, const unsigned char bTripFlow=0);
		void	PRDCRadialFlowOut(tagPRBlock* pPRBlock, const int nRadial);


	private:	//	����
		void	PRFlowIslandDFactor(tagPRBlock* pPRBlock, const int nIsland=1);	//	������֧·�ڵ㹦�ʷֲ�����, Ŀǰ��ʱͣ��
		void	PRFlowDFactorFBIterate(const int nBus);

	private:
		void	DCNetwork_Rename();
		void	DCNetwork_NewZ();
		void	DCNetwork_Admitance();
		void	DCNetwork_CholeskyDecompose();
		void	DCNetwork_Injection();
		void	DCNetwork_BackwardSubstitution();

	//	�迹����
		void	DCNetwork_InvEliminate();
		void	DCNetwork_InvImpedance();

	private:
		void	EmptyDataBuffer();
		void	AllocDataBuffer(const int nMaxBus);

		//	ֱ���������������ݣ����ö������ݿռ���ƣ������������̸���
	private:
		int		m_nSlackBus;
		int		m_nNumBus;
		int		m_nMaxBusNumber;

		int		m_nNumBra;
		std::vector<double>	m_fZArray;		//double	m_fZArray[4*DCN_MaxBran];
		int		m_nNumGen;
		std::vector<double>	m_fGenArray;	//double	m_fGenArray[3*DCN_MaxUnit];
		int		m_nNumLoad;
		std::vector<double>	m_fLoadArray;	//double	m_fLoadArray[3*DCN_MaxLoad];

		//	�м�����
		std::vector<int>	m_nBusOld;		//int		m_nBusOld[DCN_MaxBus];				//	�ڵ�Ŵ�1��ʼ��� 1<=n  n<=Max
		std::vector<int>	m_nBusNew;		//int		m_nBusNew[DCN_MaxBus];				//	�ڵ����Ż�

		std::vector<double>	m_fMatrixD;		//double	m_fMatrixD[DCN_MaxBus];				//	�Խ�Ԫ
		std::vector<double>	m_fMatrixU;		//double	m_fMatrixU[20*DCN_MaxBus];			//	�����Ƿ���Ԫ
		std::vector<int>	m_nMatrixIU;	//int		m_nMatrixIU[20*DCN_MaxBus];			//	�����Ƿ���Ԫ�к�
		int		m_nNumUTri;							//	������Ԫ�ظ���

		std::vector<double>	m_fBII;			//double	m_fBII[DCN_MaxBus];					//	������Խ�Ԫ���Ե���
		std::vector<double>	m_fWkRow;		//double	m_fWkRow[DCN_MaxBus];				//	������

		std::vector<int>	m_nNumFD;		//int		m_nNumFD[DCN_MaxBus];				//	ÿ�з���Ԫ����
		std::vector<int>	m_nNumDZ;		//int		m_nNumDZ[DCN_MaxBus];				//	�����洢ÿ���ڵ������֧·��

		std::vector<double>	m_fBusD;		//double	m_fBusD[DCN_MaxBus];				//	���Ǽ�����

		std::vector<int>	m_nNOB;			//int		m_nNOB[DCN_MaxBus];					//	�м��������ֹ���䲻���ڴ�
		std::vector<int>	m_nNOCN;		//int		m_nNOCN[MaxBPerBus*DCN_MaxBus];	//	��0��ʼ���
		std::vector<int>	m_nNDS;			//int		m_nNDS[DCN_MaxBus];					//	ÿ���ڵ������֧·����ÿ��֧·ȡС�ڵ�ŵ�

		//	�迹�������迹����ռ�Ҫ��̫�󣬲��ñ䳤���
	private:
		std::vector<float>	m_fMatZ;		//	�����
	};
	extern	const	char*	g_lpszDCNetworkLogFile;
}

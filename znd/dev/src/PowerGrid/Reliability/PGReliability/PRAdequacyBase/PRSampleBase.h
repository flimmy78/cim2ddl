#pragma once
#if _MSC_VER >= 1400 // VC++ 8.0
#pragma warning( disable : 4996 )   // disable warning about strdup being deprecated.
#endif

#include "../../../../MemDB/PRMemDB/PRMemDB.h"
using namespace PRMemDB;

namespace	PRAdequacyBase
{
	const	double	S_ERR_MAX = 8760.00;  //����������ֵ�������������ֵ�Ļ���Ԫ��һֱ���ڹ���״̬�����ϸ�����Ϊ1.0
	const	double	SET_I_SPEC = 0.8;
	const	double	SET_I_TRIP = 1.2;
	const	double	SET_RERR_MAX = 200.0;

	const	int		SystemSate_Max=1000000;

	//��������
	const	int		g_nMaxElement=20;
	const	int		g_nMaxSequence=5;

	//////////////////////////////////////////////////////////////////////////
	//	�ɿ����豸
	enum	_PReliableEnum_CompStatus_
	{
		CompStatus_Fail=0,	//	����״̬
		CompStatus_Good,	//	���״̬
		CompStatus_MState,	//	��״̬
	};

	//////////////////////////////////////////////////////////////////////////
	//	����״̬
	typedef	struct	_PRState_
	{
		int			nTimeSeries;
		std::vector<int>	nFCompArray;				//	�����豸����

		unsigned char	bMState;						//	��ʾ�Ƕ�״̬�豸״̬�仯����2״̬�豸״̬�仯
		std::vector<double>	fMSCompPoutArray;			//	��״̬�豸״̬

		int			nTotalNumber;						//	���ִ���
		double		fProbability;						//	���ϸ���
		double		fDuration;							//	����ʱ��
	}	tagPRSampleState;

	class CPRSampleBase
	{
	public:
		CPRSampleBase(void);
		~CPRSampleBase(void);

	public:
		void SetPRSampleStateMState(tagPRBlock* pPRBlock, tagPRSampleState& stBuffer);

		void InitPRSampleState(tagPRBlock* pPRBlock, tagPRSampleState& stBuffer);
		void SortPRSampleState(std::vector<tagPRSampleState>& stArray, int nDn0, int nUp0);
		void CopyPRSampleState(tagPRSampleState* pDestState, tagPRSampleState* pFromState);
		int ComparePRSampleState(tagPRSampleState& stA, tagPRSampleState& stB);

		void SortPRSampleStateCompSize(std::vector<tagPRSampleState>& stArray, int nDn0, int nUp0);
		void SortInteger(std::vector<int>& nArray, int nDn0, int nUp0);

	public:
		void PrevSample(tagPRBlock* pPRBlock);
		void PostSample(tagPRBlock* pPRBlock);
	};
}

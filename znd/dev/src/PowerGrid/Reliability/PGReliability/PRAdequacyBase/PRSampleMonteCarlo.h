#pragma once

#include "PRSampleBase.h"

namespace	PRAdequacyBase
{
	//////////////////////////////////////////////////////////////////////////
	//	���ؿ������
	typedef	struct	_MCSDevState_
	{
		int				nCompIndex;	//	��CompArray�е����
		double			fRerr;		//	�����ʣ���״̬��Ч
		double			fRrep;		//	�޸��ʣ���״̬��Ч
		unsigned char	nStatus;	//	��¼��״̬ģ�͵���һ״̬, ��״̬��ΪGOOD��FAIL����״̬ģ�Ͳ�ʹ�ø�����

		unsigned char	bMState;
		double			fMSPout;	//	��״̬ģ�͵ĵ�ǰ��������һ״̬��Ҫ�������㣬�������״̬ģ�����������

		double			fEvtTime;	//	��ǰ״̬����һ״̬����ʱ��
	}	tagMCSDevState;

	class CPRSampleMonteCarlo : public CPRSampleBase
	{
	public:
		CPRSampleMonteCarlo(void);
		~CPRSampleMonteCarlo(void);

		//////////////////////////////////////////////////////////////////////////
		//	���ؿ���״̬����
	public:
		int Sample(tagPRBlock* pPRBlock, const double fSimTime, const int nMaxGenFault, const int nMaxBranFault, const double fProbThreshold=0);

	private:
		int MonteCarlo2StateAddFDev(tagPRBlock* pPRBlock, tagPRSampleState* pState, const int nMaxGenFault, const int nMaxBranFault, const int nCompIndex);
		void MonteCarlo2StateDelFDev(tagPRBlock* pPRBlock, tagPRSampleState* pState, const int nCompIndex);
		void MCSDev2StateDuration_Calc(tagMCSDevState* pDevState);

		void MonteCarloMState(tagPRBlock* pPRBlock, tagPRSampleState* pState, const int nMStateDev);
		void MCSDevMStateDuration_Calc(tagPRBlock* pPRBlock, tagMCSDevState* pDevState);
		void MCSDevStateDuration_Sort(std::vector<tagMCSDevState>& evtArray, int nDn0, int nUp0);

	public:
		std::vector<tagMCSDevState>		m_MCSDevStateArray;
		std::vector<tagPRSampleState>	m_MCSSysStateArray;

	private:
		int ComparePRSampleMStatePout(tagPRSampleState& stA, tagPRSampleState& stB);
		void SortPRSampleMStatePout(std::vector<tagPRSampleState>& stArray, int nDn0, int nUp0);
		int FindPRSampleMStatePout(std::vector<tagPRSampleState>& stArray, int nLeft, int nRight, tagPRSampleState& stBuf);
	};
}

#pragma once

#include "PRSampleBase.h"

namespace	PRAdequacyBase
{
	//////////////////////////////////////////////////////////////////////////
	//	���������м���Ϣ
	typedef	struct	_FastSortTransient_
	{
		int				nCompIndex;	//	��CompArray�е����
		unsigned char	nStatus;	//	�豸״̬
		double			fProb;		//	����
	}	tagFastSortTransient;

	class CPRSampleFastSort : public CPRSampleBase
	{
	public:
		CPRSampleFastSort(void);
		~CPRSampleFastSort(void);

		//////////////////////////////////////////////////////////////////////////
		//	��������״̬����
	public:
		int	Sample(tagPRBlock* pPRBlock, const double fProbPrecision, const int nMaxState, const int nMaxGenFault, const int nMaxBranFault, const double fStateProbThreshold=0);

	private:
		void FastSortExperiment(tagPRBlock* pPRBlock);
		void SortFastSortTransient(std::vector<tagFastSortTransient>& tArray, int nDn0, int nUp0);

		void FastSortCalculateStateProbability(tagPRSampleState* pState);
		void FastSortCalculateStateFrequency(tagPRBlock* pPRBlock, tagPRSampleState* pState);
		void ExpandNeighbourhood(tagPRSampleState* pState);

		int FastSortAddState(tagPRBlock* pPRBlock, tagPRSampleState* pState, const int nMaxGenFault, const int nMaxBranFault, const int nMode);
		int FastSortAddNeighbourState(tagPRSampleState& stAdd);

		void ModCompModel(tagPRBlock* pPRBlock);	//	����ʵʱ������, ָ��ģ��

	private:
		double								m_fActualProbility;
		std::vector<tagFastSortTransient>	m_FSTransientArray;
		std::vector<tagPRSampleState>				m_FSStateArray;
		std::vector<tagPRSampleState>				m_FSNeighbourStateArray;

	private:
		void SortPRStateProb(std::vector<tagPRSampleState>& tArray, int nDn0, int nUp0);
	};
}

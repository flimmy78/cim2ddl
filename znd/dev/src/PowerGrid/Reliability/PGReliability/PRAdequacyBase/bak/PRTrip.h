#pragma once

#include "../../../MemDB/PRMemDB/PRMemDB.h"

#include "PRDataDefine.h"
#include "../../DCNetwork/DCNetwork.h"
using	namespace	DCNetwork;

typedef	struct _BranDFSet
{
	std::vector<tagBusDFactor>		BusDFArray;	//	֧·����ڵ�ֲ�����
	std::vector<tagBranDFactor>		LDFArray;	//	֧·��·���Ϸֲ�����
	std::vector<tagBranDFactor>		TDFArray;	//	֧·���俪�Ϸֲ�����
}	tagBranDFSet;

class CPRTrip
{
public:
	CPRTrip(void);
	~CPRTrip(void);

//////////////////////////////////////////////////////////////////////////
//	�ֲ����ӷ�����
public:
	void	PRTrip(tagPRBlock* pPRBlock, const double fDC2ACRatio, const int nTripMode, const double fDFThreshold, const double fDPThreshold);
	void	FormDFactor(tagPRBlock* pPRBlock, const double fDFThreshold, const double fDPThreshold);

public:
	std::vector<tagBranDFSet>	m_LineDFArray;
	std::vector<tagBranDFSet>	m_TranDFArray;

private:
	CDCNetwork					m_DCNetwork;
	std::vector<float>			m_fMatZArray;

private:
	void	OneBusDFactor(tagPRBlock* pPRBlock, const double fDFThreshold, const double fDPThreshold);		//	�γ�ĸ��ע�������֧·��������
	void	OneBranTrip(tagPRBlock* pPRBlock, const double fDC2ACRatio, const double fDFThreshold, const double fDPThreshold);
	void	TwoBranTrip(tagPRBlock* pPRBlock, const double fDC2ACRatio);

private:
	int		OneTripIsland(tagPRBlock* pPRBlock, const int nPrevIslandNum, const unsigned char nBTyp, const int nBIdx);
	int		TwoTripIsland(tagPRBlock* pPRBlock, const int nPrevIslandNum, const unsigned char nB1Typ, const int nB1Idx, const unsigned char nB2Typ, const int nB2Idx);
	void	Trip1Factor(tagPRBlock* pPRBlock, const unsigned char nBranTyp, const int nBran, const unsigned char nTripTyp, const int nTrip, double& fFactor);
	void	Trip2Factor(tagPRBlock* pPRBlock, const unsigned char nBranTyp, const int nBran, const unsigned char nTrip1Typ, const int nTrip1, const unsigned char nTrip2Typ, const int nTrip2, double& fFactor1, double& fFactor2);
	void	Record1Trip(tagPRBlock* pPRBlock, const short nOvlmType, const int nOvlmBran, const short nTripType, const int nTripBran, const float fFlow, const float fRated);
	void	Record2Trip(tagPRBlock* pPRBlock, const short nOvlmType, const int nOvlmBran, const short nTrip1Type, const int nTrip1Bran, const short nTrip2Type, const int nTrip2Bran, const float fFlow, const float fRated);

private:
	void	SortLBusDFactor(std::vector<tagBusDFactor>& dfArray, int nDn0, int nUp0);
	void	SortBranDFactor(std::vector<tagBranDFactor>& dfArray, int nDn0, int nUp0);

private:
	void	SortPRFMode(tagPRBlock* pBlock, int nDn0, int nUp0);
	int		ComparePRTrip(tagPRTrip& mdA, tagPRTrip& mdB);
};

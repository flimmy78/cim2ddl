#pragma once

#include "../../../../MemDB/PRMemDB/PRMemDB.h"
using namespace PRMemDB;

namespace	PRAdequacyBase
{
	const	int		g_nConstMaxAdjustTimes = 8;

	typedef	struct _PRBusDisFactor
	{
		int		nBus;
		double	fDFactor;
		double	fGDeltP;	//	��ڵ㷢���TRIP�����µ�֧·���ʱ仯��
		double	fLDeltP;	//	��ڵ㷢���TRIP�����µ�֧·���ʱ仯��
	}	tagBusDFactor;

	typedef	struct _PRBranDisFactor
	{
		int		nBran;
		double	fFactor;
		double	fDeltP;	//	��֧·TRIP�����µ�֧·���ʱ仯��
	}	tagBranDFactor;

	typedef	struct	_OLmtAdjust_
	{
		int				nBus;			//	����ĸ��
		double			fAdjFactor;		//	�ֲ����ӣ���֧·���������Ϊ���������Ϊ����
		double			fAvailAscP;		//	���Ͽɵ�����
		double			fAvailDescP;	//	���¿ɵ�����
		double			fOprAdjP;		//	������
		short			nAdDirect;		//	��������=0δ֪��=1��ʾ��������Ϊ����=-1��ʾ�����������
		int				nDevice;		//	�豸����
	}	tagOLmtAdjust;

	class CPROverLimitAd
	{
	public:
		CPROverLimitAd(void);
		~CPROverLimitAd(void);

	public:
		void	InitAdjustParameter(tagPRBlock* pPRBlock);

		int		AdjustLineOverLimit(tagPRBlock* pPRBlock,
			const unsigned char bGenEOvl,
			const unsigned char bUPFCEOvl,
			const unsigned char bAuxLoadAdjust,
			const unsigned char bUPFCAdjustRC,
			const unsigned char bCheckGenAdjDirection,
			const int nFState,
			const int nLine,
			const double fOlmP,
			double& fLoadCut,
			std::vector<float>& fMatZArray);
		int		AdjustTranOverLimit(tagPRBlock* pPRBlock,
			const unsigned char bGenEOvl,
			const unsigned char bUPFCEOvl,
			const unsigned char bAuxLoadAdjust,
			const unsigned char bUPFCAdjustRC,
			const unsigned char bCheckGenAdjDirection,
			const int nFState,
			const int nTran,
			const double fOlmP,
			double& fLoadCut,
			std::vector<float>& fMatZArray);

		int		AdjustRadialLineOverLimit(tagPRBlock* pPRBlock, const int nRadial, const int nFState, const int nLine, const double fOlmP, double& fLoadCut, std::vector<float>& fMatZArray);
		int		AdjustRadialTranOverLimit(tagPRBlock* pPRBlock, const int nRadial, const int nFState, const int nTran, const double fOlmP, double& fLoadCut, std::vector<float>& fMatZArray);

	private:
		void	SorOvLmtAdFactorL2H(std::vector<tagOLmtAdjust>& dfArray, int nDn0, int nUp0);
		void	SorOvLmtAdFactorH2L(std::vector<tagOLmtAdjust>& dfArray, int nDn0, int nUp0);

	private:
		void	FormBranDFactor(tagPRBlock* pPRBlock, const int nBusI, const int nBusJ, const double fX, const int nIsland, std::vector<float> fMatZArray);
		void	GetAdjustArray(tagPRBlock* pPRBlock, const unsigned char bAuxLoadAdjust, const unsigned char bUPFCAdjustRC, std::vector<tagOLmtAdjust>& sGenAdArray, std::vector<tagOLmtAdjust>& sLoadAdArray, std::vector<tagOLmtAdjust>& sUPFCAdArray);

		int		AdjustOverLimit(tagPRBlock* pPRBlock, const unsigned char bGenEOvl, const unsigned char bUPFCEOvl, const unsigned char bCheckGenAdjDirection,
			const int nFState, const unsigned char nOvlDevTyp, const int nOvlDevIdx, const double fOlmP, double& fLoadCut,
			std::vector<tagOLmtAdjust>& sGenAdArray, std::vector<tagOLmtAdjust>& sLoadAdArray, std::vector<tagOLmtAdjust>& sUPFCAdArray);

		int		AdjustOverLimit_DecreaseFlow(tagPRBlock* pPRBlock, const unsigned char bGenEOvl, const unsigned char bUPFCEOvl, const unsigned char bCheckGenAdjDirection,
			const int nFState, const unsigned char nOvlDevTyp, const int nOvlDevIdx, const double fOlmP,
			std::vector<tagOLmtAdjust>& sGenAdArray, std::vector<tagOLmtAdjust>& sLoadAdArray, std::vector<tagOLmtAdjust>& sUPFCAdArray);

		int		AdjustOverLimit_IncreaseFlow(tagPRBlock* pPRBlock, const unsigned char bGenEOvl, const unsigned char bUPFCEOvl, const unsigned char bCheckGenAdjDirection,
			const int nFState, const unsigned char nOvlDevTyp, const int nOvlDevIdx, const double fOlmP,
			std::vector<tagOLmtAdjust>& sGenAdArray, std::vector<tagOLmtAdjust>& sLoadAdArray, std::vector<tagOLmtAdjust>& sUPFCAdArray);

		void	FormRadialBranDFactor(tagPRBlock* pPRBlock, const int nRadial, const int nBusI, const int nBusJ, const double fX, const int nIsland, std::vector<float> fMatZArray);
		void	GetRadialAdjustArray(tagPRBlock* pPRBlock, const int nRadial, const double fOlmP, std::vector<tagOLmtAdjust>& GenAdArray, std::vector<tagOLmtAdjust>& LoadAdArray, std::vector<tagOLmtAdjust>& sUPFCAdArray);

	private:
		std::vector<tagBusDFactor>	m_BranDFactorArray;
		std::vector<double>			m_fBusGenAdjustArray;
	};
}

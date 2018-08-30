#include <float.h>
#include "PROverLimitAd.h"

extern	const	char*	g_lpszLogFile;
extern	void	Log(const char* lpszLogFile, const char* pformat, ...);
namespace	PRAdequacyBase
{
	extern	CPRMemDBInterface	g_PRMemDBInterface;

	void CPROverLimitAd::FormRadialBranDFactor(tagPRBlock* pPRBlock, const int nRadial, const int nBusI, const int nBusJ, const double fX, const int nIsland, std::vector<float> fMatZArray)
	{
		int		nBus;
		tagBusDFactor	dBuf;

		m_BranDFactorArray.clear();
		for (nBus=1; nBus<pPRBlock->m_nRecordNum[PR_ACBUS]; nBus++)
		{
			if (pPRBlock->m_ACBusArray[nBus].nRadial != nRadial)
				continue;

			//dBuf.fDFactor=-(fMatZArray[pPRBlock->m_nRecordNum[PR_ACBUS]*nBusI+nBus]-fMatZArray[pPRBlock->m_nRecordNum[PR_ACBUS]*nBusJ+nBus])/fX;
			dBuf.fDFactor=(fMatZArray[pPRBlock->m_nRecordNum[PR_ACBUS]*nBusI+nBus]-fMatZArray[pPRBlock->m_nRecordNum[PR_ACBUS]*nBusJ+nBus])/fX;
			dBuf.nBus=nBus;
			m_BranDFactorArray.push_back(dBuf);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//	֧·����Ϊ��Խ�ޣ�֧·������������Ϊ����ͬʱԽ�ޣ������������ǰ���������ٳ�����
	//	֧·����Ϊ��Խ�ޣ�֧·������������Ϊ����ͬʱԽ�ޣ������������ǰ����������ӳ�����
	//	�������Ǹ��ɷ���
	//	���������������PΪ��
	//	���������������PΪ��
	//	���ɽ����ʣ���PΪ��
	void CPROverLimitAd::GetRadialAdjustArray(tagPRBlock* pPRBlock, const int nRadial, const double fOlmP, std::vector<tagOLmtAdjust>& GenAdArray, std::vector<tagOLmtAdjust>& LoadAdArray, std::vector<tagOLmtAdjust>& sUPFCAdArray)
	{
		register int	i;
		int		nBus;
		tagOLmtAdjust	adjBuffer;

		GenAdArray.clear();
		LoadAdArray.clear();
		sUPFCAdArray.clear();

		memset(&adjBuffer, 0, sizeof(tagOLmtAdjust));
		adjBuffer.nBus=pPRBlock->m_RadialArray[nRadial].nRingBoundBus;
		adjBuffer.fAdjFactor=0;
		adjBuffer.fAvailAscP=9999;	//	������������ռ�
		adjBuffer.fAvailDescP=9999;		//	������������ռ�
		GenAdArray.push_back(adjBuffer);

		memset(&adjBuffer, 0, sizeof(tagOLmtAdjust));
		for (i=0; i<(int)m_BranDFactorArray.size(); i++)					//	���ɿɵ�������
		{
			nBus=m_BranDFactorArray[i].nBus;
			if (fabs(pPRBlock->m_ACBusArray[nBus].fLoadP) < FLT_MIN)
				continue;

			adjBuffer.nBus=nBus;
			adjBuffer.fAdjFactor=m_BranDFactorArray[i].fDFactor;
			adjBuffer.fAvailDescP=pPRBlock->m_ACBusArray[nBus].fLoadP+pPRBlock->m_ACBusArray[nBus].fAdjLoadP+pPRBlock->m_ACBusArray[nBus].fRadP+pPRBlock->m_ACBusArray[nBus].fAdjRadP;
			if (adjBuffer.fAvailDescP > 1)
				LoadAdArray.push_back(adjBuffer);
		}
	}

	int CPROverLimitAd::AdjustRadialLineOverLimit(tagPRBlock* pPRBlock, const int nRadial, const int nFState, const int nLine, const double fOlmP, double& fLoadCut, std::vector<float>& fMatZArray)
	{
		std::vector<tagOLmtAdjust>	GenAdArray, LoadAdArray, sUPFCAdArray;

		fLoadCut=0;

#ifdef	_DEBUG
		Log(g_lpszLogFile, "    Radial������· %s Խ��ֵ = %f\n", pPRBlock->m_ACLineArray[nLine].szName, fOlmP);
#endif
		FormRadialBranDFactor(pPRBlock, nRadial, pPRBlock->m_ACLineArray[nLine].nIBus, pPRBlock->m_ACLineArray[nLine].nZBus, pPRBlock->m_ACLineArray[nLine].fX, pPRBlock->m_ACLineArray[nLine].nIsland, fMatZArray);
#ifdef	_DEBUG
		//Log(g_lpszLogFile, "        �ֲ�����\n");
		//for (int nBus=0; nBus<(int)m_BranDFactorArray.size(); nBus++)
		//	Log(g_lpszLogFile, "            Bus=%s Factor=%g\n", pPRBlock->m_ACBusArray[m_BranDFactorArray[nBus].nBus].szName, m_BranDFactorArray[nBus].fDFactor);
#endif
		GetRadialAdjustArray(pPRBlock, nRadial, fOlmP, GenAdArray, LoadAdArray, sUPFCAdArray);
		if (GenAdArray.empty() && LoadAdArray.empty())
			return PRFState_AdResult_Unadjustable;
		return AdjustOverLimit(pPRBlock, 1, 0, 0, nFState, PR_ACLINE, nLine, fOlmP, fLoadCut, GenAdArray, LoadAdArray, sUPFCAdArray);
	}

	int CPROverLimitAd::AdjustRadialTranOverLimit(tagPRBlock* pPRBlock, const int nRadial, const int nFState, const int nTran, const double fOlmP, double& fLoadCut, std::vector<float>& fMatZArray)
	{
		std::vector<tagOLmtAdjust>	GenAdArray, LoadAdArray, sUPFCAdArray;

		fLoadCut=0;

#ifdef	_DEBUG
		Log(g_lpszLogFile, "    Radial���ڱ�ѹ�� %s Խ��ֵ = %f\n", pPRBlock->m_WindArray[nTran].szName, fOlmP);
#endif
		FormRadialBranDFactor(pPRBlock, nRadial, pPRBlock->m_WindArray[nTran].nIBus, pPRBlock->m_WindArray[nTran].nZBus, pPRBlock->m_WindArray[nTran].fX, pPRBlock->m_WindArray[nTran].nIsland, fMatZArray);
#ifdef	_DEBUG
		//Log(g_lpszLogFile, "        �ֲ�����\n");
		//for (int nBus=0; nBus<(int)m_BranDFactorArray.size(); nBus++)
		//	Log(g_lpszLogFile, "            Bus=%s Factor=%g\n", pPRBlock->m_ACBusArray[m_BranDFactorArray[nBus].nBus].szName, m_BranDFactorArray[nBus].fDFactor);
#endif
		GetRadialAdjustArray(pPRBlock, nRadial, fOlmP, GenAdArray, LoadAdArray, sUPFCAdArray);
		if (GenAdArray.empty() && LoadAdArray.empty())
			return PRFState_AdResult_Unadjustable;
		return AdjustOverLimit(pPRBlock, 1, 0, 0, nFState, PR_WIND, nTran, fOlmP, fLoadCut, GenAdArray, LoadAdArray, sUPFCAdArray);
	}
}

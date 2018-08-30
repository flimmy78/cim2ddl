#include <Windows.h>
#include "PRCopTable.h"
#include "../../../../Common/TinyXML/tinyxmlglobal.h"

// #define	_ExactStateFreq
namespace	PRAdequacyBase
{
	extern	CPRMemDBInterface	g_PRMemDBInterface;

	CPRCopTable::CPRCopTable(void)
	{
	}

	CPRCopTable::~CPRCopTable(void)
	{
	}

	void CPRCopTable::CopInit(tagPRBlock* pPRBlock, const double fPStep)
	{
		int		nGen, nRatio;

		pPRBlock->m_System.fCopStep=fPStep;
		pPRBlock->m_nRecordNum[PR_COPGEN]=0;
		for (nGen=0; nGen<pPRBlock->m_nRecordNum[PR_GENERATOR]; nGen++)
		{
			if (pPRBlock->m_GeneratorArray[nGen].fRerr < FLT_MIN)
				continue;
			if (pPRBlock->m_GeneratorArray[nGen].fPMax < 1)
				continue;

			memset(&pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]], 0, sizeof(tagPRCopGen));
			pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=nGen;
			pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].b3State=0;
			pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
			pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=pPRBlock->m_GeneratorArray[nGen].fPMax;

			pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=pPRBlock->m_GeneratorArray[nGen].fRerr/365;		//	����ϵͳ�ɿ��ԣ���λ=��/��
			pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=pPRBlock->m_GeneratorArray[nGen].fTrep/24;		//	����ϵͳ�ɿ��ԣ���λ=��
			pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=(pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep > FLT_MIN) ? 1.0/pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep : 0;
			pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr/(pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr+pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep);
			pPRBlock->m_nRecordNum[PR_COPGEN]++;
		}

		for (nGen=0; nGen<pPRBlock->m_nRecordNum[PR_COPGEN]; nGen++)
		{
			nRatio=(int)floor(pPRBlock->m_CopGenArray[nGen].fCapacity/fPStep);
			if ((pPRBlock->m_CopGenArray[nGen].fCapacity-nRatio*fPStep) > 0.1 && fPStep < pPRBlock->m_CopGenArray[nGen].fCapacity)
			{
				pPRBlock->m_CopGenArray[nGen].b3State=1;
				pPRBlock->m_CopGenArray[nGen].fCapL=nRatio*fPStep;
				pPRBlock->m_CopGenArray[nGen].fCapH=(nRatio+1)*fPStep;
				pPRBlock->m_CopGenArray[nGen].fAlpha=(pPRBlock->m_CopGenArray[nGen].fCapH-pPRBlock->m_CopGenArray[nGen].fCapacity)/fPStep;
			}
			else
			{
				nRatio=(int)ceil(pPRBlock->m_CopGenArray[nGen].fCapacity/fPStep);
				pPRBlock->m_CopGenArray[nGen].b3State=0;
				pPRBlock->m_CopGenArray[nGen].fCapL=0;
				pPRBlock->m_CopGenArray[nGen].fCapH=nRatio*fPStep;
				pPRBlock->m_CopGenArray[nGen].fAlpha=1;
			}
		}

		SortCopGenCapacity(pPRBlock, 0, pPRBlock->m_nRecordNum[PR_COPGEN]-1);
	}

	void CPRCopTable::CopTable(tagPRBlock* pPRBlock, const double fPStep, const char* lpszCoptResultFile)
	{
		register int	i;
		int		nTotState, nState;
		double	fTotalCapacity;

		pPRBlock->m_System.fCopStep=fPStep;
		CopInit(pPRBlock, pPRBlock->m_System.fCopStep);
		//CoptExpriment2(pPRBlock);

		if (pPRBlock->m_nRecordNum[PR_COPGEN] <= 0)
			return;

		fTotalCapacity=0;
		for (i=0; i<pPRBlock->m_nRecordNum[PR_COPGEN]; i++)
			fTotalCapacity += pPRBlock->m_CopGenArray[i].fCapacity;

		nTotState=(int)ceil(fTotalCapacity/pPRBlock->m_System.fCopStep)+1;
		m_CopTabArray.resize(nTotState);
		for (i=0; i<(int)m_CopTabArray.size(); i++)
			memset(&m_CopTabArray[i], 0, sizeof(tagPRCopTable));

		m_CopTabArray[0].fOutageCapacity=0;
		m_CopTabArray[0].fAvailCapacity=fTotalCapacity;
		for (i=1; i<nTotState; i++)
		{
			m_CopTabArray[i].fOutageCapacity = m_CopTabArray[i-1].fOutageCapacity+pPRBlock->m_System.fCopStep;
			m_CopTabArray[i].fAvailCapacity  = m_CopTabArray[i-1].fAvailCapacity -pPRBlock->m_System.fCopStep;
			m_CopTabArray[i].fStateProb=0;
			m_CopTabArray[i].fAccumProb=0;
			m_CopTabArray[i].fStateFreq=0;
			m_CopTabArray[i].fAccumFreq=0;
		}

		//////////////////////////////////////////////////////////////////////////
		//	��0�͵�һ̨�������ֵ��
		m_CopTabArray[0].fStateProb=1.0-pPRBlock->m_CopGenArray[0].fRout;			//	0���� ״̬����
		m_CopTabArray[0].fDepRP=0;													//	0���� +��ȥ��
		m_CopTabArray[0].fDepRN=pPRBlock->m_CopGenArray[0].fRerr;					//	0���� -��ȥ��

		if (pPRBlock->m_CopGenArray[0].b3State)
		{
			nState=(int)(pPRBlock->m_CopGenArray[0].fCapL/pPRBlock->m_System.fCopStep);
			m_CopTabArray[nState].fStateProb=pPRBlock->m_CopGenArray[0].fAlpha*pPRBlock->m_CopGenArray[0].fRout;	//	��һ̨������ ״̬����
			m_CopTabArray[nState].fDepRP=pPRBlock->m_CopGenArray[0].fRrep;											//	��һ̨������ -��ȥ��
			m_CopTabArray[nState].fDepRN=0;																			//	��һ̨������ -��ȥ��

			nState=(int)(pPRBlock->m_CopGenArray[0].fCapH/pPRBlock->m_System.fCopStep);
			m_CopTabArray[nState].fStateProb=pPRBlock->m_CopGenArray[0].fRout;					//	��һ̨������ ״̬����
			m_CopTabArray[nState].fDepRP=pPRBlock->m_CopGenArray[0].fRrep;						//	��һ̨������ -��ȥ��
			m_CopTabArray[nState].fDepRN=0;														//	��һ̨������ -��ȥ��
		}
		else
		{
			nState=(int)(pPRBlock->m_CopGenArray[0].fCapH/pPRBlock->m_System.fCopStep);
			m_CopTabArray[nState].fStateProb=pPRBlock->m_CopGenArray[0].fRout;					//	��һ̨������ ״̬����
			m_CopTabArray[nState].fDepRN=0;														//	��һ̨������ -��ȥ��
			m_CopTabArray[nState].fDepRP=pPRBlock->m_CopGenArray[0].fRrep;						//	��һ̨������ -��ȥ��
		}

		//////////////////////////////////////////////////////////////////////////
		//	�����һ̨�����ۼƸ���
		m_CopTabArray[nState].fAccumProb=m_CopTabArray[nState].fStateProb;
		for (i=nState-1; i>=0; i--)
			m_CopTabArray[i].fAccumProb = m_CopTabArray[i+1].fAccumProb+m_CopTabArray[i].fStateProb;

		//////////////////////////////////////////////////////////////////////////
		//	�����һ̨�����ۼ�Ƶ��
		m_CopTabArray[0].fAccumFreq=0;
		for (i=1; i<=nState; i++)
			m_CopTabArray[i].fAccumFreq=pPRBlock->m_CopGenArray[0].fRout*pPRBlock->m_CopGenArray[0].fRrep;

		//////////////////////////////////////////////////////////////////////////
		//	�ӵڶ�̨����ʼ�ݹ�
		CopStep(pPRBlock, pPRBlock->m_System.fCopStep, pPRBlock->m_CopGenArray[0].fCapacity, 1);
		m_CopTabArray[0].fAccumFreq=0;

		pPRBlock->m_nRecordNum[PR_COPTABLE]=0;
		for (i=0; i<(int)m_CopTabArray.size(); i++)
		{
			if (m_CopTabArray[i].fAvailCapacity < 0)
				continue;

			pPRBlock->m_CopTableArray[pPRBlock->m_nRecordNum[PR_COPTABLE]].nState			=m_CopTabArray[i].nState;
			pPRBlock->m_CopTableArray[pPRBlock->m_nRecordNum[PR_COPTABLE]].fOutageCapacity	=m_CopTabArray[i].fOutageCapacity;
			pPRBlock->m_CopTableArray[pPRBlock->m_nRecordNum[PR_COPTABLE]].fAvailCapacity	=m_CopTabArray[i].fAvailCapacity;
			pPRBlock->m_CopTableArray[pPRBlock->m_nRecordNum[PR_COPTABLE]].fStateProb		=m_CopTabArray[i].fStateProb;		//	ȷ�и���
			pPRBlock->m_CopTableArray[pPRBlock->m_nRecordNum[PR_COPTABLE]].fAccumProb		=m_CopTabArray[i].fAccumProb;		//	�ۻ�����
			pPRBlock->m_CopTableArray[pPRBlock->m_nRecordNum[PR_COPTABLE]].fStateFreq		=m_CopTabArray[i].fStateFreq;		//	����Ƶ��
			pPRBlock->m_CopTableArray[pPRBlock->m_nRecordNum[PR_COPTABLE]].fAccumFreq		=m_CopTabArray[i].fAccumFreq;		//	�ۻ�Ƶ��

			pPRBlock->m_CopTableArray[pPRBlock->m_nRecordNum[PR_COPTABLE]].fDepRP			=m_CopTabArray[i].fDepRP;
			pPRBlock->m_CopTableArray[pPRBlock->m_nRecordNum[PR_COPTABLE]].fDepRN			=m_CopTabArray[i].fDepRN;

			pPRBlock->m_nRecordNum[PR_COPTABLE]++;
		}
		//Log(g_lpszLogFile, "OutageP\t ExactProb\t AccumProb\t ExactFreq\t AccumFreq\t ��+\t ��-\t\n");
		//for (i=0; i<m_CopTabArray.size(); i++)
		//	Log(g_lpszLogFile, "%f\t %f\t %f\t %.12f\t %.12f\t %f\t %f\n", m_CopTabArray[i].fOutageCapacity, m_CopTabArray[i].fStateProb, m_CopTabArray[i].fAccumProb, m_CopTabArray[i].fStateFreq, m_CopTabArray[i].fAccumFreq, m_CopTabArray[i].fDepRP, m_CopTabArray[i].fDepRN);

		char	szRResultFile[260];
		if (!lpszCoptResultFile)
		{
			char	szTempPath[260];
			GetTempPath(260, szTempPath);
			sprintf(szRResultFile, "%s/%s", szTempPath, g_lpszCoptResultFileName);
		}
		else
			strcpy(szRResultFile, lpszCoptResultFile);
		SaveCoptXml(pPRBlock, szRResultFile);
	}

	void CPRCopTable::CopStep(tagPRBlock* pPRBlock, const double fPStep, const double fPrevCapacity, const int nAGen)
	{
		if (nAGen >= pPRBlock->m_nRecordNum[PR_COPGEN])
			return;

		register int	i;
		int		nState1, nState2;
		int		nStep, nTotStep, nCapStep;
		double	fRerr, fRrep, fRout, fAlpha, fProb, fFreq;

#ifdef _ExactStateFreq
		double	fDepRP, fDepRN;
#endif

		std::vector<tagPRCopTable>	pCopArray;
		pCopArray.assign(m_CopTabArray.begin(), m_CopTabArray.end());

		fRerr=pPRBlock->m_CopGenArray[nAGen].fRerr;	//	��
		fRrep=pPRBlock->m_CopGenArray[nAGen].fRrep;	//	��
		fRout=pPRBlock->m_CopGenArray[nAGen].fRout;
		fAlpha=pPRBlock->m_CopGenArray[nAGen].fAlpha;
		nTotStep=(int)ceil((fPrevCapacity+pPRBlock->m_CopGenArray[nAGen].fCapacity)/fPStep);
		nCapStep=(int)ceil(pPRBlock->m_CopGenArray[nAGen].fCapacity/fPStep);

		if (pPRBlock->m_CopGenArray[nAGen].b3State)
		{
			//////////////////////////////////////////////////////////////////////////
			//	�����������״̬��ȷ��״̬����
			//	p(X)=(1-��)*p'(X)+��*��*p'(X-C1)+(1-��)*p'(X-C2)
			nStep=0;
			while (nStep <= nTotStep)
			{
				nState1=nStep;

				m_CopTabArray[nState1].fStateProb=(1-fRout)*pCopArray[nState1].fStateProb;

				nState2=nStep-(int)(pPRBlock->m_CopGenArray[nAGen].fCapL/fPStep);
				if (nState2 >= 0)
					m_CopTabArray[nState1].fStateProb += fAlpha*fRout*pCopArray[nState2].fStateProb;

				nState2=nStep-(int)(pPRBlock->m_CopGenArray[nAGen].fCapH/fPStep);
				if (nState2 >= 0)
					m_CopTabArray[nState1].fStateProb += (1-fAlpha)*fRout*pCopArray[nState2].fStateProb;

				nStep++;
			}

#ifdef _ExactStateFreq
			//////////////////////////////////////////////////////////////////////////
			//	�������������״̬��״̬��ȥ��
			//	��+=(p'(X)*(1-r)* ��+'(X)    +��*r*p'(X-C1)*(��+'(X-C1)+��))+(1-��)*r*p'(X-C2)*(��+'(X-C2)+��))/p(X)
			//	��-=(p'(X)*(1-r)*(��-'(X)+��)+��*r*p'(X-C1)* ��+'(X-C1)     +(1-��)*r*p'(X-C2)* ��+'(X-C2)    )/p(X)
			nStep=0;
			while (nStep <= nTotStep)
			{
				nState1=nStep;

				m_CopTabArray[nState1].fDepRP=0;
				m_CopTabArray[nState1].fDepRN=0;
				if (m_CopTabArray[nState1].fStateProb >= 1.0/1000000000)
				{
					m_CopTabArray[nState1].fDepRP=pCopArray[nState1].fStateProb*(1-fRout)* pCopArray[nState1].fDepRP		;
					m_CopTabArray[nState1].fDepRN=pCopArray[nState1].fStateProb*(1-fRout)*(pCopArray[nState1].fDepRN+fRerr)	;

					//nState2=nStep-(int)(pPRBlock->m_CopGenArray[nAGen].fCapH/fPStep);
					//fDepRP=(nState2 >= 0) ? pCopArray[nState2].fDepRP : 0;
					//fDepRN=(nState2 >= 0) ? pCopArray[nState2].fDepRN : 0;
					//fProb= (nState2 >= 0) ? pCopArray[nState2].fStateProb : 0;
					//m_CopTabArray[nState1].fDepRP += fProb*fRout*(fDepRP+fRrep)	;
					//m_CopTabArray[nState1].fDepRN += fProb*fRout* fDepRN		;

					nState2=nStep-(int)(pPRBlock->m_CopGenArray[nAGen].fCapL/fPStep);
					fDepRP=(nState2 >= 0) ? pCopArray[nState2].fDepRP : 0;
					fDepRN=(nState2 >= 0) ? pCopArray[nState2].fDepRN : 0;
					fProb= (nState2 >= 0) ? pCopArray[nState2].fStateProb : 0;
					m_CopTabArray[nState1].fDepRP += fProb*fRout*(fDepRP+fRrep)	;
					m_CopTabArray[nState1].fDepRN += fProb*fRout* fDepRN		;

					nState2=nStep-(int)(pPRBlock->m_CopGenArray[nAGen].fCapH/fPStep);
					fDepRP=(nState2 >= 0) ? pCopArray[nState2].fDepRP : 0;
					fDepRN=(nState2 >= 0) ? pCopArray[nState2].fDepRN : 0;
					fProb= (nState2 >= 0) ? pCopArray[nState2].fStateProb : 0;
					m_CopTabArray[nState1].fDepRP += fProb*fRout*(fDepRP+fRrep)	;
					m_CopTabArray[nState1].fDepRN += fProb*fRout* fDepRN		;

					m_CopTabArray[nState1].fDepRP /= m_CopTabArray[nState1].fStateProb;
					m_CopTabArray[nState1].fDepRN /= m_CopTabArray[nState1].fStateProb;
				}

				nStep++;
			}
#endif

			//////////////////////////////////////////////////////////////////////////
			//	�������������״̬���ۼ�Ƶ��
			//	F(X)=(1-r)*(F'(X)-��*P'(X))+��*r*(F'(X-C1)+��*P'(X-C1)+(1-��)*r*(F'(X-C2)+��*P'(X-C2))
			nStep=0;
			while (nStep <= nTotStep)
			{
				nState1=nStep;
				nState2=nStep-nCapStep;

				m_CopTabArray[nState1].fAccumFreq=(1-fRout)*(pCopArray[nState1].fAccumFreq-fRerr*pCopArray[nState1].fAccumProb);

				//nState2=nStep-(int)ceil(pPRBlock->m_CopGenArray[nAGen].fCapH/fPStep);
				//fFreq=(nState2 >= 0) ? pCopArray[nState2].fAccumFreq : 0;
				//fProb=(nState2 >= 0) ? pCopArray[nState2].fAccumProb : 1;
				//m_CopTabArray[nState1].fAccumFreq += fRout*(fFreq+fRrep*fProb);

				nState2=nStep-(int)ceil(pPRBlock->m_CopGenArray[nAGen].fCapL/fPStep);
				fFreq=(nState2 >= 0) ? pCopArray[nState2].fAccumFreq : 0;
				fProb=(nState2 >= 0) ? pCopArray[nState2].fAccumProb : 1;
				m_CopTabArray[nState1].fAccumFreq += fAlpha*fRout*(fFreq+fRrep*fProb);

				nState2=nStep-(int)ceil(pPRBlock->m_CopGenArray[nAGen].fCapH/fPStep);
				fFreq=(nState2 >= 0) ? pCopArray[nState2].fAccumFreq : 0;
				fProb=(nState2 >= 0) ? pCopArray[nState2].fAccumProb : 1;
				m_CopTabArray[nState1].fAccumFreq += (1-fAlpha)*fRout*(fFreq+fRrep*fProb);

				nStep++;
			}
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			//	�����������״̬��ȷ��״̬����
			//	p(X)=(1-��)*p'(X)+��*p'(X-C)
			nStep=0;
			while (nStep <= nTotStep)
			{
				nState1=nStep;
				nState2=nStep-nCapStep;

				m_CopTabArray[nState1].fStateProb=(1-fRout)*pCopArray[nState1].fStateProb;
				if (nState2 >= 0)
					m_CopTabArray[nState1].fStateProb += fRout*pCopArray[nState2].fStateProb;

				nStep++;
			}

#ifdef _ExactStateFreq
			//////////////////////////////////////////////////////////////////////////
			//	�������������״̬��״̬��ȥ��
			//	��+=(p'(X)*(1-r)* ��+'(X)    +p'(X-C)*r*(��+'(X-C)+��))/p(X)
			//	��-=(p'(X)*(1-r)*(��-'(X)+��)+p'(X-C)*r* ��+'(X-C))    /p(X)
			nStep=0;
			while (nStep <= nTotStep)
			{
				nState1=nStep;
				nState2=nStep-nCapStep;

				m_CopTabArray[nState1].fDepRP=0;
				m_CopTabArray[nState1].fDepRN=0;
				if (m_CopTabArray[nState1].fStateProb >= 1.0/1000000000)
				{
					m_CopTabArray[nState1].fDepRP=pCopArray[nState1].fStateProb*(1-fRout)* pCopArray[nState1].fDepRP		;
					m_CopTabArray[nState1].fDepRN=pCopArray[nState1].fStateProb*(1-fRout)*(pCopArray[nState1].fDepRN+fRerr)	;

					fDepRP=(nState2 >= 0) ? pCopArray[nState2].fDepRP : 0;
					fDepRN=(nState2 >= 0) ? pCopArray[nState2].fDepRN : 0;
					fProb= (nState2 >= 0) ? pCopArray[nState2].fStateProb : 0;
					m_CopTabArray[nState1].fDepRP += fProb*fRout*(fDepRP+fRrep)	;
					m_CopTabArray[nState1].fDepRN += fProb*fRout* fDepRN		;

					m_CopTabArray[nState1].fDepRP /= m_CopTabArray[nState1].fStateProb;
					m_CopTabArray[nState1].fDepRN /= m_CopTabArray[nState1].fStateProb;
				}

				nStep++;
			}
#endif

			//////////////////////////////////////////////////////////////////////////
			//	�������������״̬���ۼ�Ƶ��
			//	F(X)=(1-r)*(F'(X)-��*P'(X))+r*(F'(X-C)+��*P'(X-C))
			nStep=0;
			while (nStep <= nTotStep)
			{
				nState1=nStep;
				nState2=nStep-nCapStep;

				m_CopTabArray[nState1].fAccumFreq=(1-fRout)*(pCopArray[nState1].fAccumFreq-fRerr*pCopArray[nState1].fAccumProb);

				fFreq=(nState2 >= 0) ? pCopArray[nState2].fAccumFreq : 0;
				fProb=(nState2 >= 0) ? pCopArray[nState2].fAccumProb : 1;
				m_CopTabArray[nState1].fAccumFreq += fRout*(fFreq+fRrep*fProb);

				if (m_CopTabArray[nState1].fAccumFreq < 0)	m_CopTabArray[nState1].fAccumFreq=0;
				nStep++;
			}

		}

		//////////////////////////////////////////////////////////////////////////
		//	�������������״̬���ۼ�״̬����
		m_CopTabArray[nTotStep].fAccumProb = m_CopTabArray[nTotStep].fStateProb;
		for (i=nTotStep-1; i>=0; i--)
			m_CopTabArray[i].fAccumProb = m_CopTabArray[i+1].fAccumProb+m_CopTabArray[i].fStateProb;

		pCopArray.clear();

#ifdef _ExactStateFreq
		//////////////////////////////////////////////////////////////////////////
		//	����ȷ��Ƶ��
		for (i=0; i<(int)m_CopTabArray.size(); i++)
			m_CopTabArray[i].fStateFreq=m_CopTabArray[i].fStateProb*(m_CopTabArray[i].fDepRP+m_CopTabArray[i].fDepRN);
#endif // _ExactStateFreq

		CopStep(pPRBlock, fPStep, fPrevCapacity+pPRBlock->m_CopGenArray[nAGen].fCapacity, nAGen+1);
	}

	void CPRCopTable::SortCopGenCapacity(tagPRBlock* pPRBlock, int nDn0, int nUp0)
	{
		if (nUp0 <= nDn0)
			return;

		int nDn = nDn0;
		int nUp = nUp0;

		double	fCapacity=pPRBlock->m_CopGenArray[(nDn0+nUp0)/2].fCapacity;
		while (nDn <= nUp)
		{
			while (nDn < nUp0 && pPRBlock->m_CopGenArray[nDn].fCapacity < fCapacity)
				++nDn;								 
			while (nUp > nDn0 && pPRBlock->m_CopGenArray[nUp].fCapacity > fCapacity)
				--nUp;

			if (nDn <= nUp)
			{
				tagPRCopGen	gBuf;
				memcpy(&gBuf,							&pPRBlock->m_CopGenArray[nDn],	sizeof(tagPRCopGen));
				memcpy(&pPRBlock->m_CopGenArray[nDn],	&pPRBlock->m_CopGenArray[nUp],	sizeof(tagPRCopGen));
				memcpy(&pPRBlock->m_CopGenArray[nUp],	&gBuf,							sizeof(tagPRCopGen));

				++nDn;
				--nUp;
			}
		}

		if (nDn0 < nUp)
			SortCopGenCapacity(pPRBlock, nDn0, nUp);

		if (nDn < nUp0 )
			SortCopGenCapacity(pPRBlock, nDn, nUp0);
	}

	void CPRCopTable::CoptExpriment1(tagPRBlock* pPRBlock)
	{
		register int	i;

		pPRBlock->m_System.fCopStep=50;
		pPRBlock->m_nRecordNum[PR_COPGEN]=4;
		for (i=0; i<pPRBlock->m_nRecordNum[PR_COPGEN]; i++)
			memset(&pPRBlock->m_CopGenArray[i], 0, sizeof(tagPRCopGen));

		pPRBlock->m_nRecordNum[PR_COPGEN]=0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=50;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=50;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.04;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.0011;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=38.46154;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.026;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;

		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=50;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.04;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.0011;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=38.46154;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.026;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;

		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=2;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=50;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.04;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.0011;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=38.46154;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.026;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;

		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=2;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=50;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.04;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.0011;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=38.46154;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.026;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;
	}

	void CPRCopTable::CoptExpriment2(tagPRBlock* pPRBlock)
	{
		register int	i;

		pPRBlock->m_System.fCopStep=50;
		pPRBlock->m_nRecordNum[PR_COPGEN]=3;
		for (i=0; i<pPRBlock->m_nRecordNum[PR_COPGEN]; i++)
			memset(&pPRBlock->m_CopGenArray[i], 0, sizeof(tagPRCopGen));

		pPRBlock->m_nRecordNum[PR_COPGEN]=0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=100;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.01;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.00505;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=2.0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.5;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;

		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=150;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.02;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.01020;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=2.0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.5;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;

		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=2;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=200;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.03;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.01237;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=2.5;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.4;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;
	}


	void CPRCopTable::CoptExpriment3(tagPRBlock* pPRBlock)
	{
		register int	i;

		pPRBlock->m_System.fCopStep=50;
		pPRBlock->m_nRecordNum[PR_COPGEN]=4;
		for (i=0; i<pPRBlock->m_nRecordNum[PR_COPGEN]; i++)
			memset(&pPRBlock->m_CopGenArray[i], 0, sizeof(tagPRCopGen));

		pPRBlock->m_nRecordNum[PR_COPGEN]=0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=10;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.01;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.00505;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=2.0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.5;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;

		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=56;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.02;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.01020;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=2.0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.5;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;

		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=2;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=56;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.03;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.01237;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=2.5;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.4;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;

		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=2;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=100;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.01;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.00505;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=2.0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.5;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;
	}

	void CPRCopTable::CoptExpriment4(tagPRBlock* pPRBlock)
	{
		pPRBlock->m_System.fCopStep=50;
		pPRBlock->m_nRecordNum[PR_COPGEN]=3;
		register int	i;
		for (i=0; i<pPRBlock->m_nRecordNum[PR_COPGEN]; i++)
			memset(&pPRBlock->m_CopGenArray[i], 0, sizeof(tagPRCopGen));

		pPRBlock->m_nRecordNum[PR_COPGEN]=0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=10;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.01;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.00505;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=2.0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.5;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;

		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=56;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.02;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.01020;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=2.0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.5;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;

		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=2;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=56;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.03;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.01237;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=2.5;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.4;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;
	}

	void CPRCopTable::CoptExpriment5(tagPRBlock* pPRBlock)
	{
		register int	i;

		pPRBlock->m_System.fCopStep=50;
		pPRBlock->m_nRecordNum[PR_COPGEN]=4;

		for (i=0; i<pPRBlock->m_nRecordNum[PR_COPGEN]; i++)
			memset(&pPRBlock->m_CopGenArray[i], 0, sizeof(tagPRCopGen));

		pPRBlock->m_nRecordNum[PR_COPGEN]=0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].b3State=0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=6;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.01;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.00505;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=2.0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.5;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;

		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].b3State=0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=46;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.02;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.01020;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=2.0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.5;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;

		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=2;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].b3State=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=50;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=100;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=56;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=(100.0-56.0)/50.0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.03;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.01237;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=2.5;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.4;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;

		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].nGenIndex=3;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].b3State=0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapL=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapH=pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fCapacity=100;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fAlpha=1;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRout=0.01;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRerr=0.00505;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fTrep=2.0;
		pPRBlock->m_CopGenArray[pPRBlock->m_nRecordNum[PR_COPGEN]].fRrep=0.5;
		pPRBlock->m_nRecordNum[PR_COPGEN]++;
	}

	void CPRCopTable::SaveCoptXml(tagPRBlock* pPRBlock, const char* lpszResultFileName)
	{
		register int		i;
		TiXmlDeclaration*	pDeclare;
		TiXmlDocument*		pDocument;
		TiXmlElement		*pRoot, *pElement;

		pDocument = new TiXmlDocument();						//����һ��XML���ĵ�����

		pDeclare = new TiXmlDeclaration("1.0", "gb2312", "no");
		pDocument->LinkEndChild(pDeclare);

		pRoot = new TiXmlElement("PRCoptTable");			//����һ����Ԫ�ز����ӡ�
		pDocument->LinkEndChild(pRoot);

		for (i=0; i<pPRBlock->m_nRecordNum[PR_COPGEN]; i++)
		{
			pElement = new TiXmlElement(g_PRMemDBInterface.PRGetTableName(PR_COPGEN));
			pRoot->LinkEndChild(pElement);

			pElement->SetAttribute("Name", pPRBlock->m_GeneratorArray[pPRBlock->m_CopGenArray[i].nGenIndex].szName);
			pElement->SetDoubleAttribute(g_PRMemDBInterface.PRGetFieldName(PR_COPGEN, PR_COPGEN_CAPACITY),		pPRBlock->m_CopGenArray[i].fCapacity);
			pElement->SetDoubleAttribute(g_PRMemDBInterface.PRGetFieldName(PR_COPGEN, PR_COPGEN_RERR),			pPRBlock->m_CopGenArray[i].fRerr);
			pElement->SetDoubleAttribute(g_PRMemDBInterface.PRGetFieldName(PR_COPGEN, PR_COPGEN_TREP),			pPRBlock->m_CopGenArray[i].fTrep);
			pElement->SetDoubleAttribute(g_PRMemDBInterface.PRGetFieldName(PR_COPGEN, PR_COPGEN_RREP),			pPRBlock->m_CopGenArray[i].fRrep);
			pElement->SetDoubleAttribute(g_PRMemDBInterface.PRGetFieldName(PR_COPGEN, PR_COPGEN_ROUT),			pPRBlock->m_CopGenArray[i].fRout);

			pElement->SetDoubleAttribute(g_PRMemDBInterface.PRGetFieldName(PR_COPGEN, PR_COPGEN_S1CAPACITY),	pPRBlock->m_CopGenArray[i].fCapL);
			pElement->SetDoubleAttribute(g_PRMemDBInterface.PRGetFieldName(PR_COPGEN, PR_COPGEN_S2CAPACITY),	pPRBlock->m_CopGenArray[i].fCapH);
			pElement->SetDoubleAttribute(g_PRMemDBInterface.PRGetFieldName(PR_COPGEN, PR_COPGEN_ALPHA),		pPRBlock->m_CopGenArray[i].fAlpha);
		}

		for (i=0; i<pPRBlock->m_nRecordNum[PR_COPTABLE]; i++)
		{
			pElement = new TiXmlElement(g_PRMemDBInterface.PRGetTableName(PR_COPTABLE));
			pRoot->LinkEndChild(pElement);

			pElement->SetAttribute(g_PRMemDBInterface.PRGetFieldName(PR_COPTABLE, PR_COPTABLE_STATE),					i+1);
			pElement->SetDoubleAttribute(g_PRMemDBInterface.PRGetFieldName(PR_COPTABLE, PR_COPTABLE_OUTAGECAPACITY),	pPRBlock->m_CopTableArray[i].fOutageCapacity);
			pElement->SetDoubleAttribute(g_PRMemDBInterface.PRGetFieldName(PR_COPTABLE, PR_COPTABLE_AVAILCAPACITY),	pPRBlock->m_CopTableArray[i].fAvailCapacity);
			pElement->SetDoubleAttribute(g_PRMemDBInterface.PRGetFieldName(PR_COPTABLE, PR_COPTABLE_STATEPROB),		pPRBlock->m_CopTableArray[i].fStateProb);
			pElement->SetDoubleAttribute(g_PRMemDBInterface.PRGetFieldName(PR_COPTABLE, PR_COPTABLE_ACCUMPROB),		pPRBlock->m_CopTableArray[i].fAccumProb);
			pElement->SetDoubleAttribute(g_PRMemDBInterface.PRGetFieldName(PR_COPTABLE, PR_COPTABLE_ACCUMFREQ),		pPRBlock->m_CopTableArray[i].fAccumFreq);
		}

		pDocument->SaveFile(lpszResultFileName);					//���浽�ļ�

		pDocument->Clear();
		delete pDocument;
	}
}

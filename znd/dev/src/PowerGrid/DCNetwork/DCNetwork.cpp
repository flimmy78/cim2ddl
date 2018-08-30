#include "DCNetwork.h"

extern	const	char*	g_lpszLogFile;
extern	void	Log(const char* lpszLogFile, const char* pformat, ...);
namespace DCNetwork
{
	const	char*	g_lpszDCNetworkLogFile="DCNetwork.log";
	CPRMemDBInterface	g_PRMemDBInterface;

	CDCNetwork::CDCNetwork(void)
	{
		EmptyDataBuffer();
	}

	CDCNetwork::~CDCNetwork(void)
	{
		EmptyDataBuffer();
	}

	void CDCNetwork::EmptyDataBuffer()
	{
		if (!m_fZArray.empty())		m_fZArray.clear();
		if (!m_fGenArray.empty())	m_fGenArray.clear();
		if (!m_fLoadArray.empty())	m_fLoadArray.clear();

		if (!m_nBusOld.empty())		m_nBusOld.clear();
		if (!m_nBusNew.empty())		m_nBusNew.clear();

		if (!m_fMatrixD.empty())	m_fMatrixD.clear();
		if (!m_fMatrixU.empty())	m_fMatrixU.clear();
		if (!m_nMatrixIU.empty())	m_nMatrixIU.clear();

		if (!m_fBII.empty())		m_fBII.clear();
		if (!m_fWkRow.empty())		m_fWkRow.clear();

		if (!m_nNumFD.empty())		m_nNumFD.clear();
		if (!m_nNumDZ.empty())		m_nNumDZ.clear();

		if (!m_fBusD.empty())		m_fBusD.clear();

		if (!m_nNOB.empty())		m_nNOB.clear();
		if (!m_nNOCN.empty())		m_nNOCN.clear();
		if (!m_nNDS.empty())		m_nNDS.clear();
	}

	void CDCNetwork::AllocDataBuffer(const int nMaxBus)
	{
		//Log(g_lpszDCNetworkLogFile, "AllocDataBuffer MaxBusNum=%d\n", nMaxBus);

		EmptyDataBuffer();

		m_fZArray.resize(4*(m_nNumBra+1));
		m_fGenArray.resize(3*(m_nNumGen+1));
		m_fLoadArray.resize(3*(m_nNumLoad+1));

		m_nBusOld.resize(nMaxBus+1);
		m_nBusNew.resize(nMaxBus+1);

		m_fMatrixD.resize(nMaxBus+1);
		m_fMatrixU.resize(20*(nMaxBus+1));
		m_nMatrixIU.resize(20*(nMaxBus+1));

		m_fBII.resize(nMaxBus+1);
		m_fWkRow.resize(nMaxBus+1);

		m_nNumFD.resize(nMaxBus+1);
		m_nNumDZ.resize(nMaxBus+2);

		m_fBusD.resize(nMaxBus+1);

		m_nNOB.resize(nMaxBus+1);
		m_nNOCN.resize(MaxBPerBus*(nMaxBus+1));
		m_nNDS.resize(nMaxBus+1);
	}

	void CDCNetwork::DCNetwork_NewZ()
	{
		register int	i;
		int		nBusI, nBusJ;
		int		nNBusI, nNBusJ;
		unsigned char	bTran;

		std::vector<int>	nZBusArray;
		std::vector<double>	fBranArray;
		nZBusArray.resize(2*(m_nNumBra+1), 0);
		fBranArray.resize(4*(m_nNumBra+1), 0);

		//for (i=0; i<DCN_MaxBus; i++)
		for (i=0; i<(int)m_nNOB.size(); i++)
			m_nNOB[i]=0;

		for (i=0; i<(int)nZBusArray.size(); i++)
			nZBusArray[i]=0;
		for (i=0; i<(int)fBranArray.size(); i++)
			fBranArray[i]=0;

		for (i=0; i<m_nNumBra; i++)
		{
			bTran=0;

			nBusI=(int)m_fZArray[4*i+0];
			nBusJ=(int)m_fZArray[4*i+1];
			if (nBusJ < 0)
			{
				nBusJ=abs(nBusJ);
				bTran=1;
			}
			nNBusI=m_nBusNew[nBusI];
			nNBusJ=m_nBusNew[nBusJ];

			if (nNBusI <= 0 || nNBusJ <= 0)
				Log(g_lpszDCNetworkLogFile, "֧· %d <-> %d [%d] �ڵ����Ż�������\n", nBusI, nBusJ, (int)m_fZArray[4*i+3]);

			//if (nNBusI <= 0)
			//	ASSERT(FALSE);
			//if (nNBusJ <= 0)
			//	ASSERT(FALSE);
			if (bTran == 0)
			{
				if (nNBusI < nNBusJ)
				{
					nZBusArray[2*i  ]=nNBusI;
					nZBusArray[2*i+1]=nNBusJ;
					m_nNOB[nNBusI]=m_nNOB[nNBusI]+1;
				}
				else
				{
					nZBusArray[2*i  ]=nNBusJ;
					nZBusArray[2*i+1]=nNBusI;
					m_nNOB[nNBusJ]=m_nNOB[nNBusJ]+1;
				}
			}
			else
			{
				if (nNBusI < nNBusJ)
				{
					nZBusArray[2*i  ]=nNBusI;
					nZBusArray[2*i+1]=-nNBusJ;
					m_nNOB[nNBusI]=m_nNOB[nNBusI]+1;
				}
				else
				{
					nZBusArray[2*i  ]=nNBusJ;
					nZBusArray[2*i+1]=-nNBusI;
					m_nNOB[nNBusJ]=m_nNOB[nNBusJ]+1;
				}
			}
		}

		m_nNOB[m_nNumBus]=m_nNumBra;
		for (i=m_nNumBus-1; i>=1; i--)
			m_nNOB[i]=m_nNOB[i+1]-m_nNOB[i];

		if (g_bWriteDebug)
		{
			Log(g_lpszDCNetworkLogFile, "ĸ�ߺͳ�����\n");
			for (i=m_nNumBus; i>=1; i--)
			{
				Log(g_lpszDCNetworkLogFile, "Bus=(%d/%d) Nob=%d\n", i, m_nNumBus, m_nNOB[i]);
			}
			Log(g_lpszDCNetworkLogFile, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		}
		//if (m_nNOB[1] != 0)
		//	ASSERT(FALSE);

		for (i=0; i<m_nNumBra; i++)
		{
			nBusI=nZBusArray[2*i];
			fBranArray[4*m_nNOB[nBusI]+0]=nZBusArray[2*i+0];
			fBranArray[4*m_nNOB[nBusI]+1]=nZBusArray[2*i+1];
			fBranArray[4*m_nNOB[nBusI]+2]=m_fZArray[4*i+2];
			fBranArray[4*m_nNOB[nBusI]+3]=m_fZArray[4*i+3];
			m_nNOB[nBusI]++;
		}

		for (i=0; i<m_nNumBra; i++)
		{
			m_fZArray[4*i+0]=fBranArray[4*i+0];
			m_fZArray[4*i+1]=fBranArray[4*i+1];
			m_fZArray[4*i+2]=fBranArray[4*i+2];
			m_fZArray[4*i+3]=fBranArray[4*i+3];
		}

		nZBusArray.clear();
		fBranArray.clear();

		//for (i=0; i<m_nNumBra; i++)
		//{
		//	Log(g_lpszDCNetworkLogFile, "OldBran[%d/%d] %.0f %.0f %.5f %.0f\n", i, m_nNumBra, m_fZArray[4*i+0], m_fZArray[4*i+1], m_fZArray[4*i+2], m_fZArray[4*i+3]);
		//	Log(g_lpszDCNetworkLogFile, "NewBran[%d/%d] %.0f %.0f %.5f %.0f\n", i, m_nNumBra, m_fZArray[4*i+0], m_fZArray[4*i+1], m_fZArray[4*i+2], m_fZArray[4*i+3]);
		//}
	}

	//	ԭ�����ƽ��ĸ�߽ڵ����Ż������ã��Ѿ�������
	void CDCNetwork::DCNetwork_Rename()
	{
		register int	i, j, k;
		int		nBusI, nBusJ;
		int		bFlaged;
		int		nMinBus, nMinBran, nBranNum;
		int		nCurBus, nOppBus;
		int		nProced;

		//for (i=0; i<DCN_MaxBus; i++)
		for (i=0; i<(int)m_nNOB.size(); i++)
			m_nNOB[i]=0;
		//for (i=0; i<MaxBPerBus*DCN_MaxBus; i++)
		for (i=0; i<(int)m_nNOCN.size(); i++)
			m_nNOCN[i]=0;
		//for (i=0; i<DCN_MaxBus; i++)
		for (i=0; i<(int)m_nBusNew.size(); i++)
			m_nBusNew[i]=0;
		//for (i=0; i<DCN_MaxBus; i++)
		for (i=0; i<(int)m_nBusOld.size(); i++)
			m_nBusOld[i]=0;

		//	ͳ�Ƹ��ڵ���������֧·��, ˫�ػ����صȲ���֧·��һ����
		for (i=0; i<m_nNumBra; i++)
		{
			nBusI=(int)m_fZArray[4*i+0];
			nBusJ=(int)fabs(m_fZArray[4*i+1]);
			if (nBusI == 0 || nBusJ == 0)
				continue;

			bFlaged=0;
			for (j=0; j<=i-1; j++)
			{
				if (nBusI == (int)m_fZArray[4*j] && nBusJ == (int)fabs(m_fZArray[4*j+1]) ||
					nBusJ == (int)m_fZArray[4*j] && nBusI == (int)fabs(m_fZArray[4*j+1]))
				{
					bFlaged=1;
					break;
				}
			}
			if (bFlaged)
				continue;

			m_nNOB[nBusI]++;
			m_nNOB[nBusJ]++;

			m_nNOCN[MaxBPerBus*nBusI+m_nNOB[nBusI]]=nBusJ;
			m_nNOCN[MaxBPerBus*nBusJ+m_nNOB[nBusJ]]=nBusI;
		}
		m_nNOB[m_nSlackBus] += MaxBPerBus;

		//	�붯̬�ڵ��Ż�
		m_nNumBus=m_nMaxBusNumber;
		nProced=0;

		for (i=1; i<=m_nMaxBusNumber; i++)
		{
			if (m_nNOB[i] == 0)
			{
				m_nNOB[i]=1000;
				nProced++;
				m_nNumBus--;
			}
		}

		//Log(g_lpszDCNetworkLogFile, "MaxBus=%d BusNum=%d\n", m_nMaxBusNumber, m_nNumBus);
		//for (i=1; i<=m_nMaxBusNumber; i++)
		//{
		//	if (m_nNOB[i] > 0)
		//		Log(g_lpszDCNetworkLogFile, "OldBus=%d BranNum=%d\n", i, m_nNOB[i]);
		//}

		nCurBus=1;
		while (nProced < m_nMaxBusNumber)
		{
			//	�����ǰ��С�������ڵ�
			nMinBran=300;
			nMinBus=0;

			for (i=1; i<=m_nMaxBusNumber; i++)
			{
				if (m_nNOB[i] > 1)
				{
					if (m_nNOB[i] < nMinBran)
					{
						nMinBran=m_nNOB[i];
						nMinBus=i;
					}
				}
				else
				{
					nMinBran=m_nNOB[i];
					nMinBus=i;
				}
			}
// 			if (nMinBus == 69)
// 			{
// 				Log(g_lpszDCNetworkLogFile, "MinBus=%d BranNum=%d\n", nMinBus, nMinBran);
// 				for (i=1; i<=m_nNOB[nMinBus]; i++)
// 					Log(g_lpszDCNetworkLogFile, "      OppBus=%d\n", m_nNOCN[MaxBPerBus*nMinBus+i]);
// 
// 				Log(g_lpszDCNetworkLogFile, "Bus=19 BranNum=%d\n", m_nNOB[19]);
// 				if (m_nNOB[19] < MaxBPerBus)
// 				{
// 					for (i=1; i<=m_nNOB[19]; i++)
// 						Log(g_lpszDCNetworkLogFile, "      OppBus=%d\n", m_nNOCN[MaxBPerBus*19+i]);
// 				}
// 				Log(g_lpszDCNetworkLogFile, "\n");
// 			}

			//if (nMinBus == 0)
			//	ASSERT(FALSE);

			m_nBusOld[nCurBus]=nMinBus;					//	��¼��ĸ�ߵľɱ��
			m_nBusNew[nMinBus]=nCurBus;					//	����ǰĸ�ߣ�nMinBus������ĸ�߱�ţ�nCurBus��

			m_nNOB[nMinBus]=1000;						//	������ϱ�־

			if (nMinBran >= MaxBPerBus)
				goto _SlackOut;

			//	����ǰĸ�ߣ�nMinBus������֧·�Զ�ĸ�ߵ�֧·��Ϣ��ɾ��
			for (i=1; i<=nMinBran; i++)					//	������ǰĸ�ߣ�nMinBus���ĶԶ���Ϣ
			{
				nOppBus=m_nNOCN[MaxBPerBus*nMinBus+i];	//	��ǰĸ�ߣ�nMinBus���Զ�ĸ��
				nBranNum = m_nNOB[nOppBus];				//	��ǰĸ�ߣ�nMinBus���Զ�ĸ��������ֻ·��

				if (nBranNum >= MaxBPerBus)	//	��ǰĸ�ߣ�nMinBus���Զ�ĸ����ƽ��ڵ�
				{
					//Log(g_lpszDCNetworkLogFile, "OppBus=%d BranNum=%d\n", nOppBus, nBranNum);
					continue;
				}

				for (j=1; j<=nBranNum; j++)	//	��ǰĸ�ߣ�nMinBus���Զ�ĸ��������֧·��
				{
					if (m_nNOCN[MaxBPerBus*nOppBus+j] == nMinBus)
					{
						m_nNOCN[MaxBPerBus*nOppBus+j]=m_nNOCN[MaxBPerBus*nOppBus+nBranNum];	//	���Զ�ĸ�����һλĸ�߸�ֵ����ǰĸ��λ�ã�������·����һ�أ����Բ����нڵ��ظ���
						m_nNOB[nOppBus]=nBranNum-1;

						break;
					}
				}
			}

			for (i=1; i<=nMinBran-1; i++)
			{
				nOppBus=m_nNOCN[MaxBPerBus*nMinBus+i];			//	��ǰĸ�ߣ�nMinBus���ĶԶ�ĸ��
				if (nOppBus == 0)
					continue;

				int	nOBusBran=m_nNOB[nOppBus];					//	��ǰĸ�ߣ�nMinBus���Զ�ĸ�ߵ�����֧·��
				if (nOBusBran >= MaxBPerBus)					//	���������ƽ��ĸ��
				{
					//Log(g_lpszDCNetworkLogFile, "        OppBus=%d BranNum=%d\n", nOppBus, nOBusBran);
					continue;
				}
				for (j=i+1; j<=nMinBran; j++)					//	�Ե�ǰĸ�ߣ�nMinBus��������֧·���б���
				{
					int	nNextBus=m_nNOCN[MaxBPerBus*nMinBus+j];	//	��ǰĸ�ߣ�nMinBus���ĶԶ�ĸ��

					bFlaged=0;
					for (k=1; k<=nOBusBran; k++)				//	�Ե�ǰĸ�ߣ�nMinBus���Զ�ĸ�ߵ�����֧·���б���
					{
						if (m_nNOCN[MaxBPerBus*nOppBus+k] == nNextBus)	//	�Ե�ǰĸ�ߣ�nMinBus���Զ�ĸ�ߵĶԶ�ĸ��
						{
							bFlaged=1;
							break;
						}
					}
					if (!bFlaged)
					{
						nOBusBran=nOBusBran+1;
						m_nNOCN[MaxBPerBus*nOppBus+nOBusBran]=nNextBus;	//	�ڵ�ǰĸ�ߣ�nMinBus���ĶԶ�ĸ�ߵ�֧·���м���Զ�ĸ��
						m_nNOB[nOppBus]=nOBusBran;

						if (m_nNOB[nNextBus] < MaxBPerBus)					//	���������ƽ��ĸ��
						{
							m_nNOCN[MaxBPerBus*nNextBus+m_nNOB[nNextBus]+1]=nOppBus;
							m_nNOB[nNextBus]=m_nNOB[nNextBus]+1;
						}
					}
				}
			}

_SlackOut:	;
			nCurBus=nCurBus+1;
			nProced=nProced+1;
		}

		if (g_bWriteDebug)
		{
			Log(g_lpszDCNetworkLogFile, "�ڵ����Ż����豸������������ĸ��\n");
			//for (i=0; i<DCN_MaxBus; i++)
			for (i=0; i<(int)m_nNOB.size(); i++)
				m_nNOB[i]=0;

			for (i=0; i<m_nNumBra; i++)
			{
				nBusI=(int)m_fZArray[4*i+0];
				nBusJ=(int)fabs(m_fZArray[4*i+1]);
				if (nBusI == 0 || nBusJ == 0)
					continue;

				m_nNOB[nBusI]++;
				m_nNOB[nBusJ]++;
			}

			for (i=1; i<=m_nNumBus; i++)
			{
				if (m_nNOB[i] > 0 && m_nBusNew[i] <= 0)
					Log(g_lpszDCNetworkLogFile, "Bus=(%d/%d) New=%d Nob=%d\n", i, m_nMaxBusNumber, m_nBusNew[i], m_nNOB[i]);
			}
			Log(g_lpszDCNetworkLogFile, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		}

		//for (i=1; i<=m_nNumBus; i++)
		//{
		//	if (m_nBusNew[m_nBusOld[i]] != i)
		//		ASSERT(FALSE);
		//}

		DCNetwork_NewZ();
		for (i=0; i<m_nNumGen; i++)
			m_fGenArray[3*i]=m_nBusNew[(int)m_fGenArray[3*i]];
		for (i=0; i<m_nNumLoad; i++)
			m_fLoadArray[3*i]=m_nBusNew[(int)m_fLoadArray[3*i]];
	}

	//	�γɵ��ɾ��󣬵��ɾ���ʵ���������������鲿��ȡ��
	void CDCNetwork::DCNetwork_Admitance()
	{
		register int	i;
		int		nBusI, nBusJ;
		double	fXij;

		//for (i=0; i<DCN_MaxBus; i++)
		for (i=0; i<(int)m_fBII.size(); i++)
		{
			m_fBII[i]=0;
			m_nNDS[i]=0;
		}

		for (i=0; i<m_nNumBra; i++)
		{
			nBusI=(int)m_fZArray[4*i+0];
			nBusJ=abs((int)m_fZArray[4*i+1]);
			fXij=m_fZArray[4*i+2];

			m_fZArray[4*i+2]=-1.0/fXij;

			m_fBII[nBusI] += 1.0/fXij;
			m_fBII[nBusJ] += 1.0/fXij;

			m_nNDS[nBusI]=m_nNDS[nBusI]+1;		//	ÿ���ڵ������֧·����ÿ��֧·ȡС�ڵ�ŵ�
		}

		m_nNumDZ[1]=0;
		for (i=1; i<=m_nNumBus; i++)
			m_nNumDZ[i+1]=m_nNumDZ[i]+m_nNDS[i];	//	�����洢ÿ���ڵ������֧·����

		if (g_bWriteDebug)
		{
			Log(g_lpszDCNetworkLogFile, "�Ե���:\n");
			for (i=1; i<=m_nNumBus; i++)
			{
				Log(g_lpszDCNetworkLogFile, "B[%d][%d]=%f\n", i, i, m_fBII[i]);

				for (int j=m_nNumDZ[i]; j<=m_nNumDZ[i+1]-1; j++)
					Log(g_lpszDCNetworkLogFile, "    B[%d][%d]=%f\n", i, (int)fabs(m_fZArray[4*j+1]), m_fZArray[4*j+2]);
			}
		}
	}

	void CDCNetwork::DCNetwork_CholeskyDecompose()
	{
		register int	i;
		int		nRow, nCol;
		double	fBuf;
		int		bHasNZCol;

		//for (i=0; i<20*DCN_MaxBus; i++)
		for (i=0; i<(int)m_fMatrixU.size(); i++)
		{
			m_nMatrixIU[i]=0;
			m_fMatrixU[i]=0;
		}
		//for (i=0; i<DCN_MaxBus; i++)
		for (i=0; i<(int)m_fMatrixD.size(); i++)
		{
			m_fMatrixD[i]=0;
			m_nNumFD[i]=0;
		}
		m_nNumUTri=0;

		for (nRow=1; nRow<=m_nNumBus-1; nRow++)
		{
			for (i=nRow+1; i<=m_nNumBus-1; i++)
				m_fWkRow[i]=0;

			m_fWkRow[nRow]=m_fBII[nRow];
			for (i=m_nNumDZ[nRow]; i<=m_nNumDZ[nRow+1]-1; i++)	//	�����и�ֵ
			{
				nCol=(int)fabs(m_fZArray[4*i+1]);
				m_fWkRow[nCol] += m_fZArray[4*i+2];
			}

			if (g_bWriteDebug)
			{
				Log(g_lpszDCNetworkLogFile, "[%d]�еĹ�����\n", nRow);
				for (i=nRow; i<=m_nNumBus-1; i++)
				{
					if (fabs(m_fWkRow[i]) > FLT_MIN)
						Log(g_lpszDCNetworkLogFile, "    ��[%d]=%f\n", i, m_fWkRow[i]);
				}
			}

			m_nNumUTri=1;									//	�����Ƿ���Ԫ�ؼ�������������Ҫע������ȥ������
			if (nRow > 1)
			{
				for (i=1; i<=nRow-1; i++)					//	���ϵ�����ȥ1��nRow�з���Ԫ�ء���ȥ�Ļ����������öԽ�Ԫ��ȥ�ǶԽ�Ԫ
				{
					if (m_nNumFD[i] == 0) continue;

					bHasNZCol=0;
					for (nCol=1; nCol<=m_nNumFD[i]; nCol++)	//	m_nNumFD[i]:i�еķ���������Ϊ���洢�����ǣ����Խ���ת�ö�ȡ����ȥ������ɨ�裬������ȥ
					{
						if (m_nMatrixIU[m_nNumUTri] == nRow)
						{
							bHasNZCol=1;
							break;
						}
						m_nNumUTri++;
					}
					if (!bHasNZCol)
						continue;

					fBuf=m_fMatrixU[m_nNumUTri]/m_fMatrixD[i];
					while (nCol <= m_nNumFD[i])				//	��������
					{
						m_fWkRow[m_nMatrixIU[m_nNumUTri]] -= fBuf*m_fMatrixU[m_nNumUTri];
						m_nNumUTri++;
						nCol++;
					}
				}
			}

			m_fMatrixD[nRow]=1.0/m_fWkRow[nRow];
			m_nNumFD[nRow]=0;
			if (nRow < m_nNumBus-1)
			{
				for (nCol=nRow+1; nCol<=m_nNumBus-1; nCol++)
				{
					if (fabs(m_fWkRow[nCol]) > FLT_MIN)
					{
						m_nMatrixIU[m_nNumUTri]=nCol;
						m_fMatrixU[m_nNumUTri]=m_fWkRow[nCol]/m_fWkRow[nRow];
						m_nNumUTri++;
						m_nNumFD[nRow]++;
					}
				}
			}
		}

		m_nNumUTri=m_nNumUTri-1;
	}

	void	CDCNetwork::DCNetwork_Injection()
	{
		register int	i;
		for (i=0; i<=m_nNumBus; i++)
			m_fBusD[i]=0;
		for (i=0; i<m_nNumGen; i++)
			m_fBusD[(int)m_fGenArray[3*i]] += m_fGenArray[3*i+1];
		for (i=0; i<m_nNumLoad; i++)
			m_fBusD[(int)m_fLoadArray[3*i]] -= m_fLoadArray[3*i+1];
	}

	void	CDCNetwork::DCNetwork_BackwardSubstitution()
	{
		register int	i;
		int	nRow, nCol, nWorkRow;
		int	nIndex;

		//y(nRow)=b(nRow)-l(nRow, nWorkRow)*y(nWorkRow)	(k=1, i-1)
		for (nRow=2; nRow<=m_nNumBus-1; nRow++)
		{
			nIndex=0;
			for (nWorkRow=1; nWorkRow<=nRow-1; nWorkRow++)
			{
				for (i=1; i<=m_nNumFD[nWorkRow]; i++)
				{
					nCol=m_nMatrixIU[nIndex+i];
					if (nCol == nRow)
						m_fBusD[nRow] -= m_fBusD[nWorkRow]*m_fMatrixU[nIndex+i];
				}
				nIndex += m_nNumFD[nWorkRow];
			}
		}

		//x(nRow)=y(nRow)/d(nRow)-u(nRow, nWorkRow)*x(nWorkRow)	(k=nRow+1, nTotalRow)
		m_fBusD[m_nNumBus-1]=m_fBusD[m_nNumBus-1]*m_fMatrixD[m_nNumBus-1];
		for (nRow=m_nNumBus-2; nRow>=1; nRow--)
		{
			m_fBusD[nRow]=m_fBusD[nRow]*m_fMatrixD[nRow];
			nIndex=0;
			for (i=1; i<nRow; i++)
				nIndex += m_nNumFD[i];

			for (nWorkRow=m_nNumBus-1; nWorkRow>=nRow+1; nWorkRow--)
			{
				for (i=1; i<=m_nNumFD[nRow]; i++)
				{
					nCol=m_nMatrixIU[nIndex+i];
					if (nCol == nWorkRow)
						m_fBusD[nRow] = m_fBusD[nRow]-m_fMatrixU[nIndex+i]*m_fBusD[nWorkRow];
				}
			}
		}
	}


	void CDCNetwork::DCNetwork_InvEliminate()
	{
		register int	i;
		int		nRow, nCol;
		double	fBuf;
		int		bHasNZCol;

		//for (i=0; i<20*DCN_MaxBus; i++)
		for (i=0; i<(int)m_nMatrixIU.size(); i++)
		{
			m_nMatrixIU[i]=0;
			m_fMatrixU[i]=0;
		}
		//for (i=0; i<DCN_MaxBus; i++)
		for (i=0; i<(int)m_fMatrixD.size(); i++)
		{
			m_fMatrixD[i]=0;
			m_nNumFD[i]=0;
		}
		m_nNumUTri=0;

		for (nRow=1; nRow<=m_nNumBus-1; nRow++)
		{
			for (i=nRow+1; i<=m_nNumBus-1; i++)
				m_fWkRow[i]=0.0;

			m_fWkRow[nRow]=m_fBII[nRow];
			for (i=m_nNumDZ[nRow]; i<=m_nNumDZ[nRow+1]-1; i++)
			{
				nCol=(int)fabs(m_fZArray[4*i+1]);
				m_fWkRow[nCol] += m_fZArray[4*i+2];
			}

			if (g_bWriteDebug)
			{
				Log(g_lpszDCNetworkLogFile, "[%d]�еĹ�����\n", nRow);
				for (i=nRow; i<=m_nNumBus-1; i++)
				{
					if (fabs(m_fWkRow[i]) > FLT_MIN)
						Log(g_lpszDCNetworkLogFile, "    Col=%d %f\n", i, m_fWkRow[i]);
				}
			}

			m_nNumUTri=1;
			if (nRow > 1)
			{
				for (i=1; i<=nRow-1; i++)
				{
					if (m_nNumFD[i] == 0) continue;

					bHasNZCol=0;
					for (nCol=1; nCol<=m_nNumFD[i]; nCol++)
					{
						if (m_nMatrixIU[m_nNumUTri] == nRow)
						{
							bHasNZCol=1;
							break;
						}
						m_nNumUTri++;
					}
					if (!bHasNZCol)
						continue;

					fBuf=m_fMatrixU[m_nNumUTri]/m_fMatrixD[i];
					while (nCol <= m_nNumFD[i])
					{
						m_fWkRow[m_nMatrixIU[m_nNumUTri]] -= fBuf*m_fMatrixU[m_nNumUTri];
						m_nNumUTri++;
						nCol++;
					}
				}
			}

			if (fabs(m_fWkRow[nRow]) < FLT_MIN)
				m_fMatrixD[nRow]=FLT_MAX;
			else
				m_fMatrixD[nRow]=1.0/m_fWkRow[nRow];
			m_nNumFD[nRow]=0;
			if (nRow < m_nNumBus-1)
			{
				for (nCol=nRow+1; nCol<=m_nNumBus-1; nCol++)
				{
					if (fabs(m_fWkRow[nCol]) > FLT_MIN)
					{
						m_nMatrixIU[m_nNumUTri]=nCol;

						//if (fabs(m_fWkRow[nRow]) < FLT_MIN)
						//	m_fMatrixU[m_nNumUTri]=FLT_MAX;
						//else
						m_fMatrixU[m_nNumUTri]=m_fWkRow[nCol]/m_fWkRow[nRow];

						m_nNumUTri++;
						m_nNumFD[nRow]++;
					}
				}
			}
		}

		m_nNumUTri=m_nNumUTri-1;
	}

	void CDCNetwork::DCNetwork_InvImpedance()
	{
		register int	i;
		int		nRow, nCol, nIn;
		int		nFlag;
		double	fSum;
		int		LX2, LX3;

		m_fMatZ.resize(m_nNumBus*m_nNumBus+m_nNumBus+1);
		for (i=0; i<(int)m_fMatZ.size(); i++)
			m_fMatZ[i]=0;

		LX2=m_nNumUTri;
		LX3=m_nNumUTri;

		nRow=m_nNumBus-1;
		nCol=m_nNumBus-1;
		m_fMatZ[m_nNumBus*nRow+nCol]=(float)m_fMatrixD[m_nNumBus-1];
		if (g_bWriteDebug)	Log(g_lpszDCNetworkLogFile, "Row=%d Col=%d InvA=%g\n", nRow, nCol, m_fMatZ[m_nNumBus*nRow+nCol]);

		for (nRow=m_nNumBus-2; nRow>=1; nRow--)
		{
			nFlag=0;
			for (nCol=m_nNumBus-1; nCol>=nRow+1; nCol--)
			{
				if (nFlag == 1)
					m_nNumUTri=LX2;

				fSum=0.0;
				for (i=1; i<=m_nNumFD[nRow]; i++)
				{
					nIn=(m_nMatrixIU[m_nNumUTri] <= nCol) ? m_nNumBus*m_nMatrixIU[m_nNumUTri]+nCol : m_nNumBus*nCol+m_nMatrixIU[m_nNumUTri];
					fSum += m_fMatrixU[m_nNumUTri]*m_fMatZ[nIn];

					m_nNumUTri--;
					if (nCol == nRow+1)
						LX2--;
				}

				m_fMatZ[m_nNumBus*nRow+nCol]=m_fMatZ[m_nNumBus*nCol+nRow]=(float)-fSum;
				if (g_bWriteDebug)
					Log(g_lpszDCNetworkLogFile, "Row=%d Col=%d InvA=%g\n", nRow, nCol, m_fMatZ[m_nNumBus*nRow+nCol]);
				nFlag=1;
			}

			fSum=0.0;
			for (i=1; i<=m_nNumFD[nRow]; i++)
			{
				fSum += m_fMatrixU[LX3]*m_fMatZ[m_nNumBus*nRow+m_nMatrixIU[LX3]];
				LX3--;
			}

			m_fMatZ[m_nNumBus*nRow+nRow]=(float)(m_fMatrixD[nRow]-fSum);
			if (g_bWriteDebug)	Log(g_lpszDCNetworkLogFile, "Row=%d Col=%d InvA=%g\n", nRow, nRow, m_fMatZ[m_nNumBus*nRow+nRow]);
		}
	}

}

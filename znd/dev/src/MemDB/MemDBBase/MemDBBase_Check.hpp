#include "MemDBBase.h"
#include "MemDBBaseExtern.h"

namespace	MemDBBase
{
	template<typename T>	int	MDBCheckNull(T* pBlock, const int nTable, const char lpszRecArray[][MDB_CHARLEN_LONG])
	{
		register int	i;
		unsigned char	bFlag;
		char	szBuf[MDB_CHARLEN_LONG];

		for (i=0; i<pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyNum; i++)
		{
			memset(szBuf, 0, MDB_CHARLEN_LONG);
			strcpy(szBuf, lpszRecArray[pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyId[i]]);

			bFlag=0;
			for (int j=0; j<(int)strlen(szBuf); j++)
			{
				if (szBuf[j] != ' ' && szBuf[j] != '\t' && szBuf[j] != '\r' && szBuf[j] != '\n')
				{
					bFlag=1;
					break;
				}
			}
			if (!bFlag)
			{
				MDBLog("checkRecordNull���󣺱�[%s]��¼�еļ�¼[%s]��ֵΪ��\n", 
					pBlock->m_MDBSummary.sMDBTableArray[nTable].szDesp, 
					pBlock->m_MDBSummary.sMDBTableArray[pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyId[i]].szDesp);
				return 0;
			}
		}

		return 1;
	}

	template<typename T>	int	MDBCheckContainer(T* pBlock, const int nTable, const char lpszRecArray[][MDB_CHARLEN_LONG])
	{
		register int	i;
		int		nCheck, nRest, nFind;
		char	szRecArray[g_nConstMaxPrimaryKey][MDB_CHARLEN_LONG];

		for (nCheck=0; nCheck<g_nConstMaxTableFieldContainer; nCheck++)
		{
			if (pBlock->m_MDBSummary.sMDBContainerArray[nCheck].nTable < 0)
				continue;

			if (pBlock->m_MDBSummary.sMDBContainerArray[nCheck].nTable == nTable)
			{
				for (nRest=0; nRest<g_nConstMaxPrimaryKey; nRest++)
				{
					memset(szRecArray[nRest], 0, MDB_CHARLEN_LONG);
					if (pBlock->m_MDBSummary.sMDBContainerArray[nCheck].nPKFieldArray[nRest] >= 0)
						strcpy(szRecArray[nRest], lpszRecArray[pBlock->m_MDBSummary.sMDBContainerArray[nCheck].nPKFieldArray[nRest]]);
				}
				nFind=MDBFindRecordQuick(pBlock, pBlock->m_MDBSummary.sMDBContainerArray[nCheck].nPKTable, 0, pBlock->m_nRecordNum[pBlock->m_MDBSummary.sMDBContainerArray[nCheck].nPKTable]-1, szRecArray);
				if (nFind < 0)
				{
					std::string	strMesg;
					if (pBlock->m_MDBSummary.sMDBContainerArray[nCheck].bCheckType == MDBFieldCrit)
					{
						strMesg.clear();
						for (i=0; i<g_nConstMaxPrimaryKey; i++)
						{
							if (strlen(szRecArray[i]) > 0)
							{
								if (i != 0)
									strMesg.append(".");
								strMesg.append(szRecArray[i]);
							}
						}
						MDBLog("���󣺱� [%s] ��¼ %s �ڸ���[%s]�в�����\n", pBlock->m_MDBSummary.sMDBTableArray[nTable].szDesp, strMesg.c_str(), 
							pBlock->m_MDBSummary.sMDBTableArray[pBlock->m_MDBSummary.sMDBContainerArray[nCheck].nPKTable].szDesp);
						return 0;
					}
				}
			}
		}
		return 1;
	}

	template<typename T>	int	MDBCheckExist(T* pBlock, const int nTable, const int nCurItem, const char lpszRecArray[][MDB_CHARLEN_LONG])
	{
		if (pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyNum > 0)
		{
			register int	i, j;
			std::vector<std::string>	strRestArray;

			strRestArray.resize(pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyNum);
			for (i=pBlock->m_nRecordNum[nTable]-1; i>=0; i--)
			{
				if (nCurItem >= 0 && i == nCurItem)
					continue;

				for (j=0; j<pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyNum; j++)
					strRestArray[j]=lpszRecArray[pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyId[j]];
				if (MDBCompareRestrict(pBlock, nTable, i, strRestArray) == 0)
					return i;
			}
		}
		return -1;
	}

	template<typename T>	int	MDBCheckRecord(T* pBlock, const int nTable, const int nCurItem, const int bCheckNull, const int bCheckExist, const int bCheckContainer, const char lpszRecArray[][MDB_CHARLEN_LONG])
	{
		if (bCheckNull)
		{
			if (!MDBCheckNull(pBlock, nTable, lpszRecArray))
			{
				MDBLog("���ݼ�����[checkRecordNull]����[%s]��¼�д���Ϊ�չؼ���\n", pBlock->m_MDBSummary.sMDBTableArray[nTable].szDesp);
				return 0;
			}
		}
		if (bCheckExist)
		{
			if (MDBCheckExist(pBlock, nTable, nCurItem, lpszRecArray) >= 0)
			{
				MDBLog("���ݼ�����[checkRecordExist]����[%s]��¼�д�����ͬ�ļ�¼\n", pBlock->m_MDBSummary.sMDBTableArray[nTable].szDesp);
				return 0;
			}
		}

		if (bCheckContainer)
		{
			if (!MDBCheckContainer(pBlock, nTable, lpszRecArray))
			{
				MDBLog("���ݼ�����[checkRecordContainer]����[%s]\n", pBlock->m_MDBSummary.sMDBTableArray[nTable].szDesp);
				return 0;
			}
		}

		return 1;
	}

	template<typename T>	int MDBCheckTableContainer(T* pBlock)
	{
		register int	i;
		int		nTable, nRecord, nCheck, nRest, nFind;
		char	szRecArray[g_nConstMaxPrimaryKey][MDB_CHARLEN_LONG];

		for (nTable=0; nTable<pBlock->m_MDBSummary.nTableNum; nTable++)
		{
			for (nCheck=0; nCheck<g_nConstMaxTableFieldContainer; nCheck++)
			{
				if (pBlock->m_MDBSummary.sMDBContainerArray[nCheck].nTable < 0)
					continue;

				if (pBlock->m_MDBSummary.sMDBContainerArray[nCheck].nTable == nTable)
				{
					nRecord=0;
					while (nRecord<pBlock->m_nRecordNum[nTable])
					{
						for (nRest=0; nRest<g_nConstMaxPrimaryKey; nRest++)
						{
							memset(szRecArray[nRest], 0, MDB_CHARLEN_LONG);
							if (pBlock->m_MDBSummary.sMDBContainerArray[nCheck].nPKFieldArray[nRest] >= 0)
								MDBGetRecordValue(pBlock, nTable, pBlock->m_MDBSummary.sMDBContainerArray[nCheck].nPKFieldArray[nRest], nRecord, szRecArray[nRest]);
						}
						nFind=MDBFindRecordQuick(pBlock, pBlock->m_MDBSummary.sMDBContainerArray[nCheck].nPKTable, 0, pBlock->m_nRecordNum[pBlock->m_MDBSummary.sMDBContainerArray[nCheck].nPKTable]-1, szRecArray);
						if (nFind < 0)
						{
							if (pBlock->m_MDBSummary.sMDBContainerArray[nCheck].bCheckType == MDBFieldCrit)
							{
								MDBLog("���󣺱� [%s] ��¼", pBlock->m_MDBSummary.sMDBTableArray[nTable].szDesp);
								for (i=0; i<g_nConstMaxPrimaryKey; i++)
									MDBLog("[%s] ", szRecArray[i]);
								MDBLog("��������[%s]�в�����\n", pBlock->m_MDBSummary.sMDBTableArray[pBlock->m_MDBSummary.sMDBContainerArray[nCheck].nPKTable].szDesp);

								MDBRemoveRecord(pBlock, nTable, nRecord);
							}
							else
							{
								nRecord++;
							}
						}
						else
						{
							nRecord++;
						}
					}
				}
			}
		}
		return 1;
	}

	template<typename T>	int MDBCheckTableExist(T* pBlock, const int nCheckTable)
	{
		register int	i;
		int		nTable, nRecord;
		char	szRecRest[g_nConstMaxPrimaryKey][MDB_CHARLEN_LONG];

		for (nTable=0; nTable<pBlock->m_MDBSummary.nTableNum; nTable++)
		{
			if (nCheckTable >= 0 && nCheckTable != nTable)
				continue;
			if (pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyNum <= 0)
				continue;

			for (i=0; i<g_nConstMaxPrimaryKey; i++)
				memset(szRecRest[i], 0, MDB_CHARLEN_LONG);

			nRecord=0;
			while (nRecord < pBlock->m_nRecordNum[nTable])
			{
				for (i=0; i<pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyNum; i++)
					MDBGetRecordValue(pBlock, nTable, pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyId[i], nRecord, szRecRest[i]);

				if (MDBFindRecordQuick(pBlock, nTable, nRecord+1, pBlock->m_nRecordNum[nTable]-1, szRecRest) >= 0)
				{
					MDBLog("���󣺱�[%s] ��¼", pBlock->m_MDBSummary.sMDBTableArray[nTable].szDesp);
					for (i=0; i<pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyNum; i++)
						MDBLog("[%s] ", szRecRest[i]);
					MDBLog(" ������ͬ��¼\n");

					MDBRemoveRecord(pBlock, nTable, nRecord);
				}
				else
				{
					nRecord++;
				}
			}
		}
		return 1;
	}

	template<typename T>	int MDBCheckTableNull(T* pBlock, const int nCheckTable)
	{
		register int	i;
		int		nTable, nRecord;
		unsigned char	bNull;
		char	szRecRest[MDB_CHARLEN_LONG];

		for (nTable=0; nTable<pBlock->m_MDBSummary.nTableNum; nTable++)
		{
			if (nCheckTable >= 0 && nCheckTable != nTable)
				continue;

			nRecord=0;
			while (nRecord < pBlock->m_nRecordNum[nTable])
			{
				bNull=0;
				for (i=0; i<pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyNum; i++)
				{
					memset(szRecRest, 0, MDB_CHARLEN_LONG);
					MDBGetRecordValue(pBlock, nTable, pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyId[i], nRecord, szRecRest);
					if (strlen(szRecRest) <= 0)
					{
						bNull=1;
						break;
					}
				}

				if (bNull)
				{
					MDBLog("���󣺱�[%s]��¼ �ؼ���Ϊ��\n", pBlock->m_MDBSummary.sMDBTableArray[nTable].szDesp);
					for (i=0; i<pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyNum; i++)
					{
						memset(szRecRest, 0, MDB_CHARLEN_LONG);
						MDBGetRecordValue(pBlock, nTable, pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyId[i], nRecord, szRecRest);
						MDBLog("        �ؼ���[%d/%d] = %s\n", i+1, pBlock->m_MDBSummary.sMDBTableArray[nTable].sPrimaryKey.nPrimaryKeyNum, szRecRest);
					}
					MDBRemoveRecord(pBlock, nTable, nRecord);
				}
				else
				{
					nRecord++;
				}
			}
		}
		return 1;
	}
}

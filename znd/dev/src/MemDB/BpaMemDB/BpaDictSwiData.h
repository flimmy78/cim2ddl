#pragma once

#include	"BpaDict.h"
static	tagBpa_Dict	m_BpaSwiCaseDictArray[]=
{
	{	"BpaSwi_Case",		"CASE",	"CardKey",				1,	4,	/*A4,	*/	MDB_STRING,		4,		0,		0,		0,		0,	BpaSwiCategory_Con,	0,	/*������,	*/	},
	{	"BpaSwi_Case",		"CASE",	"PFCASE",				6,	15,	/*A10,	*/	MDB_STRING,		10,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*������ʽ��,	*/	},
	{	"BpaSwi_Case",		"CASE",	"ITSKP",				17,	17,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*д����ļ��Ĳ���,	*/	},
	{	"BpaSwi_Case",		"CASE",	"NOPRNT",				20,	20,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*NOPRNT, Ϊ1ʱ��ӡȫ����������,	*/	},
	{	"BpaSwi_Case",		"CASE",	"DSW",					22,	22,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*����ļ�ҡ��������ʾ����,	*/	},
	{	"BpaSwi_Case",		"CASE",	"IWSCC",				23,	23,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�����¼����ɿ������ɱ��,	*/	},
	{	"BpaSwi_Case",		"CASE",	"ZSW",					24,	24,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�����򿨽��м���־,	*/	},
	{	"BpaSwi_Case",		"CASE",	"X2FAC",				45,	49,	/*F5.5,	*/	MDB_FLOAT,		5,		5,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*���Գƹ���ʱ�����X2��Xdp֮��,	*/	},
	{	"BpaSwi_Case",		"CASE",	"XFACT",				50,	54,	/*F5.5,	*/	MDB_FLOAT,		5,		5,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�����Xdpp��Xdp֮��,	*/	},
	{	"BpaSwi_Case",		"CASE",	"TDODPS",				55,	59,	/*F5.5,	*/	MDB_FLOAT,		5,		5,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*��������Tdopp(��),	*/	},
	{	"BpaSwi_Case",		"CASE",	"TQODPS",				60,	64,	/*F5.5,	*/	MDB_FLOAT,		5,		5,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*��������Tdopp(��),	*/	},
	{	"BpaSwi_Case",		"CASE",	"TDODPH",				65,	69,	/*F5.5,	*/	MDB_FLOAT,		5,		5,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*͹��ˮ�ֻ���Tqopp(��),	*/	},
	{	"BpaSwi_Case",		"CASE",	"TQODPH",				70,	74,	/*F5.5,	*/	MDB_FLOAT,		5,		5,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*͹��ˮ�ֻ���Tdopp(��),	*/	},
	{	"BpaSwi_Case",		"CASE",	"CFACL2",				75,	80,	/*F6.5,	*/	MDB_FLOAT,		6,		5,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*���򸺺ɵ��ɱ�ôֵ,	*/	},
};

static	tagBpa_Dict	m_BpaSwiFFDictArray[]=
{
	{	"BpaSwi_FF",		"FF",	"CardKey",				1,	2,	/*A2,	*/	MDB_STRING,		2,		0,		0,		0,		0,	BpaSwiCategory_Con,	0,	/*������,	*/	},
	{	"BpaSwi_FF",		"FF",	"T",					5,	7,	/*F3.1,	*/	MDB_STRING,		3,		1,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*���㿪ʼʱ��(�ܲ�),	*/	},
	{	"BpaSwi_FF",		"FF",	"DT",					9,	11,	/*F3.1,	*/	MDB_FLOAT,		3,		1,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*���㲽��(�ܲ�),	*/	},
	{	"BpaSwi_FF",		"FF",	"ENDT",					13,	17,	/*F5.1,	*/	MDB_FLOAT,		5,		1,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*������ֹʱ��(�ܲ�),	*/	},
	{	"BpaSwi_FF",		"FF",	"DTC",					19,	21,	/*F3.1,	*/	MDB_FLOAT,		3,		1,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�µĲ���(�ܲ�),	*/	},
	{	"BpaSwi_FF",		"FF",	"ISTP",					23,	25,	/*I3,	*/	MDB_SHORT,		3,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*��ʼ�ı䲽����ʱ��(�ܲ�),	*/	},
	{	"BpaSwi_FF",		"FF",	"TOLI",					27,	31,	/*F5.5,	*/	MDB_FLOAT,		5,		5,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*���ε����������ٹ�����Χ,	*/	},
	{	"BpaSwi_FF",		"FF",	"ILIM",					33,	35,	/*I3,	*/	MDB_SHORT,		3,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*һ�������緽����΢�ַ���������������,	*/	},
	{	"BpaSwi_FF",		"FF",	"DELANG",				37,	40,	/*F4.4,	*/	MDB_FLOAT,		4,		4,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*���ε��������Ƕ���Χ,	*/	},
	{	"BpaSwi_FF",		"FF",	"DTDV",					42,	43,	/*F2.0,	*/	MDB_FLOAT,		2,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�������ֲ�����ֱ�����ֲ���֮��,	*/	},
	{	"BpaSwi_FF",		"FF",	"DMPMLT",				45,	47,	/*F3.3,	*/	MDB_FLOAT,		3,		3,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�������������,	*/	},
	{	"BpaSwi_FF",		"FF",	"FRQBSE",				55,	56,	/*F2.0,	*/	MDB_FLOAT,		2,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*ϵͳ��׼Ƶ��,	*/	},
	{	"BpaSwi_FF",		"FF",	"LOVTEX",				58,	58,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*1���޵͵�ѹ���, ������м��,	*/	},
	{	"BpaSwi_FF",		"FF",	"IMBLOK",				60,	60,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*��ﻬ��ﵽ1ʱ�Ĵ�����,	*/	},
	{	"BpaSwi_FF",		"FF",	"MFDEP",				64,	64,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*ҡ�ڷ����붨�ӷ�����Ƶ�ʵĹ�ϵ,	*/	},
	{	"BpaSwi_FF",		"FF",	"IGSLIM",				65,	65,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*����ʱ, ���е�����ģ��GS���������޷�,	*/	},
	{	"BpaSwi_FF",		"FF",	"LSOLQIT",				66,	66,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*1������������, ֹͣ����,	*/	},
	{	"BpaSwi_FF",		"FF",	"NOANGLIM",				68,	68,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*1�������Ƕ�����, ��������Ƕȳ���500��ͣ��,	*/	},
	{	"BpaSwi_FF",		"FF",	"INFBUS",				70,	70,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*1�����з������Ϊ�������Ե��,	*/	},
	{	"BpaSwi_FF",		"FF",	"NOPP",					71,	71,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*����ʱ, ���з�����������Ǵ���̬����,	*/	},
	{	"BpaSwi_FF",		"FF",	"NODQ",					72,	72,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*1�����з������Ϊ����ģ��,	*/	},
	{	"BpaSwi_FF",		"FF",	"NOSAT",				73,	73,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*����ʱ, ���еķ���������Ǳ���,	*/	},
	{	"BpaSwi_FF",		"FF",	"NOGV",					74,	74,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*1�����з�������Ƶ�����,	*/	},
	{	"BpaSwi_FF",		"FF",	"IEQPC",				75,	75,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*����ʱ, ���з�����Ĵ����㶨,	*/	},
	{	"BpaSwi_FF",		"FF",	"NOEX",					76,	76,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*1�����з�������Ƶ�ѹ��,	*/	},
	{	"BpaSwi_FF",		"FF",	"MFTOMG",				77,	77,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*����ʱ, ���еķ������MF����ת��ΪMG��,	*/	},
	{	"BpaSwi_FF",		"FF",	"NOSC",					78,	78,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*1���������и����ź�,	*/	},
	{	"BpaSwi_FF",		"FF",	"MGTOMF",				79,	79,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*����ʱ, ���еķ������MG����ת��ΪMF��,	*/	},
	{	"BpaSwi_FF",		"FF",	"NOLOAD",				80,	80,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*1�����и���Ϊ�㶨�迹,	*/	},
};

static	tagBpa_Dict	m_BpaSwiF1DictArray[]=
{
	{	"BpaSwi_F1",		"F1",	"CardKey",				1,	2,	/*A2,	*/	MDB_STRING,		2,		0,		0,		0,		0,	BpaSwiCategory_Con,	0,	/*������,	*/	},
	{	"BpaSwi_F1",		"F1",	"TBUSF",				5,	9,	/*F5.4,	*/	MDB_FLOAT,		5,		4,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*ĸ��Ƶ���˲���ȱʡʱ�䳣��,	*/	},
	{	"BpaSwi_F1",		"F1",	"TZERO",				12,	16,	/*F5.4,	*/	MDB_FLOAT,		5,		4,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*������·ȱʡʱ�䳣��,	*/	},
	{	"BpaSwi_F1",		"F1",	"IBLOD",				18,	18,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*ĸ�߸��ɿ���,	*/	},
	{	"BpaSwi_F1",		"F1",	"DMPALL",				20,	22,	/*F3.2,	*/	MDB_FLOAT,		3,		2,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*ȱʡ�������������,	*/	},
	{	"BpaSwi_F1",		"F1",	"IAMRTS",				26,	26,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�����ֵ��Ϊ0, ��CASE���е�XFACTҲ��Ϊ0, ���۷�����Ĵ���̬�����Ƿ��Ѿ���M��ָ��, ��Ҫ����ȱʡ�Ĳ��������㷢����Ĵ���̬����,	*/	},
	{	"BpaSwi_F1",		"F1",	"VCHGL",				28,	31,	/*F4.3,	*/	MDB_FLOAT,		4,		3,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*������ɽڵ��ѹ���ڸ�ֵ, �򸺺ɽ�ת��Ϊ���迹����, ȱʡֵΪ0.5pu,	*/	},
	{	"BpaSwi_F1",		"F1",	"SPTOSG",				33,	33,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*SP��Ϊ�Լ��ٹ�����Ϊ�����źŵ�PSS, SG��Ϊ�Ե�Ź���Ϊ�����źŵ�PSS,	*/	},
	{	"BpaSwi_F1",		"F1",	"GSIQU",				37,	41,	/*F5.5,	*/	MDB_FLOAT,		5,		5,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�����ֵ��Ϊ��, �������ļ�������GH����GS���������Ĵ�С��Ϊ��ֵ,	*/	},
	{	"BpaSwi_F1",		"F1",	"TITSKP",				43,	47,	/*F5.1,	*/	MDB_FLOAT,		5,		1,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�ӱ�ʱ�俪ʼ, ���ձ����е�48-49����д�Ĳ������д洢,	*/	},
	{	"BpaSwi_F1",		"F1",	"ITSKP1",				48,	49,	/*I2,	*/	MDB_SHORT,		2,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�ӱ���43-47�й涨��ʱ��TITSKP��ʼÿ��ITSKP1���洢һ������,	*/	},
	{	"BpaSwi_F1",		"F1",	"VRLIM",				51,	51,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*FM��FO��FP��FQ��FR��FS��FT��FG������ģ�͵ĵ�ѹ���޷�����Ϊ���޷�, ��Ϊ0��Ϊ���޷�,	*/	},
};

static	tagBpa_Dict	m_BpaSwiF0DictArray[]=
{
	{	"BpaSwi_F0",		"F0",	"CardKey",				1,	2,	/*A2,	*/	MDB_STRING,		2,		0,		0,		0,		0,	BpaSwiCategory_Con,	0,	/*������,	*/	},
	{	"BpaSwi_F0",		"F0",	"IG",					5,	5,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�ȶ�������ʾѡ��,	*/	},
	{	"BpaSwi_F0",		"F0",	"IA",					8,	8,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*����������Խ���ʾѡ��,	*/	},
	{	"BpaSwi_F0",		"F0",	"GenBusName1",			10,	17,	/*A8,	*/	MDB_STRING,		8,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�����1ĸ�ߵ�ѹ,	*/	},
	{	"BpaSwi_F0",		"F0",	"GenBusBase1",			18,	21,	/*F4.0,	*/	MDB_FLOAT,		4,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�����1ĸ�ߵ�ѹ(kV),	*/	},
	{	"BpaSwi_F0",		"F0",	"GenID1",				22,	22,	/*A1,	*/	MDB_CHAR,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�����1ʶ����,	*/	},
	{	"BpaSwi_F0",		"F0",	"GenBusName2",			24,	31,	/*A8,	*/	MDB_STRING,		8,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�����2ĸ�ߵ�ѹ,	*/	},
	{	"BpaSwi_F0",		"F0",	"GenBusBase2",			32,	35,	/*F4.0,	*/	MDB_FLOAT,		4,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�����2ĸ�ߵ�ѹ(kV),	*/	},
	{	"BpaSwi_F0",		"F0",	"GenID2",				36,	36,	/*A1,	*/	MDB_CHAR,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*�����2ʶ����,	*/	},
	{	"BpaSwi_F0",		"F0",	"AMAX",					38,	42,	/*F5.0,	*/	MDB_FLOAT,		5,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*��Խ�Y����������ֵ,	*/	},
	{	"BpaSwi_F0",		"F0",	"AMIN",					43,	47,	/*F5.0,	*/	MDB_FLOAT,		5,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*��Խ�Y���������Сֵ,	*/	},
	{	"BpaSwi_F0",		"F0",	"IV",					50,	50,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*ĸ�ߵ�ѹ��ʾѡ��,	*/	},
	{	"BpaSwi_F0",		"F0",	"VBUSNAME",				52,	59,	/*A8,	*/	MDB_STRING,		8,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*��ʾ��ѹ�仯���ߵ�ĸ��,	*/	},
	{	"BpaSwi_F0",		"F0",	"VBUSVOLT",				60,	63,	/*F4.0,	*/	MDB_FLOAT,		4,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*��ʾ��ѹ�仯���ߵ�ĸ�ߵ�ѹ(kV),	*/	},
	{	"BpaSwi_F0",		"F0",	"IF",					66,	66,	/*I1,	*/	MDB_BIT,		1,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*ĸ��Ƶ����ʾѡ��,	*/	},
	{	"BpaSwi_F0",		"F0",	"FBUSNAME",				68,	75,	/*A8,	*/	MDB_STRING,		8,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*��ʾƵ�ʱ仯���ߵ�ĸ��,	*/	},
	{	"BpaSwi_F0",		"F0",	"FBUSVOLT",				76,	79,	/*F4.0,	*/	MDB_FLOAT,		4,		0,		0,		0,		0,	BpaSwiCategory_Con,	1,	/*��ʾƵ�ʱ仯���ߵ�ĸ�ߵ�ѹ(kV),	*/	},
};

static	tagBpa_Dict	m_BpaSwiLNDictArray[]=
{
	{	"BpaSwi_GenLn",		"LN",	"CardKey",					1,	2,	/*A2,	*/	MDB_STRING,	2,	0,	0,		0,		0,		BpaSwiCategory_Dat,	0,	/*������,	*/					},
	{	"BpaSwi_GenLn",		"LN",	"ACBus_Name",				4,	11,	/*A8,	*/	MDB_STRING,	8,	0,	0,		0,		0,		BpaSwiCategory_Dat,	0,	/*��ֵ���������,	*/			},
	{	"BpaSwi_GenLn",		"LN",	"ACBus_kV",					12,	15,	/*F4.0,	*/	MDB_FLOAT,	4,	0,	0,		0,		0,		BpaSwiCategory_Dat,	0,	/*��ֵ�������ѹ,	*/			},
};


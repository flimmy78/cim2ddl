#pragma once

#include <vector>
#include <stack>
#include <string>
using namespace std;

namespace	NumericFormular
{
	enum	_EnumVariableType_
	{
		VariableType_None=0,
		VariableType_String,
		VariableType_Numeric,
		VariableType_ConstString,
		VariableType_ConstNumeric,
	};

	enum	_EnumSFormularOperatorType_
	{
		SFormular_OneVarOperator=0,
		SFormular_TwoVarOperator,
	};

	enum	_ENumSFormularVariableType_
	{
		SFormular_NormalVariable=0,
		SFormular_VectorVariable,
		SFormular_CaseVariable,
		SFormular_FuncVariable,
	};

	const	int		gnSQLCharLen=128;

	typedef	struct	_SFOperator_
	{
		std::string	strOperator;
		int	nLevel;
		int	nOperatorType;	//	һԪ��������Ƕ�Ԫ�����
	}	tagSFOperator;
	inline void InitializeSFOperator(tagSFOperator& opr)
	{
		opr.strOperator.clear();
		opr.nLevel=0;
		opr.nOperatorType=0;
	};

	typedef	struct	_SFVariable_
	{
		unsigned char	bNull;			//	�ַ���
		unsigned char	bConstDigit;	//	����
		unsigned char	bConstString;	//	�ַ���
		unsigned char	nVartype;		//	�������

		std::string		strVariable;
		int				nDataType;
		std::string		strValue;
		std::vector<std::string>	strValueArray;
	}	tagSFVariable;

	const int	FormularBracketsDepth=10;
	const int	gnCharSetLen=2;	//	�ַ�����Ӧ�ַ�����

	class	CNumericFormular
	{
	public:
		CNumericFormular(void);
		~CNumericFormular(void);

	public:
		std::string	m_strFormular;
		std::vector<tagSFOperator>	m_OperatorArray;
		std::vector<tagSFVariable>	m_VariableArray;

		int		SetFormular(const std::string strFormular);
		std::string	Calculate();

	private:
		int		CheckValidate(const std::string strExpression);
		int		ResolveOperator(void);
		int		ResolveVariable(void);
		std::string	Calculate(std::stack<tagSFOperator>& OperatorStack, std::stack<tagSFVariable>& VariableStack);
		std::string	Computing(const tagSFOperator& Operator, const tagSFVariable& Variable);
		std::string	Computing(const tagSFOperator& Operator, const tagSFVariable& VarL, const tagSFVariable& VarR);

	public:
		int		ResolveFormularType(void);
	private:
		void	ClearLog();
		void	Log(char* pformat, ...);
	};
}

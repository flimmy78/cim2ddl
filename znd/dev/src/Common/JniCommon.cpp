#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <jni.h>

//	��C�ַ�תΪJAVA�ַ���������OnLoad�е�SetLocalʮ�ֹؼ�
jstring CString2JString( JNIEnv * env, const char* lpszInStrCpp)
{
	jsize	nLen;
	wchar_t	wszBuf[65536];

	nLen=(jsize)mbstowcs(NULL, lpszInStrCpp, 0);
	mbstowcs(wszBuf, lpszInStrCpp, nLen);
	return env->NewString((jchar*)wszBuf, nLen);
}

//	��JAVA�ַ�תΪC�ַ���������OnLoad�е�SetLocalʮ�ֹؼ�
int JString2CString(JNIEnv * env, jstring jstrInJava, char* lpszOutStrCpp)
{
	unsigned char	bRet=0;

	const	jchar* lpszStr = env -> GetStringChars(jstrInJava, &bRet );
	wcstombs(lpszOutStrCpp, (wchar_t*)lpszStr, 65535);
	env -> ReleaseStringChars(jstrInJava, lpszStr );

	return 1;
}

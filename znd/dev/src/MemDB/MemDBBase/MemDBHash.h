#include "stdafx.h"

namespace	MemDBBase
{
	//�����ַ�����hash�㷨
	inline unsigned int Hash_RS(const char* str)
	{
		unsigned int b = 378551 ;
		unsigned int a = 63689 ;
		unsigned int hash = 0 ;

		while ( * str)
		{
			hash = hash * a + ( * str ++ );
			a *= b;
		}

		return (hash & 0x7FFFFFFF );
	}

	// JS Hash Function
	inline unsigned int Hash_JS(const char* str)
	{
		unsigned int hash = 1315423911 ;

		while ( * str)
		{
			hash ^= ((hash << 5 ) + ( * str ++ ) + (hash >> 2 ));
		}

		return (hash & 0x7FFFFFFF );
	}

	// P. J. Weinberger Hash Function
	inline unsigned int Hash_PJW(const char* str)
	{
		unsigned int BitsInUnignedInt = (unsigned int )( sizeof (unsigned int ) *  8 );
		unsigned int ThreeQuarters = (unsigned int )((BitsInUnignedInt * 3 ) / 4 );
		unsigned int OneEighth = (unsigned int )(BitsInUnignedInt / 8 );

		unsigned int HighBits = (unsigned int )( 0xFFFFFFFF ) << (BitsInUnignedInt - OneEighth);
		unsigned int hash = 0 ;
		unsigned int test = 0 ;

		while ( * str)
		{
			hash = (hash << OneEighth) + ( * str ++ );
			if ((test = hash & HighBits) != 0 )
			{
				hash = ((hash ^ (test >> ThreeQuarters)) & ( ~ HighBits));
			}
		}

		return (hash & 0x7FFFFFFF );
	}

	// ELF Hash Function
	inline unsigned int Hash_ELF(const char* str)
	{
		unsigned int hash = 0 ;
		unsigned int x = 0 ;

		while ( * str)
		{
			hash = (hash << 4 ) + ( * str ++ );
			if ((x = hash & 0xF0000000L ) != 0 )
			{
				hash ^= (x >> 24 );
				hash &= ~ x;
			}
		}

		return (hash & 0x7FFFFFFF );
	}

	// BKDR Hash Function
	inline unsigned int Hash_BKDR(const char* str)
	{
		unsigned int seed = 131 ; // 31 131 1313 13131 131313 etc..
		unsigned int hash = 0 ;

		while ( * str)
		{
			hash = hash * seed + ( * str ++ );
		}

		return (hash & 0x7FFFFFFF );
	}

	// SDBM Hash Function
	inline unsigned int Hash_SDBM(const char* str)
	{
		unsigned int hash = 0 ;

		while ( * str)
		{
			hash = ( * str ++ ) + (hash << 6 ) + (hash << 16 ) - hash;
		}

		return (hash & 0x7FFFFFFF );
	}

	// DJB Hash Function
	inline unsigned int Hash_DJB(const char* str)
	{
		unsigned int hash = 5381 ;

		while ( * str)
		{
			hash += (hash << 5 ) + ( * str ++ );
		}

		return (hash & 0x7FFFFFFF );
	}

	// AP Hash Function
	inline unsigned int Hash_AP(const char* str)
	{
		unsigned int hash = 0 ;
		int i;

		for (i = 0 ; * str; i ++ )
		{
			if ((i & 1 ) == 0 )
			{
				hash ^= ((hash << 7 ) ^ ( * str ++ ) ^ (hash >> 3 ));
			}
			else
			{
				hash ^= ( ~ ((hash << 11 ) ^ ( * str ++ ) ^ (hash >> 5 )));
			}
		}

		return (hash & 0x7FFFFFFF );
	}
}
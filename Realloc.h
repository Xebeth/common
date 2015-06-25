/**************************************************************************
	created		:	2013-04-25
	filename	: 	Realloc.h
	author		:	Xebeth`
	copyright	:	North Edge (2013)
	purpose		:	
**************************************************************************/
#ifndef __REALLOC_H__
#define __REALLOC_H__

namespace Buffer
{
	template <typename T> bool Realloc(T** pBuffer_in_out, 
									   ULONG_PTR AllocCount_in,
									   bool ClearBuffer_in = false)
	{
		if (pBuffer_in_out != NULL)
		{
			ULONG_PTR AllocSize = AllocCount_in * sizeof(T);
			void *pRealloc = realloc(*pBuffer_in_out, AllocSize);

			if (pRealloc != NULL)
			{
				if (ClearBuffer_in)
					memset(pRealloc, 0, AllocSize);

				*pBuffer_in_out = reinterpret_cast<T*>(pRealloc);

				return true;
			}
		}

		return false;
	}
}

#endif//__REALLOC_H__

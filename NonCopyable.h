/**************************************************************************
	created		:	2011-05-29
	filename	: 	NonCopyable.h
	author		:	xebeth`
	copyright	:	North Edge (2011)
	purpose		:	Non copyable idiom
**************************************************************************/
#ifndef __NON_COPYABLE_H__
#define __NON_COPYABLE_H__

namespace NonCopyable_
{
	/*! \brief Non copyable idiom */
	class NonCopyable
	{
	protected:
		NonCopyable() {}
		virtual ~NonCopyable() {}
	private:
		NonCopyable(const NonCopyable&);
		const NonCopyable& operator=(const NonCopyable&);
	};
}

typedef NonCopyable_::NonCopyable NonCopyable;

#endif//__NON_COPYABLE_H__
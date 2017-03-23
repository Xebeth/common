#pragma once

namespace
{
	//
	// list of types to act as the primary key. The primary key dicatates the
	// format of the secondary key.
	//
	enum class eUnionKey
	{
		first,
		second
	};

	// Base class for the multi-key.
	class UnionKeyTag
	{
	public:
		explicit UnionKeyTag(eUnionKey Type_in)
			: m_Tag(Type_in) {};

		virtual ~UnionKeyTag(void) {};

		eUnionKey Tag() const
		{
			return m_Tag;
		}

		virtual bool operator< (const UnionKeyTag &b) const
		{
			return (UnionKeyTag::operator<(&b));
		}

		virtual bool operator< (const UnionKeyTag *p) const
		{
			return (m_Tag < p->m_Tag);
		}

		virtual bool operator== (const UnionKeyTag &b) const
		{
			return (UnionKeyTag::operator==(&b));
		}

		virtual bool operator== (const UnionKeyTag *p) const
		{
			return (m_Tag == p->m_Tag);
		}

	protected:
		eUnionKey m_Tag;   // the primary key
	};

	// template    UnionKeyType
	//
	// ValueType    -  select the enumerated value to use for m_Tag's value
	//
	// KeyType         -  select the class to use for the second key. For built
	//                      in types they use their default < and == operators,
	//                      If a private custom type is specified then it must
	//                      have its own < and == operators specified
	//
	template <enum class eUnionKey ValueType, typename KeyType>
	class UnionKeyType : public UnionKeyTag
	{
		using type = UnionKeyType<ValueType, KeyType>;
		using Type1Key = UnionKeyType<eUnionKey::first, KeyType>;
		using Type2Key = UnionKeyType<eUnionKey::second, KeyType>;
	public:
		explicit UnionKeyType(KeyType Key_in)
			: UnionKeyTag(ValueType), m_Key(Key_in) {};

		explicit UnionKeyType(const type &Other_in)
			: UnionKeyTag(Other_in.m_Tag), m_Key(Other_in.m_Key) {}

		explicit UnionKeyType(const type *pOther_in)
			: UnionKeyTag(pOther_in->m_Tag), m_Key(pOther_in->m_Key) {}

		virtual ~UnionKeyType(void) {};

		//
		// operator <
		//
		bool operator< (const UnionKeyTag &b) const override
		{
			return (operator<(&b));
		}

		bool operator< (const UnionKeyTag *p) const override
		{
			// if the primary key is less then it's less, don't check 2ndary
			if (UnionKeyTag::operator<(p))
			{
				return (true);
			}

			// if not less then it's >=, check if equal, if it's not equal then it
			// must be greater
			if (!(UnionKeyTag::operator==(p)))
			{
				return (false);
			}

			if (p->Tag() == eUnionKey::first)
			{
				return (m_Key < static_cast<const Type1Key*>(p)->Key());
			}
			
			if (p->Tag() == eUnionKey::second)
			{
				return (m_Key < static_cast<const Type2Key*>(p)->Key());
			}
			
			return true;
		}

		//
		// operator ==
		//
		bool operator== (const UnionKeyTag &b) const override
		{
			return(operator==(&b));
		}

		bool operator== (const UnionKeyTag *p) const override
		{
			// if the primary key isn't equal, then we're not equal
			if (!(UnionKeyTag::operator==(p)))
			{
				return (false);
			}
						
			if (p->Tag() == eUnionKey::first)
			{
				auto other_p = static_cast<const Type1Key*>(p);

				return std::equal_to<KeyType>()(m_Key, other_p->Key());
			}
			if (p->Tag() == eUnionKey::first)
			{
				auto other_p = static_cast<const Type1Key*>(p);

				if (other_p != nullptr)
					return std::equal_to<KeyType>()(m_Key, other_p->Key());
			}

			return true;
		}

		const KeyType& Key() const
		{
			return m_Key;
		}

	protected:
		KeyType    m_Key;    // The secondary key.
	};
}

template<typename T, typename U> class UnionKey
{
	using Type1Key = UnionKeyType<eUnionKey::first, T>;
	using Type2Key = UnionKeyType<eUnionKey::second, U>;

public:
	// ReSharper disable once CppNonExplicitConvertingConstructor
	UnionKey(const T& Value_in)
		: m_pTag(new Type1Key(Value_in)) {};
	// ReSharper disable once CppNonExplicitConvertingConstructor
	UnionKey(const U& Value_in)
		: m_pTag(new Type2Key(Value_in)) {};

	explicit UnionKey(const UnionKey &Other_in)
	{
		operator=(Other_in);
	}

	explicit UnionKey(const UnionKey &&Other_in)
		: m_pTag(nullptr)
	{
		operator=(Other_in);
	}

	UnionKey& operator=(const UnionKey &Other_in)
	{
		if (Other_in.m_pTag->Tag() == eUnionKey::first)
		{
			m_pTag = new Type1Key(static_cast<Type1Key*>(Other_in.m_pTag));
		}
		else if (Other_in.m_pTag->Tag() == eUnionKey::second)
		{
			m_pTag = new Type2Key(static_cast<Type2Key*>(Other_in.m_pTag));
		}

		return *this;
	}

	UnionKey& operator=(const UnionKey &&Other_in)
	{
		std::swap(m_pTag, Other_in.m_pTag);

		return *this;
	}

	operator T() const
	{
		return static_cast<Type1Key*>(m_pTag)->Key();
	}

	operator U() const
	{
		return static_cast<Type2Key*>(m_pTag)->Key();
	}

	~UnionKey(void)
	{
		if (m_pTag != nullptr)
		{
			delete m_pTag;
			m_pTag = nullptr;
		}
	};

	bool operator< (const UnionKey &mk) const 
	{
		return (m_pTag->operator<(mk.m_pTag));
	}

	bool operator== (const UnionKey &mk) const 
	{
		return (m_pTag->operator==(mk.m_pTag));
	}

protected:
	UnionKeyTag *m_pTag;
};
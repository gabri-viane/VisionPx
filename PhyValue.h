#pragma once
#ifndef PHY_VALUE_DECL
#define PHY_VALUE_DECL

#include "pch.h"
#include "PhyUnit.h"

namespace Phy {
	template<typename T>
	class VISIONPX_API Value
	{
	private:
		UnitComponent* unit;
		T* value;

		Value();
		Value(const Value& data);
		Value(const T& data,const UnitComponent* comp);

	public:
		~Value();

		Value& operator+(Value& data) const;
		Value& operator/(Value& data) const;
		Value& operator-(Value& data) const;
		Value& operator*(Value& data) const;
		Value& operator*(T& data) const;
		Value& operator!() const;
		void operator++();
		void operator--();
		operator float();
		operator double();
		operator int();
		operator long int();

	};
}

#endif
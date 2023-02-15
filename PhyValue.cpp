#include "pch.h"
#include "PhyValue.h"

template<typename T>
Phy::Value<T>::Value() {
	this->unit = Phy::UnitComponentFactory::_1;
}

template<typename T>
Phy::Value<T>::Value(const Value& data) {
	this->unit = data->unit;
	this->value= data->value;
}

template<typename T>
Phy::Value<T>::Value(const T& data,const UnitComponent* comp) {
	this->unit = comp;
	this->value = *data;
}

template<typename T>
Phy::Value<T>::~Value()
{
	
}

template<typename T>
Phy::Value<T>& Phy::Value<T>::operator+(Phy::Value<T>& data) const
{
	if (*this->unit == *data.unit) {
		return new Value<T>(*this->value + *data.value, this->unit);
	}
	throw "Unit mismatch: can't sum the two values";
}

template<typename T>
Phy::Value<T>& Phy::Value<T>::operator-(Phy::Value<T>& data) const
{
	if (*this->unit == *data.unit) {
		return new Value<T>(*this->value - *data.value, this->unit);
	}
	throw "Unit mismatch: can't subtract the two values";
}

template<typename T>
Phy::Value<T>& Phy::Value<T>::operator*(Phy::Value<T>& data) const
{
	return new Value<T>(*this->value * *data.value, *this->unit * *data.unit);
}



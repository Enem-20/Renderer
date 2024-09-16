#pragma once

#ifndef ICAPTURER_H
#define ICAPTURER_H

#include "Interface.h"

#include <functional>

template<class Type, class CurrentClass>
class ICapturer {
public:
	ICapturer(Interface<Type>& captureObject, CurrentClass& placeToCapture);
	std::function<Type&()> getOriginal;
};


template<class Type, class CurrentClass>
ICapturer<Type, CurrentClass>::ICapturer(Interface<Type>& captureObject, CurrentClass& placeToCapture) {
	getOriginal = std::bind(&Interface<Type>::getObject, &placeToCapture);
}

#endif
#pragma once

#include <functional>
#include <memory>
#include <optional>

template<class ReturnType>
class Interface {
protected:
	Interface() = default;
	static ReturnType& getObject();
private:
	static ReturnType Object;
};

template<class ReturnType>
ReturnType& Interface<ReturnType>::getObject() {
	return Interface<ReturnType>::Object;
}
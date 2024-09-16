#pragma once

#ifndef DEPENDENCY_OWNER_H
#define DEPENDENCY_OWNER_H

class DependencyOwner {
public:
	virtual void resetDependencies() = 0;
};

#endif
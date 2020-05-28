#pragma once


#include "LXSystem.h"


namespace ObjectFactory
{

	LXObject* GetNewObject(int hash, int objectID, bool trackInObjectManager=true);

}
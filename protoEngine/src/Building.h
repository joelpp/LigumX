#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "Logging.h"

class Model;
class Way;

class Building
{
public:

	Model* m_Model;
	Way* m_Way;

	Building(Way* way);

	bool GenerateModel();


private:
	REGISTERCLASS(Building);
};


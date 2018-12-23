#pragma once

/*
	GLOBAL CONFIG DEFINES
*/

#define LX_IMGUI_ENABLED

/*
	GLOBAL EXTERNAL INCLUDES
*/
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <unordered_map>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "glfw\glfw3.h"

#include "tinyxml2\tinyxml2.h"

/*
	GLOBAL INTERNAL INCLUDES
*/

#include "LXLimits.h"
#include "LXTypeDefinition.h"
#include "LXError.h"
#include "Logging.h"
#include "ImguiHelpers.h"

#include "StringUtils.h"
#include "property.h"
#include "LXObject.h"

#include "serializer.h"
#include "ObjectManager.h"
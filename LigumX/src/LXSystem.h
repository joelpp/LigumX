#pragma once

/*
	GLOBAL CONFIG DEFINES
*/

#define LX_IMGUI_ENABLED

typedef signed __int64 lxInt64;
typedef signed __int32 lxInt32;

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
#include <cstdarg>
#include <chrono>
#include <typeinfo>
#include <stdlib.h>
#include <map>
#include <functional>


#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

typedef glm::highp_ivec2 lx2I64;
typedef glm::vec2 lx2F32;
typedef glm::vec3 lx3F32;

#define LXMap std::unordered_map
#define LXVector std::vector

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
#include "VectorUtils.h"
#include "property.h"
#include "LXObject.h"

#include "ObjectManager.h"

#include "glfw\glfw3.h"

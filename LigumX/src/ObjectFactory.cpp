#include "ObjectFactory.h"

#include "LXAllClassInclude.h"

LXObject* ObjectFactory::GetNewObject(int hash, int objectID, bool trackInObjectManager)
{

#define LX_CLASS(c, p) case c::ClassID: \
{\
    c* obj = new c();\
	obj->SetObjectID(objectID);\
	obj->Serialize(false);\
	if (trackInObjectManager) g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);\
	return (LXObject*)obj;\
}
#define LX_ENUM(c)

	switch (hash)
	{
#include "LXClassList.h"
#undef LX_CLASS
#undef LX_ENUM
	default:
		break;
	}

	return nullptr;
}

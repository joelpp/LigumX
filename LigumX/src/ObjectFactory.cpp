#include "ObjectFactory.h"

#include "BoundingBoxComponent.h"
#include "TestComponentA.h"
#include "TestComponentB.h"
#include "Way.h"

LXObject* ObjectFactory::GetNewObject(int hash, int objectID)
{
	switch (hash)
	{
	case BoundingBoxComponent::ClassID:
	{
		BoundingBoxComponent* obj = new BoundingBoxComponent();
		obj->SetObjectID(objectID);
		obj->Serialize(false);
		return (LXObject*)obj;
		break;
	}
	case TestComponentA::ClassID:
	{
		TestComponentA* obj = new TestComponentA();
		obj->SetObjectID(objectID);
		obj->Serialize(false);
		return (LXObject*)obj;
		break;
	}
	case TestComponentB::ClassID:
	{
		TestComponentB* obj = new TestComponentB();
		obj->SetObjectID(objectID);
		obj->Serialize(false);
		return (LXObject*)obj;
		break;
	}

	default:break;
	}

	return nullptr;
}
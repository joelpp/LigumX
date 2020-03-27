#include "ObjectFactory.h"

#include "BoundingBoxComponent.h"

LXObject* ObjectFactory::GetNewObject(int hash)
{
	switch (hash)
	{
	case BoundingBoxComponent::ClassID:
	{
		return new BoundingBoxComponent();
		break;
	}

	default:break;
	}

	return nullptr;
}
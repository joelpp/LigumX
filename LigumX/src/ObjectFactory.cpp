#include "ObjectFactory.h"

#include "LXAllClassInclude.h"

LXObject* ObjectFactory::GetNewObject(int hash, int objectID)
{

#if 1
#define LX_CLASS(c) case c::ClassID: \
{\
    c* obj = new c();\
	obj->SetObjectID(objectID);\
	obj->Serialize(false);\
	g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);\
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

#else

    switch (hash)
    {
    case AABB::ClassID:
    {
    AABB* obj = new AABB();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case BoundingBoxComponent::ClassID:
    {
    BoundingBoxComponent* obj = new BoundingBoxComponent();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Camera::ClassID:
    {
    Camera* obj = new Camera();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Component::ClassID:
    {
    Component* obj = new Component();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case CPUBuffers::ClassID:
    {
    CPUBuffers* obj = new CPUBuffers();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case CullingOptions::ClassID:
    {
    CullingOptions* obj = new CullingOptions();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case CurlRequest::ClassID:
    {
    CurlRequest* obj = new CurlRequest();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case DebugRenderer::ClassID:
    {
    DebugRenderer* obj = new DebugRenderer();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case DefaultTextureHolder::ClassID:
    {
    DefaultTextureHolder* obj = new DefaultTextureHolder();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case DisplayOptions::ClassID:
    {
    DisplayOptions* obj = new DisplayOptions();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Editor::ClassID:
    {
    Editor* obj = new Editor();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case EditorOptions::ClassID:
    {
    EditorOptions* obj = new EditorOptions();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case EditorTool::ClassID:
    {
    EditorTool* obj = new EditorTool();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case EngineSettings::ClassID:
    {
    EngineSettings* obj = new EngineSettings();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case EngineStats::ClassID:
    {
    EngineStats* obj = new EngineStats();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Entity::ClassID:
    {
    Entity* obj = new Entity();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Framebuffer::ClassID:
    {
    Framebuffer* obj = new Framebuffer();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case GUI::ClassID:
    {
    GUI* obj = new GUI();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Heightfield::ClassID:
    {
    Heightfield* obj = new Heightfield();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case InputHandler::ClassID:
    {
    InputHandler* obj = new InputHandler();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case LXObject::ClassID:
    {
    LXObject* obj = new LXObject();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case MainWindow::ClassID:
    {
    MainWindow* obj = new MainWindow();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Material::ClassID:
    {
    Material* obj = new Material();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Mesh::ClassID:
    {
    Mesh* obj = new Mesh();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Model::ClassID:
    {
    Model* obj = new Model();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Node::ClassID:
    {
    Node* obj = new Node();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case OSMAttribute::ClassID:
    {
    OSMAttribute* obj = new OSMAttribute();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case OSMDataProcessor::ClassID:
    {
    OSMDataProcessor* obj = new OSMDataProcessor();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case OSMDataProcessorSettings::ClassID:
    {
    OSMDataProcessorSettings* obj = new OSMDataProcessorSettings();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case OSMElementComponent::ClassID:
    {
    OSMElementComponent* obj = new OSMElementComponent();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case OSMElementTypeData::ClassID:
    {
    OSMElementTypeData* obj = new OSMElementTypeData();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case OSMElementTypeDataStore::ClassID:
    {
    OSMElementTypeDataStore* obj = new OSMElementTypeDataStore();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case OSMTool::ClassID:
    {
    OSMTool* obj = new OSMTool();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case PickingTool::ClassID:
    {
    PickingTool* obj = new PickingTool();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case PostEffects::ClassID:
    {
    PostEffects* obj = new PostEffects();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case RenderDataManager::ClassID:
    {
    RenderDataManager* obj = new RenderDataManager();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Renderer::ClassID:
    {
    Renderer* obj = new Renderer();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case RenderingStats::ClassID:
    {
    RenderingStats* obj = new RenderingStats();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Sector::ClassID:
    {
    Sector* obj = new Sector();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case SectorGraphicalData::ClassID:
    {
    SectorGraphicalData* obj = new SectorGraphicalData();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case SectorTool::ClassID:
    {
    SectorTool* obj = new SectorTool();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Serializer2::ClassID:
    {
    Serializer2* obj = new Serializer2();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case SerializerInputVariable::ClassID:
    {
    SerializerInputVariable* obj = new SerializerInputVariable();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case SunLight::ClassID:
    {
    SunLight* obj = new SunLight();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case TerrainTool::ClassID:
    {
    TerrainTool* obj = new TerrainTool();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case TestComponentA::ClassID:
    {
    TestComponentA* obj = new TestComponentA();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case TestComponentB::ClassID:
    {
    TestComponentB* obj = new TestComponentB();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Texture::ClassID:
    {
    Texture* obj = new Texture();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case Way::ClassID:
    {
    Way* obj = new Way();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    case World::ClassID:
    {
    World* obj = new World();
    obj->SetObjectID(objectID);
    obj->Serialize(false);
    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);
    return (LXObject*)obj;
	}
    default:break;
    }
#endif
	return nullptr;
}

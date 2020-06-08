#include "DebugRenderer.h"

#pragma region  CLASS_SOURCE DebugRenderer
DebugRenderer* g_DebugRenderer;

#include "DebugRenderer.h"
#include "serializer.h"
const ClassPropertyData DebugRenderer::g_Properties[] = 
{
{ "Enabled", PIDX_Enabled, offsetof(DebugRenderer, m_Enabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void DebugRenderer::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_Enabled], m_Enabled);
}
bool DebugRenderer::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool DebugRenderer::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Enabled], &m_Enabled  );
	return true;
}
void DebugRenderer::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	DebugRenderer* other = (DebugRenderer*) otherObj;
	other->SetEnabled(m_Enabled);
}
const char* DebugRenderer::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE DebugRenderer

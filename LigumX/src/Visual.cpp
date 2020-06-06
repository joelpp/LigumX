#include "Visual.h"

#pragma region  CLASS_SOURCE Visual

#include "Visual.h"
#include "serializer.h"
#include "Model.h"
const ClassPropertyData Visual::g_Properties[] = 
{
{ "Models", PIDX_Models, offsetof(Visual, m_Models), 0, LXType_stdvector, sizeof(std::vector<Model*>), LXType_stdvector, false, LXType_Model, true, 0, 0, 0, 0,}, 
};
void Visual::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeVector(g_Properties[PIDX_Models], m_Models);
}
bool Visual::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Visual::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty3(this, g_Properties[PIDX_Models], m_Models  );
	return true;
}
void Visual::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	Visual* other = (Visual*) otherObj;
	other->SetModels(m_Models);
}
const char* Visual::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE Visual

#include "RenderdocManager.h"
#include "LXSystem.h"
#include "C:\Program Files\RenderDoc\renderdoc_app.h"

RENDERDOC_API_1_1_2 *rdoc_api = NULL;

void RenderdocManager::LoadRenderdoc()
{
	HMODULE rdoc_dll = LoadLibraryA("C:\\Program Files\\RenderDoc\\renderdoc.dll");
	
	// At init, on windows
	if (HMODULE mod = GetModuleHandleA("renderdoc.dll"))
	{
		pRENDERDOC_GetAPI RENDERDOC_GetAPI =
			(pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
		lxInt32 ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void **)&rdoc_api);
		lxAssert(ret == 1);

		m_IsEnabled = true;
	}
	else
	{
		lxLogMessage("Could not find renderdoc.dll! Renderdoc API will be unavailable.");
	}

}

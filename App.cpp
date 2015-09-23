#include "pch.h"
#include "RXTitleBar.h"
#include "RXCoreXaml.h"

int __cdecl main(::Platform::Array<::Platform::String^>^ args)
{
	Application::Start(ref new ApplicationInitializationCallback([](ApplicationInitializationCallbackParams^ p)
	{
		auto app = ref new CRXCoreXaml();
	}));
}

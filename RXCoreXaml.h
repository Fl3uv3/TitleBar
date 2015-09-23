//*** Macro definition ******************************************************
#ifndef _RELENTLESS_X_COREXAML_H__
	#define _RELENTLESS_X_COREXAML_H__

#pragma once

//*** Class definition **************************************************
ref class CRXCoreXaml sealed : public Windows::UI::Xaml::Application
{
	public:
		CRXCoreXaml();
	
	protected:
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;
		
	private:
		~CRXCoreXaml();
		CRXTitleBar^ m_pTitleBar;
};

//*** Constructor *******************************************************
CRXCoreXaml::CRXCoreXaml()
{
}

CRXCoreXaml::~CRXCoreXaml()
{
	if (m_pTitleBar != nullptr) m_pTitleBar->ReleaseTitleBar();	
	m_pTitleBar = nullptr;
}

void CRXCoreXaml::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
	Window::Current->Content = ref new Frame();
	Window::Current->Activate();

	ApplicationView^ view = ApplicationView::GetForCurrentView();
	view->TryEnterFullScreenMode();

	CoreApplication::GetCurrentView()->TitleBar->ExtendViewIntoTitleBar = true;
	m_pTitleBar = ref new CRXTitleBar();
}

#endif
//*** Macro definition ******************************************************
#ifndef _RELENTLESS_X_CUSTOMTITLEBAR_H__
	#define _RELENTLESS_X_CUSTOMTITLEBAR_H__

#pragma once

//*** Namesapce definition ***************************************************
using namespace Platform;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

//*** Class definition *******************************************************
ref class CRXTitleBar sealed
{
	public:
		CRXTitleBar();
		void ReleaseTitleBar();

	private:
		~CRXTitleBar();
		void OnLayoutMetricsChanged(Windows::ApplicationModel::Core::CoreApplicationViewTitleBar^ sender, Platform::Object^ args);
		void OnIsVisibleChanged(Windows::ApplicationModel::Core::CoreApplicationViewTitleBar^ sender, Platform::Object^ args);
		void OnWindowSizeChanged(Platform::Object^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);

		void UpdateLayoutMetrics();
		void UpdatePositionAndVisibility();

		void FullScreenButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

	private:
		Windows::UI::Xaml::Controls::Grid^ GridBound;
		Windows::UI::Xaml::Shapes::Rectangle^ TitleBackground;
		Windows::UI::Xaml::Controls::TextBlock^ TitleName;
		Windows::UI::Xaml::Controls::Button^ FullScreenButton;
		Windows::UI::Xaml::Controls::SymbolIcon^ FullScreenLogo;

		Windows::Foundation::EventRegistrationToken layoutMetricsChangedToken{};
		Windows::Foundation::EventRegistrationToken isVisibleChangedToken{};
		Windows::Foundation::EventRegistrationToken windowSizeChangedToken{};
};

//*** Constructor ************************************************************
CRXTitleBar::CRXTitleBar()
{
	Windows::ApplicationModel::Core::CoreApplicationViewTitleBar^ CurrentTitleBar = Windows::ApplicationModel::Core::CoreApplication::GetCurrentView()->TitleBar;
	CurrentTitleBar->LayoutMetricsChanged += ref new TypedEventHandler<CoreApplicationViewTitleBar^, Object^>(this, &CRXTitleBar::OnLayoutMetricsChanged);
	CurrentTitleBar->IsVisibleChanged += ref new TypedEventHandler<CoreApplicationViewTitleBar^, Object^>(this, &CRXTitleBar::OnIsVisibleChanged);
	Window::Current->SizeChanged += ref new WindowSizeChangedEventHandler(this, &CRXTitleBar::OnWindowSizeChanged);

	GridBound = ref new Windows::UI::Xaml::Controls::Grid();
	GridBound->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Top;
	GridBound->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Left;
	Window::Current->SetTitleBar(GridBound);

	TitleBackground = ref new Windows::UI::Xaml::Shapes::Rectangle();
	SolidColorBrush^ mySolidColorBrush = ref new SolidColorBrush();
	mySolidColorBrush->Color = Windows::UI::ColorHelper::FromArgb(255, 255, 255, 255);
	TitleBackground->Fill = mySolidColorBrush;
	GridBound->Children->Append(TitleBackground);

	TitleName = ref new Windows::UI::Xaml::Controls::TextBlock();
	TitleName->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
	TitleName->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Left;
	TitleName->Padding = Windows::UI::Xaml::Thickness(12, 0, 0, 0);
	TitleName->FontSize = 12;
	TitleName->Foreground = ref new SolidColorBrush(Windows::UI::ColorHelper::FromArgb(255, 0, 0, 0));
	TitleName->Text = "Title Bar C++ Sample";
	GridBound->Children->Append(TitleName);

	FullScreenButton = ref new Windows::UI::Xaml::Controls::Button();
	FullScreenButton->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Stretch;
	FullScreenButton->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Stretch;
	FullScreenButton->Background = ref new SolidColorBrush(Windows::UI::ColorHelper::FromArgb(100, 255, 0, 0));
	FullScreenButton->Click += ref new Windows::UI::Xaml::RoutedEventHandler(this, &CRXTitleBar::FullScreenButton_Click);
	FullScreenButton->ClickMode = ClickMode::Press;

	FullScreenLogo = ref new Windows::UI::Xaml::Controls::SymbolIcon();
	FullScreenLogo->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Stretch;
	FullScreenLogo->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Stretch;
	FullScreenLogo->Symbol = Windows::UI::Xaml::Controls::Symbol::FullScreen;
	
	FullScreenLogo->RenderTransformOrigin = Point(0.5, 0.5);
	CompositeTransform^ TmpTrans = ref new Windows::UI::Xaml::Media::CompositeTransform();
	TmpTrans->ScaleX = 0.5;
	TmpTrans->ScaleY = 0.5;
	FullScreenLogo->RenderTransform = TmpTrans;

	FullScreenButton->Content = FullScreenLogo;
	GridBound->Children->Append(FullScreenButton);

	UpdateLayoutMetrics();
	UpdatePositionAndVisibility();

	GridBound->Children->Append(Window::Current->Content);
	Window::Current->Content = GridBound;
}


CRXTitleBar::~CRXTitleBar()
{
	ReleaseTitleBar();
}

void CRXTitleBar::ReleaseTitleBar()
{
	Windows::ApplicationModel::Core::CoreApplicationViewTitleBar^ CurrentTitleBar = Windows::ApplicationModel::Core::CoreApplication::GetCurrentView()->TitleBar;

	CurrentTitleBar->LayoutMetricsChanged -= layoutMetricsChangedToken;
	CurrentTitleBar->IsVisibleChanged -= isVisibleChangedToken;
	Window::Current->SizeChanged -= windowSizeChangedToken;

	GridBound->Children->Clear();
}

void CRXTitleBar::OnLayoutMetricsChanged(CoreApplicationViewTitleBar^ sender, Object^ args)
{
	UpdateLayoutMetrics();
}

void CRXTitleBar::UpdateLayoutMetrics()
{
	Windows::ApplicationModel::Core::CoreApplicationViewTitleBar^ CurrentTitleBar = Windows::ApplicationModel::Core::CoreApplication::GetCurrentView()->TitleBar;

	TitleBackground->Width = ApplicationView::GetForCurrentView()->VisibleBounds.Width;
	TitleBackground->Height = CurrentTitleBar->Height;

	FullScreenButton->Width = CurrentTitleBar->Height;
	FullScreenButton->Height = CurrentTitleBar->Height;

	//FullScreenButton->Margin = Windows::UI::Xaml::Thickness(0, 0, 0.75 * CurrentTitleBar->SystemOverlayRightInset, 0);

	//TitleBar->Height = CurrentTitleBar->Height;

	/*Thickness NewPad;

	if (FlowDirection == Windows::UI::Xaml::FlowDirection::LeftToRight)
	NewPad = Windows::UI::Xaml::ThicknessHelper::FromLengths(CurrentTitleBar->SystemOverlayLeftInset, 0, CurrentTitleBar->SystemOverlayRightInset, 0);
	else
	NewPad = Windows::UI::Xaml::ThicknessHelper::FromLengths(CurrentTitleBar->SystemOverlayRightInset, 0, CurrentTitleBar->SystemOverlayLeftInset, 0);

	Test->Padding = NewPad;*/
}

void CRXTitleBar::OnIsVisibleChanged(CoreApplicationViewTitleBar^ sender, Object^ args)
{
	UpdatePositionAndVisibility();
}

void CRXTitleBar::OnWindowSizeChanged(Object^ sender, WindowSizeChangedEventArgs^ args)
{
	UpdatePositionAndVisibility();
	UpdateLayoutMetrics();
}

void CRXTitleBar::UpdatePositionAndVisibility()
{
	Windows::ApplicationModel::Core::CoreApplicationViewTitleBar^ CurrentTitleBar = Windows::ApplicationModel::Core::CoreApplication::GetCurrentView()->TitleBar;

	if (ApplicationView::GetForCurrentView()->IsFullScreenMode)
	{
		FullScreenButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		GridBound->Visibility = CurrentTitleBar->IsVisible ? Windows::UI::Xaml::Visibility::Visible : Windows::UI::Xaml::Visibility::Collapsed;
	}
	else
	{
		FullScreenButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
		GridBound->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}
}

void CRXTitleBar::FullScreenButton_Click(Object^ sender, RoutedEventArgs^ e)
{
	ApplicationView::GetForCurrentView()->TryEnterFullScreenMode();
}

#endif
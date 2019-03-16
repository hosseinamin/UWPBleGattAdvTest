//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

namespace BleGattAdvTest
{
	using namespace Platform;
	using namespace Windows::UI::Xaml;
	using namespace Windows::Foundation;
	using namespace Windows::Devices::Bluetooth::GenericAttributeProfile;

	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	protected:
		property BOOL mAdvertising;
		IAsyncOperation<String^>^ StartHIDBLEAsync();
		// void ProtMode_ReadRequested(GattLocalCharacteristic ^sender, GattReadRequestedEventArgs ^args);
	public:
		MainPage();
		void SetMessageBox(String ^msg);
		void MainPage::AdvertisingToggleClick(Object ^sender, RoutedEventArgs ^e);
	};
}

//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <ppltasks.h>

using namespace BleGattAdvTest;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using namespace Windows::Devices::Bluetooth;
using namespace Windows::Devices::Bluetooth::GenericAttributeProfile;

using namespace Windows::Security::Cryptography;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	mAdvertising = FALSE;
}

void MainPage::AdvertisingToggleClick(Object ^sender, RoutedEventArgs ^e) {
	if (mAdvertising) {
		SetMessageBox("Ready");
	}
	else {
		SetMessageBox("Advertising");
		create_task(StartHIDBLEAsync())
			.then([this](String ^result) {
			if (result != nullptr) {
				SetMessageBox(result);
			}
		});
	}
	mAdvertising = mAdvertising ? FALSE : TRUE;
}

void MainPage::SetMessageBox(String ^msg) {
	messageBox->Text = msg;
}
/*
void MainPage::ProtMode_ReadRequested(GattLocalCharacteristic ^sender, GattReadRequestedEventArgs ^args) {

}
*/
Windows::Foundation::IAsyncOperation<String^>^ MainPage::StartHIDBLEAsync() {
	return create_async([this]() -> String^ {
		BluetoothAdapter ^bleadapter = create_task(BluetoothAdapter::GetDefaultAsync()).get();
		if (!bleadapter->IsPeripheralRoleSupported) {
			return "BLUETOOTH PERIPHERAL ROLE NOT SUPPORTED";
		}
		GattServiceProvider ^hidServiceProvider = nullptr;
		{
			GattServiceProviderResult ^result = create_task(GattServiceProvider::CreateAsync(GattServiceUuids::HumanInterfaceDevice)).get();
			if (result->Error != BluetoothError::Success) {
				return "ERROR AT GETTING HID SERVICE PROVIDER";
			}
			hidServiceProvider = result->ServiceProvider;
		}
		GattLocalCharacteristic ^protMode = nullptr;
		{
			GattLocalCharacteristicParameters ^params = ref new GattLocalCharacteristicParameters;
			params->CharacteristicProperties = GattCharacteristicProperties::Read | GattCharacteristicProperties::WriteWithoutResponse;
			byte data[] = { 0x01 };
			ArrayReference<byte> dataArray(data, sizeof(data));
			params->StaticValue = CryptographicBuffer::CreateFromByteArray(dataArray);
			GattLocalCharacteristicResult ^result = create_task(hidServiceProvider->Service->CreateCharacteristicAsync(GattCharacteristicUuids::ProtocolMode, params)).get();
			if (result->Error != BluetoothError::Success) {
				return "Error AT CREATE CHAR ProtocolMode";
			}
			protMode = result->Characteristic;
			// protMode->ReadRequested += ref new TypedEventHandler<GattLocalCharacteristic^, GattReadRequestedEventArgs^>(this, &MainPage::ProtMode_ReadRequested);
		}
		GattServiceProviderAdvertisingParameters ^adParams = ref new GattServiceProviderAdvertisingParameters();
		adParams->IsConnectable = TRUE;
		adParams->IsDiscoverable = TRUE;
		hidServiceProvider->StartAdvertising(adParams);
		return nullptr;
	});
}

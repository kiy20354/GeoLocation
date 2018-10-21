// main.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <iostream>


using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Devices::Geolocation;
using namespace Windows::Devices::Sensors;

int main()
{
	winrt::init_apartment();

	auto lightSensor = LightSensor::GetDefault();
	auto r = lightSensor.GetCurrentReading();

	lightSensor.ReportInterval(500);
	std::cout << "Light: " <<  r.IlluminanceInLux() << std::endl;

	auto accessStatus = Geolocator::RequestAccessAsync();

	Geolocator geolocator;

	switch (accessStatus.get())
	{
	case GeolocationAccessStatus::Allowed:
		// notify user: Waiting for update
		//geolocator.ReportInterval(200);
		geolocator.DesiredAccuracyInMeters(1);

		// Subscribe to StatusChanged event to get updates of location status changes
		geolocator.StatusChanged([](auto revoke, auto & handler)
		{
			auto e = handler.as< StatusChangedEventArgs>().Status();
			
			std::string ScenarioOutput_Status;

			switch (handler.as< StatusChangedEventArgs>().Status())
			{
			case PositionStatus::Ready:
				// Location platform is providing valid data.
				ScenarioOutput_Status = "Ready";
//				Console.WriteLine("Location platform is ready.");
				break;

			case PositionStatus::Initializing:
				// Location platform is attempting to acquire a fix. 
				ScenarioOutput_Status = "Initializing";
	//			Console.WriteLine("Location platform is attempting to obtain a position.");
				break;

			case PositionStatus::NoData:
				// Location platform could not obtain location data.
				ScenarioOutput_Status = "No data";
		//		Console.WriteLine("Not able to determine the location.");
				break;

			case PositionStatus::Disabled:
				// The permission to access location data is denied by the user or other policies.
				ScenarioOutput_Status = "Disabled";
			//	Console.WriteLine("Access to location is denied.");

				// Clear cached location data if any
	//			UpdateLocationData(null);
				break;

			case PositionStatus::NotInitialized:
				// The location platform is not initialized. This indicates that the application 
				// has not made a request for location data.
				ScenarioOutput_Status = "Not initialized";
				//Console.WriteLine("No request for location is made yet.");
				break;

			case PositionStatus::NotAvailable:
				// The location platform is not available on this version of the OS.
				ScenarioOutput_Status = "Not available";
	//			Console.WriteLine("Location is not available on this version of the OS.");
				break;

			default:
				ScenarioOutput_Status = "Unknown";
		//		Console.WriteLine(string.Empty);
				break;
			}
		});
		
		// Subscribe to PositionChanged event to get updated tracking positions
		geolocator.PositionChanged( 
			[]( auto revoke, auto  & handler) {
			
		//	auto pos = handler.as< PositionChangedEventArgs>().Position;
			
			std::cout << "Position:" << std::endl;
			std::cout << "Latitude:" << handler.as< PositionChangedEventArgs>().Position().Coordinate().Latitude() << std::endl;
			std::cout << "Longitude:" << handler.as< PositionChangedEventArgs>().Position().Coordinate().Longitude() << std::endl;


		});


		break;
		case GeolocationAccessStatus::Denied:
			break;
		case GeolocationAccessStatus::Unspecified: 
			break;
	}

	// 停止
	std::string s;
	std::cin >> s;

}


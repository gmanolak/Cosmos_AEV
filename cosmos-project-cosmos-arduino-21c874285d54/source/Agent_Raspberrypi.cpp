


/* If not, go to <http://www.gnu.org/licenses/>
 * *
 * * COSMOS/core is free software: you can redistribute it and/or
 * * modify it under the terms of the GNU Lesser General Public License
 * * as published by the Free Software Foundation, either version 3 of
 * * the License, or (at your option) any later version.
 * *
 * * COSMOS/core is distributed in the hope that it will be useful, but
 * * WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * * Lesser General Public License for more details.
 * *
 * * Refer to the "licences" folder for further information on the
 * * condititons and terms to use this software.
 * ********************************************************************/
/*
#include "support/configCosmos.h"
#include "agent/agentclass.h"
#include "device/serial/serialclass.h"
#include <iostream>
#include <fstream>
//#define NUM_DATA 2


using namespace std;
Agent *agent;
ofstream file;
// use: agent_arduino <agent_name> <device path> <sensor number(ex: 000,001,...)>



int main(int argc, char** argv)
{
cout << "Starting Agent AEV" << endl;
Agent *agent;
string nodename = "cubesat1";
string agentname = "Raspberry"; //name of the agent that the request is directed to
string soh;
string serial_port = "/dev/ttyS0";
size_t serial_baud = 9600;
string sensor_num = "000";
if(argc >1)
agentname = argv[1];
if (argc >2) 
{
serial_port = argv[2];
}
if(argc >3){
sensor_num = argv[3];
}

switch (argc)
{
case 3:
nodename = argv[2];
case 2:
agentname = (string)argv[1] + "Raspberry";
//raspbi  = (string)argv[1] + "_Raspberry";
}

agent = new Agent(nodename, agentname);
//Set SOH String
// include all namespace names used - the names that are printed from the arduino serial port
soh= "{\"node_loc_utc\","
"\"node_loc_pos_eci\"}" ;
agent->set_sohstring(soh);

ElapsedTime et;
et.start();
// set up reading from serial port
// *
Serial serial_Raspberry = Serial(serial_port,serial_baud);
serial_Raspberry.set_timeout(0, 3.);

cout << "serial port: " << serial_port << " at " << serial_baud << endl;
if (serial_Raspberry.get_error() < 0) {
// there was error opening the serial port, close the program
cout << "error opening serial port: " << serial_Raspberry.get_error()  << endl;
return 0;
}
 * //
// Start executing the agent
while(agent->running())
{
int32_t status;
string jsonstring;
jsonstring="$GPGGA,015049.811,4054.929,N,08102.499,W,0,00,,,M,,M,,*58";
// read serial port from the arduino
// reads the first line and saves it in jsonstring
// status = serial_Raspberry.get_string(jsonstring, '\n');
// if(status > 0)
//{

status = json_parse(jsonstring, agent->cinfo);
// sanity check
cout<<jsonstring<<endl;
//}
//sleep for 1 sec
COSMOS_SLEEP(0.1);
}

return 0;
}

*/
#include <support/configCosmos.h>
#include <agent/agentclass.h>
#include <serial/serialclass.h>
#include <support/stringlib.h>
//#include "nmea.h"
#include <iostream>
#include <fstream>
using namespace std;

devicestruc* device_from_name(string device_name, cosmosstruc* cinfo);

int main(int argc, char** argv)
{
	cout << "Agent GPS" << endl;
	Agent *agent;
	string nodename = "cubesat1";
	string agentname = "gps";
	agent = new Agent(nodename, agentname);
	//string serial_port = "/dev/ttyUSB0";
	size_t serial_baud = 9600;
	//Serial serial_gps = Serial(serial_port,serial_baud);
	// serial_gps.set_timeout(0, 3.);
	//cout << "serial port: " << serial_port << " at " << serial_baud << endl;
	//  if (serial_gps.get_error() < 0) {
	//	       cout << "error opening serial port: " << serial_gps.get_error()  << endl;
	//	               return 0;
	//		           } else {
	//				           cout << "sucess opening serial port " << endl;
	//					       }
	string soh = "{\"node_loc_utc\","
		"\"device_gps_geods_000\"}" ;
	agent->set_sohstring(soh.c_str());
	while(agent->running())
	{
		int32_t status;
		string gpsdata;
		//status = serial_gps.get_string(gpsdata, '\n');
		gpsdata="$GPGGA,015049.811,4054.929,N,08102.499,W,0,00,,,M,,M,,*58";

		StringParser nmea(gpsdata);
		string nmea_id = nmea.getFieldNumber(1);
		double gps_latitude = 0;
		double gps_longitude = 0;
		double gps_altitute = 0;
		if (nmea_id.compare("$GPGGA") == 0) {
			string time_utc = nmea.getFieldNumber(2);
			int hours = stoi(time_utc.substr(0,2));
			int minutes = stoi(time_utc.substr(2,2));
			float seconds = stof(time_utc.substr(4,6));
			string lat_str = nmea.getFieldNumber(3);
			string indicator_ns = nmea.getFieldNumber(4);
			string lon_str  = nmea.getFieldNumber(5);
			string indicator_ew = nmea.getFieldNumber(7);
			int gps_quality = nmea.getFieldNumberAsInteger(7);
			int num_sats = nmea.getFieldNumberAsInteger(8);
			gps_altitute = nmea.getFieldNumberAsDouble(10);
			double lat_dd = stod(lat_str.substr (0,2));
			double lat_mm = stod(lat_str.substr (2,8));
			double lon_dd = stod(lon_str.substr (0,3));
			double lon_mm = stod(lon_str.substr (3,9));
			gps_latitude = lat_dd + lat_mm/60.;
			gps_longitude = lon_dd + lon_mm/60.;
			cout << gpsdata;
			cout << "Time UTC:\t" << hours << ":" << minutes << ":" << fixed << setprecision(3) << seconds << endl;

			cout << "Lat:\t" << lat_dd << "d " << lat_mm << "' " << indicator_ns << endl;
			cout << "Lat:\t" << setprecision (10) << gps_latitude << endl;
			cout << "Lon:\t" << lon_dd << "d " << lon_mm << "' " << indicator_ew << endl;
			cout << "Lon:\t" << setprecision (10) << gps_longitude << endl;
			cout << "Quality:\t" << gps_quality << endl;
			cout << "# Sats:\t" << num_sats << endl;
			cout << "Alt:\t" << gps_altitute << endl; 

			devicestruc* device_gps = device_from_name("GPS",agent->cinfo);

			if (device_gps != nullptr) {
				agent->cinfo->devspec.gps[1]->utc = currentmjd(0);
				agent->cinfo->devspec.gps[0]->geods.lat = gps_latitude;
				agent->cinfo->devspec.gps[0]->geods.lon = gps_longitude;
				agent->cinfo->devspec.gps[0]->geods.h = gps_altitute;
			}
		}
		COSMOS_SLEEP(0.1);
	}
	return 0;
}

devicestruc* device_from_name(string device_name, cosmosstruc* cinfo) {
	devicestruc* device = nullptr;
	for (size_t i=0; i< cinfo->device.size(); ++i) {
		char* device_name_from_list = cinfo->pieces[cinfo->device[i].all.pidx].name;
		if (device_name == device_name_from_list) {
			device = &cinfo->device[i];
			return device;
		}
	}
	return device;
}


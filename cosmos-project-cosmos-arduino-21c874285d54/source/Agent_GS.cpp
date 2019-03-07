/********************************************************************
 * Copyright (C) 2015 by Interstel Technologies, Inc.
 *   and Hawaii Space Flight Laboratory.
 *
 * This file is part of the COSMOS/core that is the central
 * module for COSMOS. For more information on COSMOS go to
 * <http://cosmos-project.com>
 *
 * The COSMOS/core software is licenced under the
 * GNU Lesser General Public License (LGPL) version 3 licence.
 *
 * You should have received a copy of the
 * GNU Lesser General Public License
 * If not, go to <http://www.gnu.org/licenses/>
 *
 * COSMOS/core is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * COSMOS/core is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * Refer to the "licences" folder for further information on the
 * condititons and terms to use this software.
 ********************************************************************/
/*

#include "support/configCosmos.h"
#include "agent/agentclass.h"
#include "device/serial/serialclass.h"
#include <iostream>
#include <fstream>
using namespace std;
Agent *agent;
*/

/* COSMOS Agent for a Raspberry pi
 * runs with the temp.py Python script in the cosmos-arduino/python directory
 * Node: node-arduino
 * Agent: raspi
 * JSON data: "device_tsen_temp_000"
 */

/*
   int main(int argc, char** argv)
   {
   cout << "Starting Agent GS" << endl;
   Agent *agent;
   string nodename="cubesat1";
//string agentname="002";
//string nodename = "nodeAEV1";
string agentname = "GS"; //name of the agent that the request is directed to
string soh;
jsonnode response;
//string raspbi="001";
string raspbi= "Raspberry";

size_t serial_baud =9600;
if(argc >1)
agentname = argv[1];
switch (argc)
{
case 3:
nodename = argv[2];
case 2:
agentname = (string)argv[1] + "_GS";
raspbi  = (string)argv[1] + "_Raspberry";
}
agent = new Agent(nodename, agentname);

//Set SOH String
// include all namespace names used - the names that are printed from the raspberry pi

soh= "{\"node_loc_utc\","
"\"node_loc_pos_eci\"}";

agent->set_sohstring(soh);

ElapsedTime et;
et.start();

// Find GS agent on network
beatstruc beat_agent_raspi;
beat_agent_raspi=agent->find_server(nodename,raspbi, 2.);

// Start executing the agent
while(agent->running())
{

int32_t status;
int32_t jsonstring;

// ??? >> request_string()?  (must do add_request() first)
// 
agent->send_request_jsonnode(beat_agent_raspi, response, 2.); // reponse.node is our string 

// Check if string went through
if(response.node.size() > 1) {
cout << "Recieved: " << response.node<< "from Agent AEV. Thank you brother." << endl;

// Convert_NMEA(response.node);
// Example NMEA output: $GPGGA,181908.00,3404.7041778,N,07044.3966270,W,4,13,1.00,495.144,M,29.200,M,0.10,0000*40
// Print converted output

response.node.clear();
}

else {
cout << "Nothing." << endl;
beat_agent_raspi.node[0] = '\0'; // reset
beat_agent_raspi=agent->find_server(nodename, raspbi, 2.);
cout << "beat agent AEV node: " << beat_agent_raspi.utc << endl;
}

//sleep for 1 sec
COSMOS_SLEEP(0.1);
}

return 0;
}

*/





#include <support/configCosmos.h>
#include <agent/agentclass.h>
#include <iostream>
#include <fstream>
using namespace std;
int main(int argc, char** argv)
{
	cout << "Monitor GPS" << endl;
	Agent *agent;
	 string nodename = "cubesat1";
	 string agentname = "gps_monitor";
	 agent = new Agent(nodename, agentname);
	 int32_t iretn;
	 Agent::messstruc mess;
	 while(agent->running())
	 {
		 iretn = agent->readring(mess, Agent::AgentMessage::BEAT, 5., (Agent::Where)1);
		 if (iretn > 0 && !strcmp(mess.meta.beat.proc, "gps"))
		 {
			 json_parse(mess.adata, agent->cinfo);
			 double gps_latitude = agent->cinfo->devspec.gps[0]->geods.lat;
			 double gps_longitute = agent->cinfo->devspec.gps[0]->geods.lon;
			  double gps_altitude = agent->cinfo->devspec.gps[0]->geods.h;
			   cout << "GPS (lat, lon, h) = [" << gps_latitude << ", " <<  gps_longitute << ", " << gps_altitude << "]" << endl;
			   // GUI comman:wq
			
		 }
		 COSMOS_SLEEP(1.0);
	 }
	 return 0;
}



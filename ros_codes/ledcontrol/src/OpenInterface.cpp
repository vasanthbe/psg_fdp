#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <netinet/in.h>
#include <sys/types.h>
#include <sstream>
#include <cstring>
#include <ledcontrol/OpenInterface.h>
#include <iostream>

// *****************************************************************************
// Constructor
irobot::OpenInterface::OpenInterface(const char * new_serial_port)
{	
	port_name_ = new_serial_port;
	serial_port_ = new sereal::SerealPort();
	packets_size_ = 100;
}


// *****************************************************************************
// Destructor
irobot::OpenInterface::~OpenInterface()
{
	// Clean up!
	delete serial_port_;
}


// *****************************************************************************
// Open the serial port
int irobot::OpenInterface::openSerialPort(bool full_control)
{
	try{ serial_port_->open(port_name_.c_str(), 9600); }
	catch(sereal::Exception& e){ return(-1); }
	return(0);
}


// *****************************************************************************

// Close the serial port
int irobot::OpenInterface::closeSerialPort()
{
	try{ serial_port_->close(); }
	catch(sereal::Exception& e){ return(-1); }
	return(0);
}

int irobot::OpenInterface::flushSerialPort()
{
	try{ serial_port_->flush(); }
	catch(sereal::Exception& e){ return(-1); }
	return(0);
}

// *****************************************************************************
// Send an OP code to the roomba
int irobot::OpenInterface::setLedOn()
{	 
 	 std::stringstream sstr;
 	 sstr.clear();
 	 sstr << "<l:1>\r\n";
 	 std::string bstr(sstr.str());
 	 const char* to_send = bstr.c_str(); 
 	 std::cout<<"Serial write:"<<to_send; 
 	 
	try{ serial_port_->write(to_send, strlen(to_send)); }
	catch(sereal::Exception& e){ return(-1); }
	return(1);
}

int irobot::OpenInterface::setLedOff()
{
 	 std::stringstream sstr;
 	 sstr.clear();
 	 sstr << "<l:0>\r\n";
 	 std::string bstr(sstr.str());
 	 const char* to_send = bstr.c_str(); 	 
  	 std::cout<<"Serial write:"<<to_send;
 	  	 
	try{ serial_port_->write(to_send, strlen(to_send)); }
	catch(sereal::Exception& e){ return(-1); }
	return(1);
}

int irobot::OpenInterface::startStream()
{
 	 is_stream = true;
 	 std::stringstream sstr;
 	 sstr.clear();
 	 sstr << "<s:1>\r\n";
 	 std::string bstr(sstr.str());
 	 const char* to_send = bstr.c_str();	 
 	 
	try{ serial_port_->write(to_send, strlen(to_send)); }
	catch(sereal::Exception& e){ return(-1); }
	return(1);
}

int irobot::OpenInterface::stopStream()
{
	 is_stream = false;
 	 std::stringstream sstr;
 	 sstr.clear();
 	 sstr << "<s:0>\r\n";
 	 std::string bstr(sstr.str());
 	 const char* to_send = bstr.c_str();

	try{ serial_port_->write(to_send, strlen(to_send)); }
	catch(sereal::Exception& e){ return(-1); }
	return(1);
}

int irobot::OpenInterface::setStreamRate(int rate)
{
 	 std::stringstream sstr;
 	 sstr.clear();
 	 sstr << "<t:" << rate <<">\r\n";
 	 std::string bstr(sstr.str());
 	 const char* to_send = bstr.c_str();
 	  	 
	try{ serial_port_->write(to_send, strlen(to_send)); }
	catch(sereal::Exception& e){ return(-1); }
	return(1);
}

// Read the sensors stream
int irobot::OpenInterface::streamSensorPackets()
{
	char data_buffer[packets_size_];
	int receive_stream_len = 23;
	try{ serial_port_->readBytes(data_buffer, receive_stream_len, 100); }
	catch(sereal::Exception& e){ return(-1); }
	
	return this->parseSensorPackets((unsigned char*)data_buffer);
}

// Parse sensor data
int irobot::OpenInterface::parseSensorPackets(unsigned char * buffer)
{
 	std::stringstream ssbuffer;  	 
	ssbuffer << buffer;
  	std::string sbuffer(ssbuffer.str());
  	std::string ebuffer;
  	int index = 999;
  	
  	int stfound, edfound;
  	stfound = sbuffer.find("<");
  	edfound = sbuffer.rfind(">");
//	std::cout<<"sBuffer::"<<buffer<<std::endl;
	if((stfound >= 0) and (edfound > stfound))
	{
		ebuffer = sbuffer.substr(stfound+1, (edfound - stfound - 1));

		stfound = 0;
		index = 0;		
		//std::cout<<"Buffer::"<<ebuffer<<std::endl;
		while((index < ebuffer.length()) and (is_stream))
		{
			edfound = ebuffer.find("#",index);
			if(edfound >= 0)
			{
				index = edfound;	
				index = parseSensorPacket(ebuffer, index);
			}
			else 
				index = 999;
		}
	}
	return index;
}


int irobot::OpenInterface::parseSensorPacket(std::string buffer, int index)
{
	char command = buffer[index + 1];
	std::size_t stfound, etfound;
	int rindex = 999;
	int indx = 0;
		
	switch(command)
	{
		case 'a':
			stfound = buffer.find(":",index);
			etfound = buffer.find(",",stfound);
			
			if ((not(etfound > 0))or(etfound > packets_size_))
				etfound = buffer.length();
			
			indx = (int)buffer[stfound - 1];
			analog_data[indx] = (int)(buffer[stfound + 1] | buffer[stfound + 2]);
			rindex = etfound + 1;
			break;
		case 'c':
			stfound = buffer.find(":",index);
			etfound = buffer.find(",",stfound);
			
			if ((not(etfound > 0))or(etfound > packets_size_))
				etfound = buffer.length();
			indx = 0;			
			analog_data[indx] = (int)(buffer[stfound + 1] | buffer[stfound + 2]);
			rindex = etfound + 1;
			break;
	}
	return rindex;
}




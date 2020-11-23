
#include <sereal/sereal.h>

namespace irobot
{
	class OpenInterface
	{
		int packets_size_;
		bool is_stream;
		
		public:
		int analog_data[8];	
		
		//! Constructor
		/*!
		 * By default the constructor will set the Roomba to read only the encoder counts (for odometry).
		 *
		 *  \param new_serial_port    Name of the serial port to open.
		 *
		 *  \sa setSensorPackets()
		 */
		OpenInterface(const char * new_serial_port);
		//! Destructor
		~OpenInterface();
	
		//! Open the serial port
		/*!
		 *  \param full_control    Whether to set the Roomba on OImode full or not.
		 */
		int openSerialPort(bool full_control);
		//! Close the serial port
		int closeSerialPort();
		int flushSerialPort();
		int setLedOn();
		int setLedOff();
		int startStream();
		int stopStream();
		int setStreamRate(int rate);
		int streamSensorPackets();
		int parseSensorPackets(unsigned char * buffer);
		int parseSensorPacket(std::string buffer, int index);

		//! Serial port to which the robot is connected
		std::string port_name_;
		//! sereal port object
		sereal::SerealPort * serial_port_;
	
	};

}

// EOF

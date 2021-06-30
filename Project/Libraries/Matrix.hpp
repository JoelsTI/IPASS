#ifndef Matrix
#define Matrix
#include "hwlib.hpp"
#include "command_list.hpp"

/// @file

/// \brief
/// Setup of Pins
/// \details
/// This is a class made to setup the pins for ease of use.
/// An array is made so that the pins can all be setup at once.
class pin_setup : public hwlib::pin_in_out{
protected:
    std::array< hwlib::pin_in_out *, 4> pinnen;
public:
    pin_setup(hwlib::pin_in_out & data,
          hwlib::pin_in_out & write = hwlib::pin_in_out_dummy,
          hwlib::pin_in_out & read = hwlib::pin_in_out_dummy,
          hwlib::pin_in_out & cs = hwlib::pin_in_out_dummy):
            pinnen{ &data, &write, &read, & cs}{}

/// \brief
/// sets a pin to output mode
/// \details
/// This function makes it so that a pin can be used as an output.
/// An example of this is a LED that has to emit light.				
    void direction_set_output() override{
        for(const auto &p : pinnen){
            p->direction_set_output();
        }
    }

/// \brief
/// sets a pin to input mode
/// \details 
/// This function makes it so that a pin can be used as an input.
/// An example of this is a button that can be used to turn on a LED.   	
	void direction_set_input() override{
        for(const auto &p : pinnen){
            p->direction_set_input();
        }
    }

/// \brief
/// writes data to a pin
/// \details
/// This function allows it so that data can be written towards a pin.
/// An example of this is making a LED turn on or off.	
	void write(bool v) override{
        for(const auto &p : pinnen){
            p->write(v);
        }
    }
	
/// \brief 	
/// flushes the pin
/// \details
/// This function flushes the pin.
/// This clears the internal buffer.
    void flush() override{
        for(const auto &p : pinnen){
            p->flush();
        }
    }

/// \brief
/// reads the pin
/// \details
/// This function reads the pin.
/// This function returns false by standard.
/// If it is able to read the pin, it returns true	
    bool read() override{
        bool ret = false;
        for(const auto &p : pinnen){
            ret |= p->read();
        }
        return ret;
    }
    void refresh() override{
        for(const auto &p : pinnen){
            p->refresh();
        }
    }
    void direction_flush() override{
        for(const auto &p : pinnen){
            p->direction_flush();
        }
    }
};

/// \brief
/// SPI Bus Implementation
/// \details
/// The constructor puts all the pins into the bus.
/// the write is used to write clock input.
/// the data is used to send over data.
/// the cs pin is the chip select pin.
class bus{
protected:
   friend class writeTransaction;
public:
	hwlib::pin_in_out &write;
    hwlib::pin_in_out &data;
    hwlib::pin_in_out &cs;
    bus(hwlib::pin_in_out &write, hwlib::pin_in_out &data, hwlib::pin_in_out & cs):
    write( write ),
    data ( data ),
    cs ( cs )
    {}
	
/// \brief
/// sets pins to output
/// \details
///	sets Ht1632C Led Matrix pins to output and flushes these pins.
    void set_output(){
        write.direction_set_output();
        data.direction_set_output();
        cs.direction_set_output();
        write.direction_flush();
        data.direction_flush();
        cs.direction_flush();
    }
};

/// \brief
/// SPI Transaction
/// \details
/// This class takes the spi bus and creates a transaction from it.
/// The CS pin starts off high to mark the beginning and ends low to mark the ending.
class writeTransaction{
protected:
    bus &b;
    hwlib::pin_in_out &write;
    hwlib::pin_in_out &data;
    hwlib::pin_in_out &cs;
    friend class bus;
public:
    writeTransaction(bus &b):
        b ( b ),
        write ( b.write),
        data ( b.data),
        cs ( b.cs )
    {
        b.set_output();
        cs.write(0);
    }
    
/// \brief
/// writes data to the bus
/// \details
///	This is a function that sends data to the receiving chip.
/// The byte_length is an indication of how many bytes are sent.
/// a stands for the data to send.	
    void writeData(uint8_t byte_length, uint16_t a){
        for (uint16_t bit = 1<<(byte_length-1); bit; bit >>= 1) {
            write.write(0);
            data.write((a & bit) ? 1 : 0);
            hwlib::wait_ms(1);
//            hwlib::cout << "Data: " << data.read()<< "\n"; uncomment voor debugging
            write.write(1);
        }
    }
	
    ~writeTransaction(){
        cs.write(1);
    }
};

/// \brief
/// Matrix HT1632C
/// \details
/// This is the self-written library for my IPASS project. 
/// The LED-matrix I'm using has a length of 16 pixels and a width of 24 pixels.
/// It is controlled by the HT1632 chip. This chip uses a SPI bus.
class HT1632C : public writeTransaction{
protected:
	bus &b;
	hwlib::pin_in_out &write;
	hwlib::pin_in_out &data;
	hwlib::pin_in_out &cs;
	uint16_t array[24] = {0};
	friend class bus;
	friend class writeTransaction;
public:
	HT1632C(bus &b):
		writeTransaction(b),
		b(b),
		write(b.write),
		data(b.data),
		cs(b.cs)
		{}

/// \brief
/// send command to LED matrix
/// \details
///	This function creates a temporary transaction of the spi bus.
/// It is used to send commands to the HT1632C chip.	
/// The standard bit given is to enable the System, which must be on at all times to ensure that changes are actually being made.		
void cmnd(uint8_t cmnd = 0x01){
	writeTransaction command(b);
	command.writeData(12, (((uint16_t)HT1632C_DATA_LEN << 8) | cmnd) << 1 );
}

/// \brief
/// Initialize LED Matrix
/// \details
/// Using the cmnd function above, it turns on all the necessary commands for the chip to work.
/// First of all the system oscillator is turned on. Because the standard bit given is already the byte needed, no byte is given.
/// Secondly the LED duty cycle generator is turned on. This allows the LEDS to turn on and off.
/// Thirdly the Blinking effect is turned off, so that we get a still image.
/// Fourthly the on-chip RC oscillator is turned on.
/// Lastly the N-MOS open drain output and 16 COM option is selected.
void initialize(){
	cmnd();
	cmnd(HT1632C_CMD_LEDON);
	cmnd(HT1632C_CMD_BLINKOFF);
	cmnd(HT1632C_CMD_INT_RC);
	cmnd(HT1632C_CMD_COMS01);	
} 

/// \brief
/// Clears the LED Matrix
/// \details
/// This function clears all the LEDS on the LED Matrix.
/// First off the operation write is called upon with the first writeData command.
/// Second off 0 is being written to all 7 of the memory adresses.
/// Lastly it writes 0 to all the ROWS and COMS.
void clear(){
	writeTransaction command(b);
	command.writeData(3, 0x05);
	command.writeData(7, 0x00);
	for(int i = 0; i < 24; i++){
		command.writeData(16, 0x0000);
	}
}

/// \brief
/// Sets a Pixel on the LED Matrix
/// \details
/// This function sets a pixel on the LED matrix using hwlib::xy.
/// It creates an x location from 0 to 15 and an y location from 0 to 23.
void setPixel(hwlib::xy xy) {
		if((xy.x < 0) || (xy.x >= 16) || (xy.y < 0) || (xy.y >= 24)) return;
		array[xy.y] |= 0x8000 >> xy.x;
}

/// \brief
/// Flushes the data
/// \details
/// All the data in the buffer gets transferred to the permanent memory.
/// Once this function is called upon changes actually happen on the LED matrix.
/// This is because the flush function allows all the commands to be processed.
void flush(){
	writeTransaction command(b);
	command.writeData(3, 0x05);
	command.writeData(7, 0x00);
	for(int i = 0; i < 24; i++){
		command.writeData(16, array[i]);
	}
}

};



#endif
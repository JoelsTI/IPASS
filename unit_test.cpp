#include "hwlib.hpp"
#include "command_list.hpp"

class pin_setup : public hwlib::pin_in_out{
protected:
    std::array< hwlib::pin_in_out *, 4> pinnen;
public:
    pin_setup(hwlib::pin_in_out & data,
          hwlib::pin_in_out & write = hwlib::pin_in_out_dummy,
          hwlib::pin_in_out & read = hwlib::pin_in_out_dummy,
          hwlib::pin_in_out & cs = hwlib::pin_in_out_dummy):
            pinnen{ &data, &write, &read, & cs}{}
			
    void direction_set_output() override{
        for(const auto &p : pinnen){
            p->direction_set_output();
        }
    }
	
    void write(bool v) override{
        for(const auto &p : pinnen){
            p->write(v);
        }
    }
	
    void flush() override{
        for(const auto &p : pinnen){
            p->flush();
        }
    }
	
    void direction_set_input() override{
        for(const auto &p : pinnen){
            p->direction_set_input();
        }
    }
	
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
	
    void set_output(){
        write.direction_set_output();
        data.direction_set_output();
        cs.direction_set_output();
        write.direction_flush();
        data.direction_flush();
        cs.direction_flush();
    }
};

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
	
    void writeData(uint8_t byte_length, uint16_t a){
        for (uint16_t bit = 1<<(byte_length-1); bit; bit >>= 1) {
            write.write(0);
            data.write((a & bit) ? 1 : 0);
			hwlib::cout << data.read()<< " "; 
            hwlib::wait_ms(1);
            write.write(1);
        }
    }
	
    ~writeTransaction(){
        cs.write(1);
    }
};

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
		
void cmnd(uint8_t cmnd = 0x01){
	writeTransaction command(b);
	command.writeData(12, (((uint16_t)HT1632C_DATA_LEN << 8) | cmnd) << 1 );
}

void initialize(){
	hwlib::cout << "Since we are expecting a command, the first 3 bits for every command should be 1 0 0" << "\n";
	hwlib::cout << "After those 3 bits, there should be 8 more bits containing the correct command with 1 insignificant bit behind it, the X stands for the insignificant bit " << "\n";
	
	hwlib::cout << "\n" << "1 0 0 " << "0 0 0 0 0 0 0 1" << " X" << " check" << "\n";
	cmnd();
	hwlib::cout << "passed" << "\n";
	
	hwlib::cout << "\n" << "1 0 0 " << "0 0 0 0 0 0 1 1" << " X" << " check" << "\n";
	cmnd(HT1632C_CMD_LEDON);
	hwlib::cout << "passed" << "\n";
	
	hwlib::cout << "\n" << "1 0 0 " << "0 0 0 0 1 0 0 0" << " X" << " check" << "\n";
	cmnd(HT1632C_CMD_BLINKOFF);
	hwlib::cout << "passed" << "\n";
	
	hwlib::cout << "\n" << "1 0 0 " << "0 0 0 1 1 0 0 0" << " X" << " check" << "\n";
	cmnd(HT1632C_CMD_INT_RC);
	hwlib::cout << "passed" << "\n";
	
	hwlib::cout << "\n" << "1 0 0 " << "0 0 1 0 0 1 0 0" << " X"<< " check" << "\n";
	cmnd(HT1632C_CMD_COMS01);	
	hwlib::cout << "passed" << "\n";
	hwlib::cout << "==================================================" << "\n" << "\n";
} 

void clear(){
	writeTransaction command(b);
	hwlib::cout << "The first 3 bits that have to be written are 1 0 1, these are to set it to the write id." << "\n";
	command.writeData(HT1632C_ID_LEN, 0x05);
	hwlib::cout << "\n" << "After that the memory addresses are written with just 0's" << "\n"; 
	command.writeData(HT1632C_ADDRESS_LEN, 0x00);
	hwlib::cout << "\n";
	for(int i = 0; i < 24; i++){
		command.writeData(16, 0x0000);
	}
	hwlib::cout << "passed" << "\n";
	hwlib::cout << "==================================================" << "\n" << "\n";
}

void setPixel(hwlib::xy xy) {
		if((xy.x < 0) || (xy.x >= 16) || (xy.y < 0) || (xy.y >= 24)) return;
		array[xy.y] |= 0x8000 >> xy.x;
}

void flush(){
	writeTransaction command(b);
	hwlib::cout << "The first 3 bits that have to be written are 1 0 1, these are to set it to the write id." << "\n";
	command.writeData(HT1632C_ID_LEN, 0x05);
	hwlib::cout << "\n" << "Afterwards the memory address should be written with a byte of 7 bits all containing 0's" << "\n";
	command.writeData(HT1632C_ADDRESS_LEN, 0x00);
	hwlib::cout << "\n" << "Afterwards the bits in the array are written to the HT1632C, if this array is empty, only 0's will be written." << "\n";
	for(int i = 0; i < 24; i++){
		command.writeData(16, array[i]);
	}
	hwlib::cout << "passed" << "\n";
	hwlib::cout << "==================================================" << "\n" << "\n";
}

};

int main(void){
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    namespace target = hwlib::target;
    auto data = target::pin_in_out(target::pins::d8);
    auto write = target::pin_in_out(target::pins::d9);
    auto read = target::pin_in_out(target::pins::d10);
    auto cs = target::pin_in_out(target::pins::d11);
    auto setup = pin_setup(data, write, read, cs);
    setup.direction_set_output();
    setup.direction_flush();
	hwlib::wait_ms(2000);
    bus bus(write, data, cs);
	HT1632C ht(bus);
	hwlib::cout << "================= INITIALIZE TEST =================" << "\n";
	ht.initialize();
	hwlib::cout << "================= CLEARS TEST =================" << "\n";
	ht.clear();
	hwlib::cout << "================= FLUSH TEST: EMPTY ARRAY =================" << "\n";
	ht.flush();
	hwlib::cout << "================= FLUSH TEST: FULL ARRAY =================" << "\n";
	for( int x = 0; x < 16; x++){
		for( int y = 0; y < 24; y++){
			hwlib::xy xy1(x, y);
			ht.setPixel(xy1);
		}
	}
	ht.flush();
	hwlib::wait_ms(1);
	}
	



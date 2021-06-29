#ifndef Matrix
#define Matrix
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
            hwlib::wait_ms(1);
//            hwlib::cout << "Data: " << data.read()<< "\n"; uncomment voor debugging
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
	cmnd();
	cmnd(HT1632C_CMD_LEDON);
	cmnd(HT1632C_CMD_BLINKOFF);
	cmnd(HT1632C_CMD_INT_RC);
	cmnd(HT1632C_CMD_COMS01);	
} 

void clear(){
	writeTransaction command(b);
	command.writeData(3, 0x05);
	command.writeData(7, 0x00);
	for(int i = 0; i < 24; i++){
		command.writeData(16, 0x0000);
	}
}

void setPixel(hwlib::xy xy) {
		if((xy.x < 0) || (xy.x >= 16) || (xy.y < 0) || (xy.y >= 24)) return;
		array[xy.y] |= 0x8000 >> xy.x;
}

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
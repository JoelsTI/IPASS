#include "hwlib.hpp"
#include "command_list.hpp"
#include "Matrix.hpp"

int main(void){
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    namespace target = hwlib::target;
    auto data = target::pin_in_out(target::pins::d8);
    auto write = target::pin_in_out(target::pins::d9);
    auto read = target::pin_in_out(target::pins::d10);
    auto cs = target::pin_in_out(target::pins::d11);
	auto sw_steen_p1 = target::pin_in_out(hwlib::target::pins::d7);
	auto sw_papier_p1 = target::pin_in_out(hwlib::target::pins::d6);
	auto sw_schaar_p1 = target::pin_in_out(hwlib::target::pins::d5);
	auto sw_steen_p2 = target::pin_in_out(hwlib::target::pins::d4);
	auto sw_papier_p2 = target::pin_in_out(hwlib::target::pins::d3);
	auto sw_schaar_p2 = target::pin_in_out(hwlib::target::pins::d2);
    auto setup = pin_setup(data, write, read, cs);
	int steen_p1 = 0;
	int papier_p1 = 0;
	int schaar_p1 = 0;
	int steen_p2 = 0;
	int papier_p2 = 0;
	int schaar_p2 = 0;
	bool p1_keuze = 0;
	bool p2_keuze = 0;
    setup.direction_set_output();
    setup.direction_flush();
	hwlib::wait_ms(2000);
    bus bus(write, data, cs);
	HT1632C ht(bus);
	ht.initialize();
	hwlib::wait_ms(1);
	ht.clear();
	sw_steen_p1.direction_set_input();
	sw_papier_p1.direction_set_input();
	sw_schaar_p1.direction_set_input();
	sw_steen_p2.direction_set_input();
	sw_papier_p2.direction_set_input();
	sw_schaar_p2.direction_set_input();
	while(true){
	if(p1_keuze == 0){	
		if(sw_steen_p1.read()){
			if(steen_p1 == 0){
				hwlib::cout << "Player 1 has chosen"<< "\n";
				hwlib::wait_ms(10);
				steen_p1 = 1;
				p1_keuze = 1;
			}
		}
		
		if(sw_papier_p1.read()){
			if(papier_p1 == 0){
				hwlib::cout << "Player 1 has chosen" << "\n";
				hwlib::wait_ms(10);
				papier_p1 = 1;
				p1_keuze = 1;
			}
		}
		
		if(sw_schaar_p1.read()){
			if(schaar_p1 == 0){
				hwlib::cout << "Player 1 has chosen" << "\n";
				hwlib::wait_ms(10);
				schaar_p1 = 1;
				p1_keuze = 1;
			}
		}
	}
	
	if( p2_keuze == 0 ){	
		if(sw_steen_p2.read()){
			if(steen_p2 == 0){
				hwlib::cout << "Player 2 has chosen" << "\n";
				hwlib::wait_ms(10);
				steen_p2 = 1;
				p2_keuze = 1;
			}
		}
		
		if(sw_papier_p2.read()){
			if(papier_p2 == 0){
				hwlib::cout << "Player 2 has chosen" << "\n";
				hwlib::wait_ms(10);
				papier_p2 = 1;
				p2_keuze = 1;
			}
		}
				
		if(sw_schaar_p2.read()){
			if(schaar_p2 == 0){
				hwlib::cout << "Player 2 has chosen" << "\n";
				hwlib::wait_ms(10);
				schaar_p2 = 1;
				p2_keuze = 1; 
			}
		}
	}
	
	if( (steen_p1 == 1 && schaar_p2 == 1) || (papier_p1 == 1 && steen_p2 == 1) || (schaar_p1 == 1 && papier_p2 == 1) ||
        (steen_p2 == 1 && schaar_p1 == 1) || (papier_p2 == 1 && steen_p1 == 1) || (schaar_p2 == 1 && papier_p1 == 1) ){
		hwlib::xy xy0(6, 1); hwlib::xy xy1(5, 1); hwlib::xy xy2(4, 1); hwlib::xy xy3(3,1 ); hwlib::xy xy4(2, 2); hwlib::xy xy5(1, 3); hwlib::xy xy6(2, 4);
		hwlib::xy xy7(3, 4); hwlib::xy xy8(1, 5); hwlib::xy xy9(2, 6); hwlib::xy xy10(3, 7); hwlib::xy xy11(4, 7); hwlib::xy xy12(5,7); hwlib::xy xy13(6,7);
		ht.setPixel(xy0); ht.setPixel(xy1); ht.setPixel(xy2); ht.setPixel(xy3); ht.setPixel(xy4); ht.setPixel(xy5); ht.setPixel(xy6); ht.setPixel(xy7);
		ht.setPixel(xy8); ht.setPixel(xy9); ht.setPixel(xy10); ht.setPixel(xy11); ht.setPixel(xy12); ht.setPixel(xy13);
		
	for(int x=1; x < 6; x++){
		hwlib::xy xy14(x, 9); hwlib::xy xy15(x, 10);
		ht.setPixel(xy14); ht.setPixel(xy15);
	}
	
	hwlib::xy xy35(7, 9); hwlib::xy xy36(7, 10);
	ht.setPixel(xy35); ht.setPixel(xy36);
	
	for(int x = 1; x < 8; x++){
		hwlib::xy xy16(x, 12); hwlib::xy xy17(x, 16);
		ht.setPixel(xy16); ht.setPixel(xy17);
	}
	
	hwlib::xy xy18(6, 13); hwlib::xy xy19(5, 14); hwlib::xy xy20(4,15); hwlib::xy xy21(3, 16); 
	ht.setPixel(xy18); ht.setPixel(xy19); ht.setPixel(xy20); ht.setPixel(xy21);
	hwlib::xy xy22(1, 19); hwlib::xy xy23(1, 20); hwlib::xy xy24(2, 18); hwlib::xy xy25(2, 21); hwlib::xy xy26(3, 21); hwlib::xy xy27(4, 18); 
	hwlib::xy xy28(4, 19); hwlib::xy xy29(4,20); hwlib::xy xy30(5, 18); hwlib::xy xy31(6, 18); hwlib::xy xy32(7, 19); hwlib::xy xy33(7, 20);
	hwlib::xy xy34(6, 21); hwlib::xy xy37(7, 1); hwlib::xy xy38(7, 7); hwlib::xy xy39(4, 4);
	ht.setPixel(xy22); ht.setPixel(xy23); ht.setPixel(xy24); ht.setPixel(xy25); ht.setPixel(xy26); ht.setPixel(xy27); ht.setPixel(xy28); ht.setPixel(xy29); ht.setPixel(xy30);
	ht.setPixel(xy31); ht.setPixel(xy32); ht.setPixel(xy33); ht.setPixel(xy34); ht.setPixel(xy37); ht.setPixel(xy38); ht.setPixel(xy39);
	
	for(int x=9; x < 15; x++){
		hwlib::xy xy40(x, 1);
		ht.setPixel(xy40);
	}
	// P 
	hwlib::xy xy41(12, 2); hwlib::xy xy42(12, 3); hwlib::xy xy43(13, 4); hwlib::xy xy44(14,4); hwlib::xy xy45(15, 2); hwlib::xy xy46(15,3);
	ht.setPixel(xy41); ht.setPixel(xy42); ht.setPixel(xy43); ht.setPixel(xy44); ht.setPixel(xy45); ht.setPixel(xy46);
	
	if( (steen_p1 == 1 && schaar_p2 == 1) || (papier_p1 == 1 && steen_p2 == 1) || (schaar_p1 == 1 && papier_p2 == 1) ){
	// 1
	hwlib::xy xy47(14, 6); hwlib::xy xy48(15,7); hwlib::xy xy49(10, 7); hwlib::xy xy50(10, 9); 
	ht.setPixel(xy47); ht.setPixel(xy48); ht.setPixel(xy49); ht.setPixel(xy50);
	for(int x = 10; x < 16; x++){
		hwlib::xy xy51(x, 8);
		ht.setPixel(xy51);
	}
	}
	else{
		// 2
		hwlib::xy xy51(14, 7); hwlib::xy xy52(15,8); hwlib::xy xy53(15, 9); hwlib::xy xy54(15, 9); hwlib::xy xy55(14, 10); hwlib::xy xy56(13, 9);
		hwlib::xy xy57(12, 8); hwlib::xy xy58(11, 7);
		ht.setPixel(xy51); ht.setPixel(xy52); ht.setPixel(xy53); ht.setPixel(xy54); ht.setPixel(xy55); ht.setPixel(xy56); ht.setPixel(xy57); ht.setPixel(xy58);
	for(int y = 6; y <= 10; y++){
		hwlib::xy xy59(10, y);
		ht.setPixel(xy59);
	}
		
	}
	ht.flush();
	hwlib::wait_ms(100);
}
else if( ( steen_p1 == 1 && steen_p2 == 1) || ( papier_p1 == 1 && papier_p2 == 1) || (schaar_p1 == 1 && schaar_p2 == 1) ){
	// D
	for(int x = 4; x < 12; x++){
		hwlib::xy xy60(x, 1);
		ht.setPixel(xy60);
	}
	hwlib::xy xy61(11, 2); hwlib::xy xy62(11, 3); hwlib::xy xy63(10, 4); hwlib::xy xy64(9, 5); hwlib::xy xy65(8,5); hwlib::xy xy66(7,5); hwlib::xy xy67(6, 5);
	hwlib::xy xy68(5, 4); hwlib::xy xy69(4,3); hwlib::xy xy70(4,2);
	ht.setPixel(xy61); ht.setPixel(xy62); ht.setPixel(xy63); ht.setPixel(xy64); ht.setPixel(xy65); ht.setPixel(xy66); ht.setPixel(xy67); ht.setPixel(xy68); 
	ht.setPixel(xy69); ht.setPixel(xy70); 
	// R
	for(int x = 4; x < 11; x++){
		hwlib::xy xy71(x, 7);
		ht.setPixel(xy71);
	}
	hwlib::xy xy72(9, 8); hwlib::xy xy73(9,9); hwlib::xy xy74(8,9); hwlib::xy xy75(8,10); hwlib::xy xy76(7, 10);
	ht.setPixel(xy72); ht.setPixel(xy73); ht.setPixel(xy74); ht.setPixel(xy75); ht.setPixel(xy76);
	
	// A 
	for(int x = 4; x < 9; x++){
		hwlib::xy xy77(x, 12); hwlib::xy xy78(x, 16);
		ht.setPixel(xy77); ht.setPixel(xy78);
	}
	hwlib::xy xy79(9, 13); hwlib::xy xy80(10, 14); hwlib::xy xy81(9, 15); hwlib::xy xy82(7, 13); hwlib::xy xy83(7, 14); hwlib::xy xy84(7, 15); 
	ht.setPixel(xy79); ht.setPixel(xy80); ht.setPixel(xy81); ht.setPixel(xy82); ht.setPixel(xy83); ht.setPixel(xy84);
	// W
	for(int x = 5; x < 11; x++){
		hwlib::xy xy85(x, 18); hwlib::xy xy86(x, 22);
		ht.setPixel(xy85); ht.setPixel(xy86);
	}
	hwlib::xy xy87(4, 19); hwlib::xy xy88(4, 21); hwlib::xy xy89(5, 20); hwlib::xy xy90(6, 20);
	ht.setPixel(xy87); ht.setPixel(xy88); ht.setPixel(xy89); ht.setPixel(xy90);
	ht.flush();
}
else{
	ht.clear();
}
	}
	}
	



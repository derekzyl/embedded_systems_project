#include<reg52.h>

sbit RS=P2^0;
sbit RW=P2^1;
sbit E=P2^2;

void send_command(unsigned int command_value);
void send_data(unsigned int data_value);
void ms_delay(unsigned int time);



void main(){
    send_command(0x38 );
    send_command(0x0E);
    send_command(0X1);

    while(1){
    send_data('d');
			ms_delay(1000);
    send_data('e');
				ms_delay(1000);
    send_data('r');
				ms_delay(1000);
    send_data('e');
				ms_delay(1000);
    send_data('k');
    send_command(0xC0);

    send_data('o');
				ms_delay(1000);
    send_data('g');
    break;


}
}  

void send_command(unsigned int command_value){
    P1 = command_value;
    RW = 0;
    RS =0;
    E =1;
    ms_delay(10);
    E = 0;
}


void send_data(unsigned int data_value){
    P1 = data_value;
    RW = 0;
    RS =1;
    E =1;
    ms_delay(10);
    E = 0;
}

void ms_delay(unsigned int time){
	unsigned int i, j;
    for(i=0; i<time; i++){
        for(j=1; j<113; j++);
    }
}

#include<reg52.h>





void main (){
     TMOD = 0X01;
     while(1){
         P1 =0XFF;
         ms-
     }


     void ms_delay (){
         TLO = 0X00;
         THO = 0X0EE;
         TRO =1;
         while(!TFO);
         TRO =0;
         TFO=0;
     }
}
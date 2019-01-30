#include "cansend.hpp"
#include <unistd.h>

void cansend(int s, int* out_can) 
{
  char buffer[3][5] = {0};
  for(int i = 0; i < 3; i++) {
    out_can[5*i+4] = static_cast<unsigned int>(out_can[5*i+4]);
    buffer[i][0] = (out_can[5*i+1]&0xFFF)>>4;
    buffer[i][1] = ((out_can[5*i+1]&0xF)<<4)+((out_can[5*i+3]&0xFFF)>>8);  
    buffer[i][2] =  out_can[5*i+3]&0xFF;
    buffer[i][3] = ((out_can[5*i]&0x3)<<6)+((out_can[5*i+2]&0x7FF)>>5);
    buffer[i][4] = ((out_can[5*i+2]&0x1F)<<3)+(out_can[5*i+4]&0x7);
        
    printf("buffer[0]: --- %x\n", (unsigned char)buffer[i][0]);
    printf("buffer[1]: --- %x\n", (unsigned char)buffer[i][1]);
    printf("buffer[2]: --- %x\n", (unsigned char)buffer[i][2]);
    printf("buffer[3]: --- %x\n", (unsigned char)buffer[i][3]);
    printf("buffer[4]: --- %x\n", (unsigned char)buffer[i][4]);	        
       
    can_send_frame(s, i, buffer[i]);
    usleep(300);     
  }
}

int can_init(int& s )
{
	struct sockaddr_can addr;    
	struct ifreq ifr;           
	const char* ifname = "can1";

	/* open socket */
	if (( s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Error while opening socket...\n");
		return 1;
	}
	
	strcpy(ifr.ifr_name, ifname); 
	ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);  
	if (!ifr.ifr_ifindex) {
		perror("if_nametoindex error...\n");
		return 1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;                        
	addr.can_ifindex = ifr.ifr_ifindex;              

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Error in socket bind...\n");
		return 1;
	}
	return 0;

}

void can_send_frame( int s,int i, char* buffer )//can-send-one-frame...
{
	struct can_frame frame;
  if(i == 0)
    frame.can_id = 0x30;
  if(i == 1)
    frame.can_id = 0x31;
  if(i == 2)
    frame.can_id = 0x32;
	frame.can_dlc = 8;

	frame.data[0] = *buffer;
	frame.data[1] = *(buffer+1);
	frame.data[2] = *(buffer+2);
	frame.data[3] = *(buffer+3);
	frame.data[4] = *(buffer+4);
        
	if (write(s, &frame, sizeof(struct can_frame)) != CAN_MTU) {
		perror("write error...\n");
		return ;
	}
}

void canread(int& s) {
	struct sockaddr_can addr;    
	struct ifreq ifr;           
	struct can_frame frame;
  struct can_filter rfilter[1];

	if (( s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Error while opening socket...\n");
	}
	
	strcpy(ifr.ifr_name, "can1"); 
	/* ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);  
	if (!ifr.ifr_ifindex) {
		perror("if_nametoindex error...\n");
	}*/ 

  ioctl(s, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;                        
	addr.can_ifindex = ifr.ifr_ifindex;              

	bind(s, (struct sockaddr *)&addr, sizeof(addr));

  rfilter[0].can_id = 0x3E9;
  rfilter[0].can_mask = CAN_SFF_MASK;
  setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter)); //disable filter. 

  int nbytes = read(s, &frame, sizeof(frame));

  if (nbytes > 0) {
    printf("read successfully!\n");
    printf("id=0X%X  DLC=%d  (unsigned char)frame.data[0]:---%x\n", frame.can_id, frame.can_dlc, (unsigned char)frame.data[1]);
  }
}








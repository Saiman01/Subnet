#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> 
#include <arpa/inet.h>
#include <stdint.h>
#include <math.h>

int main(){
	struct in_addr inaddr; 
	uint32_t maxHost;//store the host format of max IP Address
	uint32_t ipAddrHost; //store the host format of input IP Address
	char subnetMask[INET_ADDRSTRLEN]; //store the subnet mask for output
	char networkAddress[INET_ADDRSTRLEN]; //store network address for output
	char broadcastAddress[INET_ADDRSTRLEN]; //store broadcast address for output
	char hostIPRange1[INET_ADDRSTRLEN]; //store the range of IP address
	char hostIPRange2[INET_ADDRSTRLEN]; //store range for IP address
	char max[]="255.255.255.255"; //max IP adress
	char ipAddr[32]; //store input IP address
	int subnet_part; //store the subnet part of input

	//converting max IP address into network form
	if(( inet_pton(AF_INET, max, &inaddr) <= 0)) {
	        printf("inet_pton fail\n");
                exit(1);		
	};
	
	//converting the max IP address into host format
	maxHost = ntohl(inaddr.s_addr);  

	//user input
	printf("IP address: "); 
	scanf("%s", ipAddr); 
	
	//user input
	printf("Subnet part: "); 
	scanf("%d", &subnet_part); 
	
	//error checking
	if (subnet_part <0) {
		printf("The input cannot be negative\n");
	       exit(1);	
	}

	//error checking
        if (subnet_part == 0 || subnet_part > 30) {
                printf("There are no usable host for 0, 31, or 32.\n");
        	exit(1); 
	}

	//calculating and converting subnet mask into network format
	uint32_t subnetMaskHost = maxHost << (32-subnet_part); 
	uint32_t subnetMaskNet = htonl(subnetMaskHost); 

	//converting the subnet mask form network format into string format
	inaddr.s_addr = subnetMaskNet; 
	if((inet_ntop(AF_INET, &inaddr,subnetMask,sizeof(subnetMask)) == NULL)) {
                printf("inet_pton fail\n");
		exit(1); 
        };

	//output subnet mask
	printf("Subnet mask:   %s \n", subnetMask); 


	//converting the input IP address into network format
	if(( inet_pton(AF_INET, ipAddr, &inaddr) <= 0)) {
               	printf("inet_pton fail\n");
		exit(1); 
        };

	//converting the input IP address into host format
	ipAddrHost = ntohl(inaddr.s_addr);

	//error checking for class C IP address
	if(ipAddrHost<3232235520 || ipAddrHost > 3232301055) {
		printf("The ip address provided does not fall in the range of class C IP addresses.\n"); 
		exit (1); 
	}
	//caculating network address
	uint32_t networkAddr = (ipAddrHost & subnetMaskHost);  
	
	//converting the network address into network format
	uint32_t networkAddrNet = htonl(networkAddr); 

	//converting the network address into printabel form
	inaddr.s_addr = networkAddrNet; 
	if((inet_ntop(AF_INET, &inaddr,networkAddress,sizeof(networkAddress)) == NULL)) {
                printf("inet_pton fail\n");
                exit(1);
        };
	
	//printing network address
	printf("Network Address:   %s \n", networkAddress);
	
	//cacluating broadcast address
	uint32_t broadcastAddr = (ipAddrHost | ~subnetMaskHost);  

	//converting the broadcast address into network form
        uint32_t broadcastAddrNet = htonl(broadcastAddr);

	//converting the broadcast address into printable form
        inaddr.s_addr = broadcastAddrNet;
        if((inet_ntop(AF_INET, &inaddr,broadcastAddress,sizeof(broadcastAddress)) == NULL)) {
                printf("inet_pton fail\n");
                exit(1);
        };

	//printing broadcast address
        printf("Broadcast Address:   %s \n", broadcastAddress);

	//calculating the usable host IP range and converting them into printable form
	uint32_t usableRange1 = networkAddr + 1; //low range

        uint32_t usableRangeNet1 = htonl(usableRange1);

        inaddr.s_addr = usableRangeNet1;

        if((inet_ntop(AF_INET, &inaddr,hostIPRange1,sizeof(hostIPRange1)) == NULL)) {
                printf("inet_pton fail\n");
                exit(1);
        };
	
	uint32_t usableRange2 = broadcastAddr - 1; //high range
       	
	uint32_t usableRangeNet2 = htonl(usableRange2);

        inaddr.s_addr = usableRangeNet2;

        if((inet_ntop(AF_INET, &inaddr,hostIPRange2,sizeof(hostIPRange2)) == NULL)) {
                printf("inet_pton fail\n");
                exit(1);
        };

	//printing usable host IP range
        printf("Usable host IP range:   %s - %s \n", hostIPRange1, hostIPRange2);
	
	//calculating and printing the number of usable host and host part
	int result = pow(2,(32-subnet_part))- 2;
	printf("Number of usable host: %d \n", result);
	printf("Host Part: %d\n", (32-subnet_part)); 	
	
	return 0;
}

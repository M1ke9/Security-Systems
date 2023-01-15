
#include <stdio.h>
#include <pcap.h>
#include <pcap/pcap.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 
#include <arpa/inet.h> 
#include <net/ethernet.h>
#include <netinet/udp.h>	
#include <netinet/tcp.h>	
#include <netinet/ip.h>	

int nf=0, tcp_f=0, udp_f=0, total_others=0, total_packets=0, total_tcps=0, total_udps=0, total_bytes_tcp=0, total_bytes_udp=0;

int check=0;


pcap_t* handle;
int retnum=0;

typedef struct net_flow{
	char source_ip[INET_ADDRSTRLEN];
	char dest_ip[INET_ADDRSTRLEN];
	unsigned int protocol;
	unsigned int sport;
	unsigned int dport;

	struct net_flow* next;

}flow;


typedef struct retransmission{

	char source_ip[INET_ADDRSTRLEN];
	char dest_ip[INET_ADDRSTRLEN];
	int payload;
	struct tcphdr *tcp;

	struct retransmission* next;

}retr;


flow* net = NULL;
retr* retrans_glb = NULL;
retr* current_flow=NULL;

int c=0;



void statistics(){

	printf("\nTotal number of network flows captured​: %d\n",nf);
	printf("Number of TCP network flows captured: %d\n",tcp_f);
	printf("Number of UDP network flows captured: %d\n",udp_f);
	printf("Total number of packets received: %d\n",total_packets);
	printf("Total number of TCP packets received: %d\n",total_tcps);
	printf("Total number of UDP packets received: %d\n",total_udps);
	printf("Total bytes of TCP packets received: %d\n",total_bytes_tcp);
	printf("Total bytes of UDP packets received: %d\n",total_bytes_udp);
	
}



flow * in_list(flow * net, char* ips, char* ipd, int pr, unsigned int sp, unsigned int dp){
	flow* tmp = net;
    while(tmp != NULL){

        if(tmp->protocol == pr && tmp->sport==sp && tmp->dport==dp && strcmp(tmp->source_ip,ips)==0 && strcmp(tmp->dest_ip,ipd)==0 ){
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

void new_net(flow * net, char* ips, char* ipd, int pr, unsigned int sp, unsigned int dp){

	flow* new = (flow*)malloc(sizeof(flow));
	flow* tmp = net;

	while(tmp->next != NULL){
		tmp = tmp->next;
	}

	tmp->next = new;
	memcpy(new->source_ip,ips,INET_ADDRSTRLEN);
	memcpy(new->dest_ip,ipd,INET_ADDRSTRLEN);
	new->protocol = pr;
	new->sport    = sp;
	new->dport    = dp;
	new->next 	  = NULL;

	++nf;
	if(new->protocol == IPPROTO_TCP)
		++tcp_f;
	else if (new->protocol == IPPROTO_UDP)
		++udp_f;

}

retr* add_to_current_flow(retr* head ,retr* new){

	if(new==NULL)
	 return head;
	
	if(head==NULL){
		head=new; 
		return head;}

	else{
		
		retr* temp=head;
		while(temp->next!=NULL)		
			temp=temp->next;
		
		temp->next=new;
	}
	return head;
}	


void tcp_info(const u_char * packet, int size)
{

	flow* tmp_flow = NULL;
	char s_ip[INET_ADDRSTRLEN];
	char d_ip[INET_ADDRSTRLEN];
	unsigned short ip_len;
	const struct ip * iphead = (struct ip *)(packet  + sizeof(struct ethhdr) );

	struct ether_header *eptr = (struct ether_header*)packet;

	if (ntohs(eptr->ether_type) != ETHERTYPE_IP && ntohs(eptr->ether_type) != ETHERTYPE_IPV6) {
		printf("Not an IPv4 or IPv6 packet. Skipped\n");
		return;
	}
	ip_len = iphead->ip_hl*4;
	
	inet_ntop(AF_INET, &(iphead->ip_src), s_ip, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &(iphead->ip_dst), d_ip, INET_ADDRSTRLEN);
	

	struct tcphdr *tcph=(struct tcphdr*)(packet + ip_len + sizeof(struct ethhdr));

	int header_size =  sizeof(struct ethhdr) + ip_len + tcph->doff*4;
	int payload_length = size-header_size;
	total_bytes_tcp = total_bytes_tcp + size;

	if (net==NULL){

		tmp_flow=(flow*)malloc(sizeof(flow));
		memcpy(tmp_flow->source_ip,s_ip,INET_ADDRSTRLEN);
		memcpy(tmp_flow->dest_ip,d_ip,INET_ADDRSTRLEN);

		tmp_flow->protocol = (unsigned int)iphead->ip_p;
		tmp_flow->sport    = ntohs(tcph->source);
		tmp_flow->dport	   = ntohs(tcph->dest);
		tmp_flow->next 	   = NULL;
		net = tmp_flow;

		++nf;
		++tcp_f;


	}else{
		if((tmp_flow = in_list(net,s_ip,d_ip,(unsigned int)iphead->ip_p,ntohs(tcph->source),ntohs(tcph->dest)))==NULL)
			new_net(net,s_ip,d_ip,(unsigned int)iphead->ip_p,ntohs(tcph->source),ntohs(tcph->dest));
		}

if(check==1)
{
	printf(" |Source IP: %s| |Dest. IP: %s| |Protocol: TCP| ",s_ip,d_ip);
	printf("|Source Port: %u| |Dest. Port: %u| |Header Length: %d| |Payload Length: %d|\n",ntohs(tcph->source),ntohs(tcph->dest),(unsigned int)tcph->doff*4, payload_length);
	
}

else{
	char *filename = "log.txt";

    // open the file for writing
    FILE *fp = fopen(filename, "a");
    if (fp == NULL)
    {
        printf("Error opening the file %s", filename);
        return -1;
    }
    // write to the text file

        fprintf(fp," |Source IP: %s| |Dest. IP: %s| |Protocol: TCP| ",s_ip,d_ip);
		fprintf(fp,"|Source Port: %u| |Dest. Port: %u| |Header Length: %d| |Payload Length: %d|\n",ntohs(tcph->source),ntohs(tcph->dest),(unsigned int)tcph->doff*4, payload_length);
	 

    // close the file
    fclose(fp);
}


	return;
}


void udp_info(const u_char * packet, int size)
{

	flow* tmp_flow = NULL;
	char s_ip[INET_ADDRSTRLEN];
	char d_ip[INET_ADDRSTRLEN];
	unsigned short ip_len;

	const struct ip * iphead = (struct ip *)(packet  + sizeof(struct ethhdr) );
	struct ether_header *eptr = (struct ether_header*)packet;

	if (ntohs(eptr->ether_type) != ETHERTYPE_IP && ntohs(eptr->ether_type) != ETHERTYPE_IPV6) {
		printf("Not an IPv4 or IPv6 packet. Skipped\n");
		return;
	}
	ip_len = iphead->ip_hl*4;
	
	inet_ntop(AF_INET, &(iphead->ip_src), s_ip, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &(iphead->ip_dst), d_ip, INET_ADDRSTRLEN);
	

	struct udphdr *udph=(struct udphdr*)(packet + ip_len + sizeof(struct ethhdr));

	int header_size =  sizeof(struct ethhdr) + ip_len +sizeof(udph);
	int payload_length = size-header_size;

	total_bytes_udp = total_bytes_udp + size;

	if (net==NULL){

		tmp_flow=(flow*)malloc(sizeof(flow));
		memcpy(tmp_flow->source_ip,s_ip,INET_ADDRSTRLEN);
		memcpy(tmp_flow->dest_ip,d_ip,INET_ADDRSTRLEN);

		tmp_flow->protocol = (unsigned int)iphead->ip_p;
		tmp_flow->sport    = ntohs(udph->source);
		tmp_flow->dport	   = ntohs(udph->dest);
		tmp_flow->next 	   = NULL;
		net = tmp_flow;

		++nf;
		++udp_f;

	}else{
		if((tmp_flow = in_list(net,s_ip,d_ip,(unsigned int)iphead->ip_p,ntohs(udph->source),ntohs(udph->dest)))==NULL)
			new_net(net,s_ip,d_ip,(unsigned int)iphead->ip_p,ntohs(udph->source),ntohs(udph->dest));
	}
    if(check==1)
	{
	printf(" |Source IP: %s| |Dest. IP: %s| |Protocol: UDP| ",s_ip,d_ip);
	printf("|Source Port: %u| |Dest. Port: %u| |Header Length: %d| |Payload Length: %d|\n",ntohs(udph->source),ntohs(udph->dest),(unsigned int)udph->len, payload_length);
	}
else{
    // open the file for writing
    FILE *fp = fopen("log.txt", "a"); 
	fprintf(fp," |Source IP: %s| |Dest. IP: %s| |Protocol: UDP| ",s_ip,d_ip);
	fprintf(fp,"|Source Port: %u| |Dest. Port: %u| |Header Length: %d| |Payload Length: %d|\n",ntohs(udph->source),ntohs(udph->dest),(unsigned int)udph->len, payload_length);
	fclose(fp);
}
	
	return;
}


void packet_handler(unsigned char *args, const struct pcap_pkthdr *header, const unsigned char *packet){


	int size = header->caplen;

	struct iphdr *ip_header = (struct iphdr*)(packet + sizeof(struct ethhdr));
	++total_packets;
	switch (ip_header->protocol) 
	{
		case IPPROTO_TCP: 
			++total_tcps;
			tcp_info(packet , size);

			break;
		
		case IPPROTO_UDP: 
			++total_udps;
			udp_info(packet , size);

			break;
		default: 
			++total_others;
			break;		
	}

}


void packet_capture(char* file ){

	char errbuf[PCAP_ERRBUF_SIZE];
	bpf_u_int32 mask;		
	bpf_u_int32 net;		
	int timeout = 1000;


	pcap_t* descr = pcap_open_offline(file,errbuf);
	if(descr != NULL){
       
		pcap_loop(descr,-1,packet_handler,NULL);

		statistics();

			retr* t;
		while(retrans_glb!=NULL){
			t=retrans_glb->next;
			free(retrans_glb);
			retrans_glb=t;
		}
	}else 
		printf("Error opening file!\n");

		

	return;
}

void terminate_process(int signum){

	pcap_breakloop(handle);
	pcap_close(handle);

}


void network_device(char * dev,char *filter){

	char errbuf[PCAP_ERRBUF_SIZE];
	bpf_u_int32 mask;		
	bpf_u_int32 net;		
	int timeout = 1000;
	struct bpf_program fp;

    
    /* Find a device */
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
        printf("Error finding device: %s\n", errbuf);
		mask = 0;
		net  = 0;
    }


   handle = pcap_open_live(dev,BUFSIZ,0,timeout,errbuf);   

    if(handle == NULL){
        printf("Error for pcap_open_live(): %s\n",errbuf);
		return ;
	}

	if(pcap_compile(handle,&fp,filter,0,net) == -1){
		fprintf(stderr,"Error calling pcap compile \n ");
		exit(1);
	}

	if(pcap_setfilter(handle,&fp) == -1){
		fprintf(stderr,"Error setting filter. \n");
		exit(1);
	}


	signal(SIGINT, terminate_process);
	pcap_loop(handle,timeout,packet_handler,NULL);

	statistics();

	pcap_freecode(&fp);

	return;
}

void find_dev(){

	pcap_if_t *alldevsp , *device;

	char errbuf[PCAP_ERRBUF_SIZE] , *devname ;
	int count = 1 , n;
	
	//First get the list of available devices
	printf("Finding available devices ... ");
	
	if(pcap_findalldevs(&alldevsp , errbuf))
	{
		printf("Error finding devices : %s" , errbuf);
		exit(1);
	}
	
	//Print the available devices
	printf("\nAvailable Devices are :\n");
	for(device = alldevsp ; device != NULL ; device = device->next)
	{
		printf("%d. %s - %s\n" , count , device->name , device->description);
		count++;
	}

	return;
}



void
usage(void)
{
	printf(
	       "\n"
	       "Usage:\n\n"
		   "Options:\n"
		   "-i, Network interface name \n"
		   "-r, Packet capture file name\n"
		   "-h, Help message\n\n"
		   );

	exit(1);
}
//global variable


int  main(int argc, char **argv)
{
	
	int ch;
	char device[1024], file[1024];
	char *filter = NULL;
	


	

	while ((ch = getopt(argc, argv, "irfh")) != -1) {
		switch (ch) {		
		case 'i':

			find_dev();
			printf("Select the one interface that you wish to monitor: \n");
	  		scanf("%s",device);
			network_device(device,filter);

			break;

		case 'r': 
           check=1;
			printf("Enter the file name that you wish to monitor: \n");
	  		scanf("%s",file);
			packet_capture(file); 

			break;

		case 'f':
        
		   filter=strdup(optarg);

		   break;
		
		case 'h':   
		    usage();
		    break;
		default:
			usage();
		}

	}

	
	return 0;
}
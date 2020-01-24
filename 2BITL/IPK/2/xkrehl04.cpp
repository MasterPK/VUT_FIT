/** 
 * Name:  Projekt IPK 2
 * File:  xkrehl04.cpp
 * Autor: Petr Křehlík (xkrehl04)
 * Date:  16.4.2019
 */

#include <iostream>
#include <string>
#include <regex>
#include <pcap/pcap.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ether.h>
#include <byteswap.h>
#include <time.h>

using namespace std;

// Max packet length
#define PCKT_LEN 8192

struct pseudoTCPPacket  
{
  uint32_t srcAddr;
  uint32_t dstAddr;
  uint8_t zero;
  uint8_t protocol;
  uint16_t TCP_len;
};

/**
 * @brief  Test Big/Little Endian, used from: https://helloacm.com/how-to-find-out-whether-a-machine-is-big-endian-or-little-endian-in-cc/
 * @retval Result of test
 */
int TestByteOrder() 
{
    short int word = 0x0001;
    char *b = (char *)&word;
    return (b[0] ? LITTLE_ENDIAN : BIG_ENDIAN);
}


#define UDP 0
#define TCP 1
#define OPEN 0
#define CLOSED 1
#define FILTERED 2
/**
 * @brief  Print formatted output of current port state
 * @param  port
 * @param  protocol
 * @param  state
 * @retval None
 */
void print_port_state(int port, int protocol, int state)
{
    string proto;
    if(protocol==0)
    {
        proto="udp";
    }
    else if(protocol==1)
    {
        proto="tcp";
    }
    string stat;
    if(state==0)
    {
        stat="open";
    }
    else if(state==1)
    {
        stat="closed";
    }
    else if(state==2)
    {
        stat="filtered";
    }
    string tab="\t\t";
    string output=to_string(port)+"/"+proto+tab+stat;
    cout << output << endl;

}

/**
 * @brief  Debugging function, print string to stderr 
 * @param  text: 
 * @retval None
 */
void debug(string text)
{
    cerr << text << endl;
}

/**
 * @brief  Print err message to stderr and exit program with code 
 * @param  code: 
 * @param  error: 
 * @retval None
 */
void error(int code, string error)
{
    cerr <<"Error code:"<<code << "..." << error << endl;
    exit(code);
}

/**
 * @brief  Checksum function for UDP from https://www.tenouk.com/Module43a.html
 * @param  *buf: buffer with packet
 * @param  nwords
 * @retval Checksum
 */
unsigned short csum(unsigned short *buf, int nwords)
{      
    unsigned long sum;
    for(sum=0; nwords>0; nwords--)
            sum += *buf++;
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);

}

/**
 * @brief  Checksum function for TCP with pseudoheader from https://github.com/rbaron/raw_tcp_socket/blob/master/raw_tcp_socket.c?fbclid=IwAR1rlNNONqg5qHdfT4ue1NQcMV6-ChRjWNzGDf5DEhbTYqeCVcXgMIDQHBM
 * @param  *ptr: pointer with packet
 * @param  nbytes
 * @retval 
 */
unsigned short csum_tcp(unsigned short *ptr,int nbytes) 
{

    long sum;
    unsigned short oddbyte;
    short answer;


    sum=0;
    while(nbytes>1) {
    sum+=*ptr++;
    nbytes-=2;
    }
    if(nbytes==1) {
    oddbyte=0;
    *((u_char*)&oddbyte)=*(u_char*)ptr;
    sum+=oddbyte;
    }

    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;

    return(answer);
}


/**
 * @brief  Get random number in interval
 * @note   
 * @param  lower: Lower boundary
 * @param  upper: Upper boundary
 * @retval 
 */
int Randoms(int lower, int upper) 
{ 
    return (rand() % (upper - lower + 1)) + lower;
} 

/**
 * @brief  Get IP from hostname, if IP entered, only return back
 * @param  hostname: IP or hostname
 * @retval IP address in numbers-and-dots notation
 */
string get_ip_from_hostname(const char* hostname)
{
    struct addrinfo hints, *results, *item;
    char ipstr[INET_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(hostname, NULL, &hints, &results) != 0)
    {
        cerr << "Cant resolve hostname or IP address is in bad format \""<< hostname << "\": " << endl;
        exit(4);
    }
    void* addr;

    if(results->ai_family == AF_INET) 
    {
        struct sockaddr_in* ipv4 = (struct sockaddr_in*)results->ai_addr;
        addr = &(ipv4->sin_addr);
    }

    inet_ntop(results->ai_family, addr, ipstr, sizeof ipstr);

    freeaddrinfo(results);
    return ipstr;
 
}

/**
 * @brief  Get interface default IP address by interface name
 * @note   Uses socket and ioctl to determine IP
 * @param  interface: Interface name
 * @retval String of interface name
 */
string get_ip_interface(string interface)
{
    struct ifreq ifr;
    size_t if_name_len=interface.length();
    if (if_name_len<sizeof(ifr.ifr_name)) {
        memcpy(ifr.ifr_name,interface.c_str(),if_name_len);
        ifr.ifr_name[if_name_len]=0;
    } else {
        error(99,"Name of interface is too long");
    }
    
    int fd=socket(AF_INET,SOCK_DGRAM,0);
    if (fd==-1) {
        close(fd);
        error(1,strerror(errno));
    }
    if (ioctl(fd,SIOCGIFADDR,&ifr)==-1) {
        int temp_errno=errno;
        close(fd);
        error(1,strerror(temp_errno));
    }
    close(fd);

    struct sockaddr_in* ipaddr = (struct sockaddr_in*)&ifr.ifr_addr;
    return inet_ntoa(ipaddr->sin_addr);
}

/**
 * @brief Find first non loopback interface if argument -i is not specified
 * @retval name of interface
 */
string get_default_interface()
{
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1) 
    {
        error(99,"Unknow error while processing interface");
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_PACKET) continue;
        string ip=get_ip_interface(ifa->ifa_name);
        if(ip=="127.0.0.1" || ip=="localhost") continue;
        return ifa->ifa_name;
    }
}

/**
 * @brief  Test all specified UDP ports on target IP address and print sumarization of their state
 * @note   Creating own packet from scratch and send via RAW socket
 * @param  ip_address_source: 
 * @param  ip_address_target: 
 * @param  ports: 
 * @param  interface: 
 * @retval None
 */
void udp_test_ipv4(string ip_address_source, string ip_address_target, vector<int> ports, string interface)
{
    char errbuf[PCAP_ERRBUF_SIZE];

    for (int &port : ports)
    {  
        int sd;

        char buffer[PCKT_LEN];

        struct iphdr  *ip = (struct iphdr  *) buffer;
        struct udphdr  *udp = (struct udphdr  *) (buffer + sizeof(struct iphdr));
        struct sockaddr_in sin, din;
        int one = 1;
        const int *val = &one;

        memset(buffer, 0, PCKT_LEN);

        // Socket, using IPv4, RAW sending over UDP
        sd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
        if(sd < 0)
        {
            error(2,"Error while creating socket");

        }
    
        // Fill up target data
        din.sin_family = AF_INET;
        din.sin_port = htons(port);
        din.sin_addr.s_addr = inet_addr(ip_address_target.c_str()); // Convert IP numbers-and-dots notation into binary data in network byte order
        
        // Fill up IP header
        ip->ihl = 5;
        ip->version = 4; // IPv4
        ip->tos = 16;
        ip->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr);
        ip->id = htons(54321);
        ip->ttl = 64; // TTL
        ip->protocol = 17; // UDP

        ip->saddr = inet_addr(ip_address_source.c_str());
        ip->daddr = inet_addr(ip_address_target.c_str());

        // Destination port number
        udp->dest = htons(port);
        udp->len = htons(sizeof(struct udphdr));

        // Calculate the checksum for IP header
        ip->check = csum((unsigned short *)buffer, sizeof(struct iphdr) + sizeof(struct udphdr));

        // Disable auto header completition
        if(setsockopt(sd, IPPROTO_IP,IP_HDRINCL, val, sizeof(one)) < 0)
        {
            error(2,"Error while setting up setsockopt()");
        
        }

        // Init pcap
        pcap* pcap;
        pcap =pcap_create(interface.c_str(),errbuf);
        // Recieve packet as soon as arrive
        pcap_set_immediate_mode(pcap, 1);
        // Scan all traffic
        pcap_set_promisc(pcap, 1); 
        // Timeout if no packet arrives
        pcap_set_timeout(pcap, 500);
        // Enable timeout breaking loop
        pcap_setnonblock(pcap,1,errbuf);
        // Active pcap
        pcap_activate(pcap);

        struct bpf_program filter;
        string filter_exp="ip src host " +ip_address_target + " and dst host " + ip_address_source + " and icmp";
        bpf_u_int32 subnet_mask, ip_cap;
        if (pcap_compile(pcap, &filter, filter_exp.c_str(), 0, ip_cap) == -1) {
            printf("Wrong filter - %s\n", pcap_geterr(pcap));
            exit(3);
        }
        if (pcap_setfilter(pcap, &filter) == -1) {
            printf("Erro while setting pcap filter - %s\n", pcap_geterr(pcap));
            exit(3);
        }
        
        // Because UDP has no response if OPEN, then try it two times to be sure
        bool icmp_response=false;
        for(int count = 0; count <1; count++)
        {
            if(sendto(sd, buffer, ip->tot_len, 0, (struct sockaddr *)&din, sizeof(din)) < 0)
            {
                error(2,"Erro while sending packet");
            }
            else
            {
                struct pcap_pkthdr packet_header;
                const u_char *packet;
                packet=pcap_next(pcap,&packet_header);
                // If no response then try again
                if(packet==NULL)
                {
                    continue;
                }
                else
                {
                    struct ether_header *eth = (struct ether_header*)packet;
                    struct iphdr *ip = (struct iphdr*)(packet + sizeof(struct ether_header));
                    struct icmphdr *icmp = (struct icmphdr*)(packet + sizeof(struct ether_header) + sizeof(struct iphdr));
                    int type,code;
                    if(TestByteOrder()==LITTLE_ENDIAN)
                    {
                        type = bswap_16(ntohs(icmp->type));
                        code = bswap_16(ntohs(icmp->code));
                    }
                    else
                    {
                        type = ntohs(icmp->type);
                        code = ntohs(icmp->code);
                    }
                    
                    if(type==3 && code==3)
                    {
                        icmp_response=true;
                        
                        break; 
                    }
                }
                
            }

        }
        // If there is ICMP response (3,3) port is CLOSED, if no response then we can say its OPEN
        if(icmp_response)
        {
            print_port_state(port,UDP,CLOSED);
        }
        else
        {
            print_port_state(port,UDP,OPEN);
        }
        
        pcap_close(pcap);
        close(sd);
    }
}


/**
 * @brief  Scan target IP address ports over TCP protocol
 * @param  ip_address_source: Source address
 * @param  ip_address_target: Target address
 * @param  ports: Ports to scan
 * @param  interface: Interface for sending
 * @retval None
 */
void tcp_test_ipv4(string ip_address_source, string ip_address_target, vector<int> ports, string interface)
{
    char errbuf[PCAP_ERRBUF_SIZE];

    for (int &port : ports)
    { 
        int sd;
        char buffer[PCKT_LEN];
        struct iphdr  *ip = (struct iphdr  *) buffer;
        struct tcphdr  *tcp = (struct tcphdr  *) (buffer + sizeof(struct iphdr));
        struct sockaddr_in sin, din;
        struct pseudoTCPPacket Ptcp;
        char *pseudo_packet;
        int one = 1;
        const int *val = &one;

        memset(buffer, 0, PCKT_LEN);

        // Socket, using IPv4, RAW sending over TCP
        sd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
        if(sd < 0)
        {
            error(2,"Error while creating socket");
        }
    
        din.sin_family = AF_INET;
        din.sin_port = htons(port);
        din.sin_addr.s_addr = inet_addr(ip_address_target.c_str());
        
        // Fill up IP header
        ip->ihl = 5;
        ip->version = 4; //IPv4
        ip->tos = 16;
        ip->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
        ip->id = htons(54321);
        ip->frag_off = 0;
        ip->ttl = 64; //TTL
        ip->protocol = 6; //TCP

        ip->saddr = inet_addr(ip_address_source.c_str());
        ip->daddr = inet_addr(ip_address_target.c_str());

        // Fill up TCP header
        int src_port=Randoms(32768,61000); //Random sin number
        tcp->source=htons(src_port);
        tcp->dest = htons(port);
        tcp->seq = htonl(rand());
        tcp->ack_seq = 0;
        tcp->doff = 5;
        tcp->syn = 1;
        tcp->ack = 0;
        tcp->window = htons(32767);
        tcp->urg = 0;

        // Fill up pseudo TCP packet
        Ptcp.srcAddr = inet_addr(ip_address_source.c_str());
        Ptcp.dstAddr = inet_addr(ip_address_target.c_str());
        Ptcp.zero = 0;
        Ptcp.protocol = IPPROTO_TCP;
        Ptcp.TCP_len = htons(sizeof(struct tcphdr));
        
        // Checksum for pseudo TCP header, copied from https://github.com/rbaron/raw_tcp_socket/blob/master/raw_tcp_socket.c?fbclid=IwAR1rlNNONqg5qHdfT4ue1NQcMV6-ChRjWNzGDf5DEhbTYqeCVcXgMIDQHBM
        pseudo_packet = (char *) malloc((int) (sizeof(struct pseudoTCPPacket) + sizeof(struct tcphdr)));
        memset(pseudo_packet, 0, sizeof(struct pseudoTCPPacket) + sizeof(struct tcphdr));
        memcpy(pseudo_packet, (char *) &Ptcp, sizeof(struct pseudoTCPPacket));
        memcpy(pseudo_packet + sizeof(struct pseudoTCPPacket), tcp, sizeof(struct tcphdr));
        tcp->check = (csum_tcp((unsigned short *) pseudo_packet, (int) (sizeof(struct pseudoTCPPacket) + sizeof(struct tcphdr))));


        ip->check = csum((unsigned short *)buffer, sizeof(struct iphdr) + sizeof(struct tcphdr));
        // Disable auto header completition
        if(setsockopt(sd, IPPROTO_IP,IP_HDRINCL, val, sizeof(one)) < 0)
        {
            error(2,"Error while setting up socket setsockopt()");
        }

        // Init pcap
        pcap* pcap;
        pcap =pcap_create(interface.c_str(),errbuf);
        // Recieve packet as soon as arrive
        pcap_set_immediate_mode(pcap, 1);
        // Scan all traffic
        pcap_set_promisc(pcap, 1); 
        // Timeout if no packet arrives
        pcap_set_timeout(pcap, 500);
        // Enable timeout breaking loop
        pcap_setnonblock(pcap,1,errbuf);
        // Active pcap
        pcap_activate(pcap);

        // Set filter to capture only packet from target IP and port to this station
        struct bpf_program filter;
        string filter_exp="ip src host " +ip_address_target + " and dst host " + ip_address_source + " and src port "+to_string(port)+" and dst port "+to_string(src_port);
        bpf_u_int32 subnet_mask, ip_cap;
        if (pcap_compile(pcap, &filter, filter_exp.c_str(), 0, ip_cap) == -1) {
            printf("Wrong pcpap filter - %s\n", pcap_geterr(pcap));
            exit(3);
        }
        if (pcap_setfilter(pcap, &filter) == -1) {
            printf("Error while setting pcap filter - %s\n", pcap_geterr(pcap));
            exit(3);
        }
        int count;
        bool icmp_response=false;
        if(sendto(sd, buffer, ip->tot_len, 0, (struct sockaddr *)&din, sizeof(din)) < 0)
        {
            error(2,"Error while sending packet");
        }
        else
        {
            struct pcap_pkthdr packet_header;
            const u_char *packet;
            packet=pcap_next(pcap,&packet_header);
            // If no packet arrived, then try again...
            if(packet==NULL)
            {
                if(sendto(sd, buffer, ip->tot_len, 0, (struct sockaddr *)&din, sizeof(din)) < 0)
                {
                    pcap_close(pcap);
                    error(2,"Error while sending packet");
                }
                else
                {
                    struct pcap_pkthdr packet_header;
                    const u_char *packet;
                    packet=pcap_next(pcap,&packet_header);
                    
                    if(packet==NULL)
                    {
                        // if packet is null after second send, then port is FILTERED
                        print_port_state(port,TCP,FILTERED);
                    }
                    else
                    {
                        // Headers
                        struct ether_header *eth = (struct ether_header*)packet;
                        struct iphdr *ip = (struct iphdr*)(packet + sizeof(struct ether_header));
                        struct tcphdr *tcp = (struct tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct iphdr));
                        // Convert from network byte order to integer, byteswap because of Endianness problem
                        int syn,ack,rst;
                        if(TestByteOrder()==LITTLE_ENDIAN)
                        {
                            syn=bswap_16(ntohs(tcp->syn));
                            ack=bswap_16(ntohs(tcp->ack));
                            rst=bswap_16(ntohs(tcp->rst));
                        }
                        else
                        {
                            syn=ntohs(tcp->syn);
                            ack=ntohs(tcp->ack);
                            rst=ntohs(tcp->rst);
                        }
                        
                        // SYN and ACK = OPEN, ACK and RST = CLOSED
                        if(syn==1 && ack==1 && rst==0)
                        {
                            print_port_state(port,TCP,OPEN);
                        }
                        else if(syn==0 && ack==1 && rst==1)
                        {
                            print_port_state(port,TCP,CLOSED);
                        }
                    }
                    
                }
            }
            else
            {
                if(packet==NULL)
                {
                    // if packet is null after second send, then port is FILTERED
                    print_port_state(port,TCP,FILTERED);
                }
                else
                {
                    // Headers
                    struct ether_header *eth = (struct ether_header*)packet;
                    struct iphdr *ip = (struct iphdr*)(packet + sizeof(struct ether_header));
                    struct tcphdr *tcp = (struct tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct iphdr));
                    // Convert from network byte order to integer, byteswap because of Endianness problem
                    int syn,ack,rst;
                    if(TestByteOrder()==LITTLE_ENDIAN)
                    {
                        syn=bswap_16(ntohs(tcp->syn));
                        ack=bswap_16(ntohs(tcp->ack));
                        rst=bswap_16(ntohs(tcp->rst));
                    }
                    else
                    {
                        syn=ntohs(tcp->syn);
                        ack=ntohs(tcp->ack);
                        rst=ntohs(tcp->rst);
                    }
                    
                    // SYN and ACK = OPEN, ACK and RST = CLOSED
                    if(syn==1 && ack==1 && rst==0)
                    {
                        print_port_state(port,TCP,OPEN);
                    }
                    else if(syn==0 && ack==1 && rst==1)
                    {
                        print_port_state(port,TCP,CLOSED);
                    }
                }
            }
            
        }

        

    }
}
/**
 * @brief Print help message to stdout
 * @retval None
 */
void help()
{
    cout << "Simple Port Scanner, works only with IPv4 :(" << endl;
    cout << "Usage: ./xkrehl04 {-i <interface>} -pu <port-ranges> -pt <port-ranges> [<domain-name> | <IP-address>]" << endl;
    cout << "Example: ./xkrehl04 -i eth0 -pu 21-25 -pt 80,81,82 192.168.1.1" << endl << endl;
    cout << "-pu <port-ranges>: UDP ports to scan, acceptable format (80; 80-85; 80,81,82)" << endl;
    cout << "-pt <port-ranges>: TCP ports to scan, acceptable format (80; 80-85; 80,81,82)" << endl;
    cout << "-i <interface>: Network interface that will be used for scanning (eth0, wlan0, lo, ...)" << endl;
    cout << "Warning: This program has to be run as root (sudo)!" <<endl;
}


int main(int argc, char** argv)

{
    srand(time(NULL));
    string arg_pu;
    string arg_pt;
    string arg_hostname;
    string arg_i;
    bool exist_pu=false;
    bool exist_pt=false;
    bool exist_i=false;
    bool exist_hostname = false;

    if(argc<=1)
    {
        help();
        error(1,"No arguments entered");
        
    }

    // Help
    if(strcmp(argv[1],"--help")==0 || strcmp(argv[1],"-h")==0)
    {
        help();
        exit(0);
    }

    // Process arguments, position doesnt matter
    for(int i=1;i<argc;i++)
    {
        string arg=argv[i];
        string arg_next="";
        if((i+1)<argc)
        {
            arg_next=argv[i+1];
        }

        if (arg=="-i")
        {
            if(exist_i) error(1,"Redeclaration of argument -i");
            exist_i=true;

            if(arg_next[0]=='-')
            {
                cerr << "Next argument after -i starts with \'-\'" << endl;
                exit(1);
            }
            if (arg_next.compare("")==0)
            {
                cerr << "Missing argument" << endl;
                exit(1);
            }
            i++;
            arg_i=arg_next;
        }
        else if (arg=="-pt")
        {
            if(exist_pt) error(1,"Redeclaration of argument -pt");
            exist_pt=true;

            if(arg_next[0]=='-')
            {
                cerr << "Next argument after -pt starts with \'-\'" << endl;
                exit(1);
            }

            if (arg_next.compare("")==0)
            {
                cerr << "Missing argument" << endl;
                exit(1);
            }
            i++;
            arg_pt=arg_next;
        }
        else if (arg=="-pu")
        {
            if(exist_pu) error(1,"Redeclaration of argument -pu");
            exist_pu=true;
            if(arg_next[0]=='-')
            {
                cerr << "Next argument after -pu starts with \'-\'" << endl;
                exit(1);
            }
            if (arg_next.compare("")==0)
            {
                cerr << "Missing argument" << endl;
                exit(1);
            }
            i++;
            arg_pu=arg_next;

        }
        else
        {
            if(exist_hostname) error(1,"Too much arguments");
            exist_hostname=true;
            if(arg[0]=='-')
            {
                cerr << "Unknown argument \'"<< arg <<"\'" << endl;
                exit(1);
            }
            regex ipaddress_regex("^(\\d{1,3}\\.){3}\\d{1,3}$");
            arg_hostname=arg;
        }
    }

    string ip_address_source;
    string interface;
    string ip_address_target =get_ip_from_hostname(arg_hostname.c_str());

    // Settings if target is localhost
    if(arg_hostname=="localhost" || arg_hostname=="127.0.0.1")
    {
        interface="lo";
        ip_address_source=get_ip_interface(interface);
        ip_address_target="127.0.0.1";
    }
    else if(arg_i=="")
    {
        interface=get_default_interface();
        ip_address_source=get_ip_interface(interface);

        
    }
    else
    {
        interface=arg_i;
        ip_address_source=get_ip_interface(interface);
    }

    cout << "PORT\t\tSTATE" << endl;

    // If UDP ports are entered, then chek their format by regex,parse and push to vector and send to another function
    if(arg_pu.empty() == false)
    {
        vector<int> udp_ports;
        regex number("^[\\d]+$");
        regex vector_numbers("^(\\d+)(,\\s*\\d+)*$");
        regex range_numbers("^(\\d+)-(\\d+)$");
        if(regex_match(arg_pu,number))
        {
            udp_ports.push_back(stoi(arg_pu));
        }
        else if(regex_match(arg_pu,vector_numbers))
        {
            stringstream ss(arg_pu);

            int i;

            while (ss >> i)
            {
                udp_ports.push_back(i);

                if (ss.peek() == ',')
                    ss.ignore();
            }
        }
        else if(regex_match(arg_pu,range_numbers))
        {
            string delimiter = "-";
            int first = stoi(arg_pu.substr(0, arg_pu.find(delimiter)));
            int second = stoi(arg_pu.substr(arg_pu.find(delimiter)+1, arg_pu.length()));
            for(int i = first; i <=second; i++)
            {
                udp_ports.push_back(i);
            }
        }
        else
        {
            error(1,"Error while processing -pu");
        }
        
        udp_test_ipv4(ip_address_source,ip_address_target,udp_ports,interface);
        
    }

    // If TCP ports are entered, then chek their format by regex,parse and push to vector and send to another function
    if(arg_pt.empty() == false)
    {
        vector<int> tcp_ports;
        regex number("^[\\d]+$");
        regex vector_numbers("^(\\d+)(,\\s*\\d+)*$");
        regex range_numbers("^(\\d+)-(\\d+)$");
        if(regex_match(arg_pt,number))
        {
            tcp_ports.push_back(stoi(arg_pt));
        }
        else if(regex_match(arg_pt,vector_numbers))
        {
            stringstream ss(arg_pt);

            int i;

            while (ss >> i)
            {
                tcp_ports.push_back(i);

                if (ss.peek() == ',')
                    ss.ignore();
            }
        }
        else if(regex_match(arg_pt,range_numbers))
        {
            string delimiter = "-";
            int first = stoi(arg_pt.substr(0, arg_pt.find(delimiter)));
            int second = stoi(arg_pt.substr(arg_pt.find(delimiter)+1, arg_pt.length()));
            for(int i = first; i <=second; i++)
            {
                tcp_ports.push_back(i);
            }
        }
        else
        {
            error(1,"Error whlie processing argument -pu");
        }

        tcp_test_ipv4(ip_address_source,ip_address_target,tcp_ports,interface);
        
    }
    
    return 0;
}



/******************************************************************************/
/* main進入點                                                                 */
/* Author:YiWen Chen                                                          */
/* Last update: 2020/07/29                                                    */
/*                                                                            */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/ioctl.h>
#include <math.h>

// 參考自:lighttpd-1.4.20/cgi_src/webtool.c中的void GetDhcpClientList(void)

int main(int argc, char **argv){


	FILE *fp;
	struct dhcpOfferedAddr {
		unsigned char hostname[16];
		unsigned char mac[16];
		unsigned long ip;
		unsigned long expires;
	} lease;
	
	int i;
	struct in_addr addr;
	unsigned long expires;
	unsigned d, h, m;
  int count = 0;

	system("killall -q -USR1 udhcpd");
	fp = fopen("/var/udhcpd.leases", "r");
	if (NULL == fp) {
	  puts("not found /var/udhcpd.leases...");
		return;
  }
	while (fread(&lease, 1, sizeof(lease), fp) == sizeof(lease)) {

		count++;
		printf("Client NO.%d \n", count);
		
		printf("  NAME: %s\n", lease.hostname);
		
		printf("  MAC : %02x", lease.mac[0]);
		for (i = 1; i < 6; i++) {
			printf(":%02X", lease.mac[i]);
		}
		printf("\n");	
		
		printf("  IP:");
		addr.s_addr = lease.ip;
		printf(" %s\n", inet_ntoa(addr));
		
		printf("  EXPIRES :" );
		expires = ntohl(lease.expires);		
		d = expires / (24*60*60); expires %= (24*60*60);
		h = expires / (60*60); expires %= (60*60);
		m = expires / 60; expires %= 60;
		if (d) {
		  printf(" %u days", d);
		}
		printf(" %02u:%02u:%02u \n", h, m, (unsigned)expires);
		
		puts("=========================");
	}
	fclose(fp);
  printf("[readDHCPclient] read OVER!");
}


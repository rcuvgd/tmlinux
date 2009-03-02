#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define _(x) x

char *safe_strncpy(char *dst, const char *src, size_t size)
{   
	dst[size-1] = '\0';
	return strncpy(dst,src,size-1);   
}

static int INET_resolve(char *name, struct sockaddr_in *sin, int hostfirst)
{
	/* Grmpf. -FvK */
	sin->sin_family = AF_INET;
	sin->sin_port = 0;

	/* Default is special, meaning 0.0.0.0. */
	if (!strcmp(name, "default")) {
		sin->sin_addr.s_addr = INADDR_ANY;
		return (0);
	}
	/* Look to see if it's a dotted quad. */
	if (inet_aton(name, &sin->sin_addr)) {
		return 0;
	}

	return 0;
}

static int INET_getnetmask(char *adr, struct sockaddr *m, char *name)
{
	struct sockaddr_in *mask = (struct sockaddr_in *) m;
	char *slash, *end;
	int prefix;

	if ((slash = strchr(adr, '/')) == NULL)
		return 0;

	*slash++ = '\0';
	prefix = strtoul(slash, &end, 0);
	if (*end != '\0')
		return -1;

	if (name) {
		sprintf(name, "/%d", prefix);
	}
	mask->sin_family = AF_INET;
	mask->sin_addr.s_addr = htonl(~(0xffffffffU >> prefix));
	return 1;
}

static int set_flag(int skfd,char *ifname, short flag)
{
	struct ifreq ifr;

	safe_strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0) {
		printf("%s: unknown interface: %s\n",ifname,strerror(errno));
		return (-1);
	}
	safe_strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	ifr.ifr_flags |= flag;
	printf("goto bring up interface!\n");
	if (ioctl(skfd, SIOCSIFFLAGS, &ifr) < 0) {
		printf("%s: failed to bring up interface: %d\n",ifname,errno);
		return -1;
	}
	return (0);
}

static int set_netmask(int skfd, struct ifreq *ifr, struct sockaddr *sa)
{
	memcpy((char *) &ifr->ifr_netmask, (char *) sa,
			sizeof(struct sockaddr));
	if (ioctl(skfd, SIOCSIFNETMASK, ifr) < 0) {
		fprintf(stderr, "SIOCSIFNETMASK: %s\n",
				strerror(errno));
		return -1;
	}
	return 0;
}

int main(int argc, char **argv)
{
	int skfd=0;
	int ret=0;
	struct sockaddr sa;
	char host[128];
	struct ifreq ifr;

	/* Find any options. */
	argc--;
	argv++;

	/* Create a channel to the NET kernel. */
	skfd=socket(PF_INET,SOCK_DGRAM,0);
	if(skfd<0){
		printf("No usable address families found.\n");
		exit(-1);
	}

	/* No. Fetch the interface name. */
	safe_strncpy(ifr.ifr_name, *argv++, IFNAMSIZ);
	printf("ifr.ifr_name is %s\n",ifr.ifr_name);

	/* Process the remaining arguments. */

	/* If the next argument is a valid hostname, assume OK. */
	safe_strncpy(host, *argv, (sizeof host));
	printf("host is %s\n",host);

	ret=INET_getnetmask(host,&sa,NULL);
	if(ret==-1){
		printf("Invalid netmask\n");
		exit(-1);
	}

	if(ret==1){
		/*set netmask*/
		printf("goto set netmask\n");
		ret=set_netmask(skfd,&ifr,&sa);
		if(ret!=0){
			printf("Failed to set netmask!\n");
			exit(-1);
		}

	}

	if(INET_resolve(host,(struct sockaddr_in*)&sa,0)<0){
		printf("Invalid ip address.\n");
		exit(-1);
	}

	memcpy((char *) &ifr.ifr_addr, (char *) &sa, sizeof(struct sockaddr));

	ret=ioctl(skfd,SIOCSIFADDR,&ifr);
	if(ret<0){
		printf("Failed to set ip address, error=%d!\n",errno);
		exit(-1);
	}

	ret=set_flag(skfd,ifr.ifr_name, (IFF_UP | IFF_RUNNING));
	if(ret<0){
		printf("Failed to set interface flags\n");
		exit(-1);
	}

	close(skfd);
	printf("ifconfig process exit!\n");

	return 0;
}

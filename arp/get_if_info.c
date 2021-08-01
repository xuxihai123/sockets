#include <arpa/inet.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <netdb.h>

int main() {
    struct ifaddrs *ifap;
    getifaddrs(&ifap);
    for (struct ifaddrs *ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && (ifa->ifa_addr->sa_family == AF_INET6)) {
            char addr[INET6_ADDRSTRLEN];
            getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in6), addr, sizeof(addr), NULL, 0, NI_NUMERICHOST);
            printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, addr);
        } else if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
            char *addr = inet_ntoa(sa->sin_addr);
            printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, addr);
        }
    }
    freeifaddrs(ifap);
    return 0;
}
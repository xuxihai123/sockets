#if defined(HAVE_SIOCGIFHWADDR)
bool get_mac_address(char* mac_addr, const char* if_name = "eth0")
{
    struct ifreq ifinfo;
    strcpy(ifinfo.ifr_name, if_name);
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    int result = ioctl(sd, SIOCGIFHWADDR, &ifinfo);
    close(sd);

    if ((result == 0) && (ifinfo.ifr_hwaddr.sa_family == 1)) {
        memcpy(mac_addr, ifinfo.ifr_hwaddr.sa_data, IFHWADDRLEN);
        return true;
    }
    else {
        return false;
    }
}
#elif defined(HAVE_GETIFADDRS)
bool get_mac_address(char* mac_addr, const char* if_name = "en0")
{
    ifaddrs* iflist;
    bool found = false;
    if (getifaddrs(&iflist) == 0) {
        for (ifaddrs* cur = iflist; cur; cur = cur->ifa_next) {
            if ((cur->ifa_addr->sa_family == AF_LINK) &&
                    (strcmp(cur->ifa_name, if_name) == 0) &&
                    cur->ifa_addr) {
                sockaddr_dl* sdl = (sockaddr_dl*)cur->ifa_addr;
                memcpy(mac_addr, LLADDR(sdl), sdl->sdl_alen);
                found = true;
                break;
            }
        }

        freeifaddrs(iflist);
    }
    return found;
}
#else
# error no definition for get_mac_address() on this platform!
#endif
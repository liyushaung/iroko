#include <net/if.h> // if_nametoindex
#include <libnl3/netlink/route/qdisc.h>
#include <netlink/object-api.h> // nl_object_free()

int get_iface_queue(char *interface) {
    struct nl_sock *sock;
    struct nl_cache *qdisc_cache;
    struct rtnl_qdisc *qdisc;
    int ifindex;
    ifindex = if_nametoindex(interface);
    /* Allocate a netlink socket and connect it with the netlink route module */
    sock = nl_socket_alloc();
    nl_connect(sock, NETLINK_ROUTE);
    /* Get all active qdiscs and find the main qdisc of the target interface*/
    rtnl_qdisc_alloc_cache(sock, &qdisc_cache);
    qdisc = rtnl_qdisc_get_by_parent(qdisc_cache, ifindex, TC_H_ROOT);

    /* Query current queue length on the interface */
    int qdisc_len = rtnl_tc_get_stat(TC_CAST(qdisc),  RTNL_TC_QLEN);

    /* Free all allocated data structures */
    nl_cache_free(qdisc_cache);
    nl_socket_free(sock);
    nl_object_free((struct nl_object *) qdisc);
    return qdisc_len;
}

// Minimal test to verify functionality
// int main(int argc, char ** argv) {
//     int num_ifaces = 1;
//     char *ifaces[] = {"enp0s31f6"};

//     for (int i = 0; i < num_ifaces; i++) {
//         int queue_len = get_iface_queue(ifaces[i]);
//         printf("%d ", queue_len);
//     }
//     printf("\n");
//     return 0;
// }
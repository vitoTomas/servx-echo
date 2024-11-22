/* 
 * Simple UDP echo server deamon.
 * What you send me, I will send it
 * back to you!
 */
#include <servx-echo.h>

#include <string.h>
#include <syslog.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int sockfd, ret;

    /*
     * Open a connection to system logger.
     */
    openlog(SE_LOG_NAME, 0, LOG_DAEMON);

    ret = udp_daemon_init(&sockfd);
    if (ret) {
        syslog(LOG_ERR, "Daemon initialization failed. Exiting");
        closelog();
        return SE_DE_EXIT_INIT;
    }

    ret = udp_daemon_run(&sockfd);
    if (ret) {
        syslog(LOG_ERR, "Daemon runtime error. Exiting");
        closelog();
        return SE_DE_EXIT_RUNT;
    }

    /*
     * If a program continues to this point,
     * it means it received a termination message
     * and shall exit peacfully.
     */

    syslog(LOG_NOTICE, "Daemon shutting down peacefully. Goodbye!");
    closelog();

    return SE_DE_EXIT_SUCC;
}

int udp_daemon_init(int *sockfd) {
    struct sockaddr_in addr;
    int ret;

    /* Will not check for return value (void *) -> (void). */
    (void) memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;  /* Address family IPv4 */
    addr.sin_port = UDP_PORT;   /* Port (default 7) */

    /* Opening a socket for UDP protocol (SOCK_DGRAM). */
    *sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (*sockfd < 0) {
        syslog(LOG_ERR, "Socket error: %s", strerror(errno));
        return SERVX_ECHO_SOCKET;
    }

    /* Binding the socket to a predefined port. */
    ret = bind(*sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    if (ret) {
        syslog(LOG_ERR, "Bind error: %s", strerror(errno));
        (void) close(*sockfd);
        return SERVX_ECHO_BIND;
    }

    syslog(LOG_INFO, "Initialization complete");

    return SERVX_ECHO_SUCC;
}

int udp_daemon_run(int *sockfd) {
    struct sockaddr_in addr;
    socklen_t addr_lenght = sizeof(struct sockaddr_in);
    ssize_t msg_lenght;
    unsigned coffee = 0xC0FFEE;
    char buffer[50];

    /*
     * Main program loop, will exit if it receives
     * the magic code.
     */
    do {
        msg_lenght = recvfrom(*sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &addr, &addr_lenght);
        if (errno) {
            syslog(LOG_ERR, "Receive from error: %s", strerror(errno));
            (void) close(*sockfd);
            return SERVX_ECHO_RECV;
        }

        if (!memcmp(buffer, &coffee, 3) && msg_lenght == 3) {
            /* ... time for a coffee ... */
            break;
        }

        (void) sendto(*sockfd, buffer, msg_lenght, 0, (struct sockaddr *) &addr, addr_lenght);
        if (errno) {
            syslog(LOG_ERR, "Send to error: %s", strerror(errno));
            (void) close(*sockfd);
            return SERVX_ECHO_SEND;
        }
    } while (1);

    /* Magic code was received. */
    (void) close(*sockfd);
    syslog(LOG_NOTICE, "Time for a coffee break...");

    return SERVX_ECHO_SUCC;
}
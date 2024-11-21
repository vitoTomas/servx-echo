#ifndef _SERVX_ECHO_
#define _SERVX_ECHO_

/*
 *  Defining log name for syslog to remain
 *  posix compliant.
 */
#define SE_LOG_NAME         "servx-echo"

/*
 *  If not passed during compile time, define
 *  a UDP port to be 7.
 */
#ifndef UDP_PORT
#define UDP_PORT            7
#endif

/*
 *  Defining Servx Echo program exit status';
 *
 *  @define     SE_DE_EXIT_SUCC     daemon execution ended without error,
 *  @define     SE_DE_EXIT_INIT     daemon execution ended with init error,
 *  @define     SE_DE_EXIT_RUNT     daemon execution ended with runtime error.
 */
#define SE_DE_EXIT_SUCC     0
#define SE_DE_EXIT_INIT     -1
#define SE_DE_EXIT_RUNT     -2

/*
 *  Defining Servx Echo exit status';
 *
 * @define  SERVX_ECHO_SUCC     Function exited successfully.
 * @define  SERVX_ECHO_SOCKET   Failed at opening a socket.
 * @define  SERVX_ECHO_BIND     Failed at binding.
 * @define  SERVX_ECHO_RECV     Failed at receiving data.
 * @define  SERVX_ECHO_SEND     Failed at sending data.
 */
#define SERVX_ECHO_SUCC     0
#define SERVX_ECHO_SOCKET   -1
#define SERVX_ECHO_BIND     -2
#define SERVX_ECHO_RECV     -3
#define SERVX_ECHO_SEND     -4


/*
 * Initializes data for the UDP daemon.
 * 
 * @param       *socket     Socket number for opened UDP socket.
 * 
 * @result      Function exit status.
 */
int udp_daemon_init(int *sockfd);

/*
 * UDP daemon main loop.
 * 
 * @param       *socket     Socket number for opened UDP socket.
 * 
 * @result      Function exit status.
 */
int udp_daemon_run(int *sockfd);

#endif
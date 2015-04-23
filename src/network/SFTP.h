/** 
 * File:   SFTP.h
 * Author: twarren
 *
 * Created on April 6, 2015, 2:22 PM
 */

#ifndef SFTP_H
#define	SFTP_H

#include "../common.h"

// Socket includes
#include <cstring>

#ifdef WIN32
	// Define this so I actually get functions out of the windows header file
	#undef _WIN32_WINNT
	#define _WIN32_WINNT 0x0501
	#include <Ws2tcpip.h>
#else
	#include <sys/socket.h>
	#include <netdb.h>
#endif

#include <stdio.h>

// libssh2 includes
#include <libssh2.h>
#include <libssh2_sftp.h>

class SFTP {
public:
	SFTP(const char *host, const char *user, const char *pass, const char *port="22");
	~SFTP();
	string getFingerprint();
	string getFile(const char *path);
private:
	struct addrinfo host_info;
	struct addrinfo *host_info_list;
	const char *fingerprint;
	int sock;
	int status;
	int rc;
	LIBSSH2_SESSION *session;
	LIBSSH2_SFTP *sftp_session;
	void ssh_connect(const char *host, const char *user, const char *pass, const char *port);
	void sftp_connect();
};

#endif	/* SFTP_H */


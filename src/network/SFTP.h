/* 
 * File:   SFTP.h
 * Author: twarren
 *
 * Created on April 6, 2015, 2:22 PM
 */

#ifndef SFTP_H
#define	SFTP_H

#include "../common.h"

#define LIBSSH_STATIC 1

// libssh2 includes
#include <libssh2.h>
#include <libssh2_sftp.h>

// Socket includes
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>

class SFTP {
public:
	SFTP(const char *host, const char *user, const char *pass, const char *port="22");
	~SFTP();
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


/* 
 * File:   SFTP.h
 * Author: twarren
 *
 * Created on April 6, 2015, 2:22 PM
 */

#ifndef SFTP_H
#define	SFTP_H

#define LIBSSH_STATIC 1
#include "../common.h"

// libssh2 includes
#include <libssh2.h>
#include <libssh2_sftp.h>

// Socket includes
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>

class SFTP {
public:
	SFTP(const char *host, string user, string pass);
	~SFTP();
private:
	LIBSSH2_SESSION *session;
	LIBSSH2_SFTP *sftp_session;
	unsigned long hostaddr;
};

#endif	/* SFTP_H */


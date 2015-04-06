#include "SFTP.h"

SFTP::SFTP(const char *host, string user, string pass) {
	struct addrinfo host_info;
	struct addrinfo *host_info_list;
	int sock;
	int status;
	int rc;
	
	// Clear out memory in addr structure
	memset(&host_info, 0, sizeof host_info);
	
	host_info.ai_family = AF_UNSPEC;
	host_info.ai_socktype = SOCK_STREAM;
	
	status = getaddrinfo(host, "22", &host_info, &host_info_list);
	
	if (status != 0)
	{
		cout << "getaddrinfo error" << gai_strerror(status) << endl;
	}
	
	// Start libssh2
	rc = libssh2_init(0);
	
	if (rc != 0)
	{
		cout << "Libssh2 initialization failed " << "(" << rc << ")" << endl;
		return;
	}
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	session = libssh2_session_init();
	
	/* Since we have set non-blocking, tell libssh2 we are blocking */ 
    libssh2_session_set_blocking(session, 1);
	
	// Actually do the ssh handshake
	rc = libssh2_session_handshake(session, sock);
};

SFTP::~SFTP() {
	
};

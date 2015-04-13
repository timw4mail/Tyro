#include "SFTP.h"

SFTP::SFTP(const char *host, const char *user, const char *pass, const char *port) 
{
	this->ssh_connect(host, user, pass, port);
	this->sftp_connect();
};

SFTP::~SFTP() {
	libssh2_sftp_shutdown(sftp_session);
	libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
	libssh2_session_free(session);
#ifdef WIN32
	closesocket(sock);
#else
	close(sock);
#endif
	libssh2_exit();
	freeaddrinfo(host_info_list);
	freeaddrinfo(&host_info);
};

string SFTP::getFingerprint()
{
	// Get the host fingerprint to check against known hosts
	fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

	cerr << "Fingerprint: ";

	for (int i = 0; i < 20; i++)
	{
		fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
	}
	cerr << endl;

	return (string)fingerprint;
}

string SFTP::getFile(const char *path)
{
	if ( ! fingerprint)
	{
		cerr << "Check fingerprint before grabbing files!" << endl;
		return "";
	}

	string output = "";
	
	LIBSSH2_SFTP_HANDLE *sftp_handle;
	sftp_handle = libssh2_sftp_open(sftp_session, path, LIBSSH2_FXF_READ, 0);
	
	if ( ! sftp_handle)
	{
		cerr << "Unable to open file with SFTP: ";
		cerr << libssh2_sftp_last_error(sftp_session) << endl;
		return output;
	}
	
	do {
		char mem[1024];
		
		// loop until fail
		rc = libssh2_sftp_read(sftp_handle, mem, sizeof(mem));
		
		if (rc > 0)
		{
			output += (string)mem;
		}
		else break;
		
	} while (1);
	
	libssh2_sftp_close(sftp_handle);
	
	return output;
}

void SFTP::ssh_connect(const char *host, const char *user, const char *pass, const char *port)
{
#ifdef WIN32
	WSADATA wsadata;
	int err;
	
	err = WSAStartup(MAKEWORD(2,0), &wsadata);
	if (err != 0)
	{
		cerr << "WSAStartup failed with error:" << err << endl;
		return;
	}
#endif
	
	// Clear out memory in addr structure
	memset(&host_info, 0, sizeof host_info);
	
	host_info.ai_family = AF_UNSPEC;
	host_info.ai_socktype = SOCK_STREAM;
	
	status = getaddrinfo(host, port, &host_info, &host_info_list);
	
	if (status != 0)
	{
		cerr << "getaddrinfo error" << gai_strerror(status) << endl;
	}
	
	// Start libssh2
	rc = libssh2_init(0);
	
	if (rc != 0)
	{
		cerr << "Libssh2 initialization failed " << "(" << rc << ")" << endl;
		return;
	}
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if (connect(sock, host_info_list->ai_addr, host_info_list->ai_addrlen) != 0)
	{
		cerr << "Failed to connect to socket." << endl;
		return;
	}
	
	session = libssh2_session_init();
	
	/* Since we have set non-blocking, tell libssh2 we are blocking */ 
	libssh2_session_set_blocking(session, 1);
	
	// Actually do the ssh handshake
	rc = libssh2_session_handshake(session, sock);
	
	if (rc)
	{
		cout << "Failure establishing SSH session:" << rc << endl;
		return;
	}
	
	//@TODO do something with the handling of known hosts
	
	// Authenticate (by password)
	if (libssh2_userauth_password(session, user, pass))
	{
		cerr << "Authentication by password failed." << endl;
		return;
	}
}

void SFTP::sftp_connect()
{
	sftp_session = libssh2_sftp_init(session);
	
	if ( ! sftp_session)
	{
		cerr << "Unable to start SFTP session" << endl;
		return;
	}
}

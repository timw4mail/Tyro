#include "SFTP.h"

/**
 * Basic username/password auth
 * @param string host
 * @param string user
 * @param string pass
 * @param string base_path
 * @param string port
 */
SFTP::SFTP(const char *host, const char *user, const char *pass, const char *base_path, const char *port) 
{
	this->create_socket(host, port);
	
	// Create ssh session
	this->ssh_connect();
	
	// Authenticate (by password)
	if (libssh2_userauth_password(this->session, user, pass))
	{
		cerr << "Authentication by password failed." << endl;
		return;
	}
	
	// Create sftp session
	this->sftp_connect();
};

SFTP::~SFTP() {
	// Free libssh2 sessions
	if (this->sftp_session != nullptr)
	{
		libssh2_sftp_shutdown(this->sftp_session);
		this->sftp_session = nullptr;
	}
	if (this->session != nullptr)
	{
		libssh2_session_disconnect(this->session, "Normal Shutdown, Thank you for playing");
		libssh2_session_free(this->session);
		this->session = nullptr;
	}
	
	libssh2_exit();
	this->destroy_socket();
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
	sftp_handle = libssh2_sftp_open(this->sftp_session, path, LIBSSH2_FXF_READ, 0);
	
	if ( ! sftp_handle)
	{
		cerr << "Unable to open file with SFTP: ";
		cerr << libssh2_sftp_last_error(this->sftp_session) << endl;
		return output;
	}
	
	do {
		char mem[1024];
		
		// loop until fail
		int rc = libssh2_sftp_read(sftp_handle, mem, sizeof(mem));
		
		if (rc > 0)
		{
			output += (string)mem;
		}
		else break;
		
	} while (1);
	
	libssh2_sftp_close(sftp_handle);
	
	return output;
}

void SFTP::create_socket(const char *host, const char *port)
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
	
	host_info.ai_family = AF_UNSPEC; // IPv4/IPv6
	host_info.ai_socktype = SOCK_STREAM; // TCP stream sockets
	host_info.ai_flags = AI_PASSIVE; // fill in my IP for me
	
	int status = getaddrinfo(host, port, &host_info, &host_info_list);
	
	if (status != 0)
	{
		cerr << "getaddrinfo error: " << gai_strerror(status) << endl;
	}
	
	this->sock = socket(
		host_info_list->ai_family, 
		host_info_list->ai_socktype,
		host_info_list->ai_protocol
	);
	
	if (connect(this->sock, host_info_list->ai_addr, host_info_list->ai_addrlen) != 0)
	{
		cerr << "Failed to connect to socket." << endl;
		return;
	}
}

void SFTP::destroy_socket()
{
#ifdef WIN32
	closesocket(sock);
	WSACleanup();
#else
	close(sock);
	
	freeaddrinfo(host_info_list);
	freeaddrinfo(&host_info);
#endif
}

void SFTP::ssh_connect()
{	
	// Start libssh2
	int rc = libssh2_init(0);
	
	if (rc != 0)
	{
		cerr << "Libssh2 initialization failed " << "(" << rc << ")" << endl;
		return;
	}
	
	this->session = libssh2_session_init();
	
	// Since we have set non-blocking, tell libssh2 we are blocking
	libssh2_session_set_blocking(this->session, 1);
	
	// Actually do the ssh handshake
	rc = libssh2_session_handshake(this->session, this->sock);
	
	if (rc)
	{
		cout << "Failure establishing SSH session:" << rc << endl;
		return;
	}
	
	//@TODO do something with the handling of known hosts
}

void SFTP::sftp_connect()
{
	this->sftp_session = libssh2_sftp_init(this->session);
	
	if ( ! this->sftp_session)
	{
		cerr << "Unable to start SFTP session" << endl;
		return;
	}
}

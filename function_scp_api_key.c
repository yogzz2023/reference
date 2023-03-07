#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libssh/libssh.h>

int scp_file(const char* local_path, const char* remote_host, const char* remote_path, const char* username) 
{
    ssh_session session = ssh_new();
    if (session == NULL) 
    {
        fprintf(stderr, "Error creating SSH session\n");
        return -1;
    }

// Set SSH options 
    ssh_options_set(session, SSH_OPTIONS_HOST, remote_host);
    ssh_options_set(session, SSH_OPTIONS_USER, username);
    // ssh_options_set(session, SSH_OPTIONS_PASSWORD_AUTH, password);

// Connect to the SSH server
    if (ssh_connect(session) != SSH_OK)
    {
        fprintf(stderr, "Error connecting to host %s\n", remote_host);
        ssh_free(session);
        return -1;
    }

// Authenticate with the SSH server
   int rc  = ssh_userauth_publickey_auto(session, NULL, NULL);
    if (rc != SSH_AUTH_SUCCESS)
     {
        fprintf(stderr, "Error: Authentication failed: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        exit(1);
    }
// Create an SCP session
    ssh_scp scp = ssh_scp_new(session, SSH_SCP_WRITE, remote_path);
    if (scp == NULL)
    {
        fprintf(stderr, "Error creating SCP session\n");
        ssh_disconnect(session);
        ssh_free(session);
        return -1;
    }

// Initiate the SCP transfer
    if (ssh_scp_init(scp) != SSH_OK) 
    {
        fprintf(stderr, "Error initializing SCP session\n");
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        return -1;
    }

// Copy the file to the remote server
    if (ssh_scp_push_file(scp, local_path, 0 , 0644) != SSH_OK) 
    {
        fprintf(stderr, "Error pushing file %s to remote host %s\n", local_path, remote_host);
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        return -1;
    }
// Close the SCP session
    ssh_scp_close(scp);
    ssh_scp_free(scp);

// Disconnect from the SSH server and free resources
    ssh_disconnect(session);
    ssh_free(session);
    return 0;
}

int main()
{
    const char* local_path = "/home/yogesh/Music/source/file.txt"; //source_path
    const char* remote_host = "192.168.1.37"; 
    const char* remote_path = "/home/yogesh/Music/destination/"; //destination_path
    const char* username = "yogesh"; //username
    // const char* password = "123"; //pass
    int result = scp_file(local_path, remote_host, remote_path, username);
    if (result != 0) 
    {
        printf("Error copying file\n");
        return 1;
    }
    printf("file transferred successfully \n ");
    return 0;
}

#include <stdio.h>
#include <libssh/libssh.h>

int main()
{
    ssh_session session;
    ssh_scp scp;
    int rc;
    char *src = "/home/yogesh/Music/source/";
    char *dest = "/home/yogesh/Music/destination/";
    
    // initialize ssh session
    session = ssh_new();
    ssh_options_set(session, SSH_OPTIONS_HOST, "192.168.1.37");
    ssh_options_set(session, SSH_OPTIONS_USER, "yogesh");
    
    // connect to ssh server
    rc = ssh_connect(session);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error connecting to host: %s\n", ssh_get_error(session));
        ssh_free(session);
        return 1;
    }
    
    // authenticate
    rc = ssh_userauth_password(session, NULL, "federer21");
    if (rc != SSH_AUTH_SUCCESS) 
    {
        fprintf(stderr, "Error authenticating with password: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }
    
    // initialize scp session
    scp = ssh_scp_new(session, SSH_SCP_WRITE | SSH_SCP_RECURSIVE, dest);
    if (scp == NULL) {
        fprintf(stderr, "Error initializing scp session: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }
    
    // copy file
    rc = ssh_scp_push_file(scp, src, 0 , 0644);
    if (rc != SSH_OK) 
    {
        fprintf(stderr, "Error copying file: %s\n", ssh_get_error(session));
        ssh_scp_close(scp);
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }
    
    // cleanup
    ssh_scp_close(scp);
    ssh_scp_free(scp);
    ssh_disconnect(session);
    ssh_free(session);
    
    return 0;
}

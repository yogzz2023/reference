#include <libssh/libssh.h>
#include <stdio.h>

int main() {
    ssh_session session;
    ssh_scp scp;
    int rc;
       char *src = "/home/yogesh/Music/source/";
    char *dest = "/home/yogesh/Music/destination/";

    // Initialize SSH session
    session = ssh_new();
    ssh_options_set(session, SSH_OPTIONS_HOST, "192.168.100.115");
    ssh_options_set(session, SSH_OPTIONS_PORT_STR, "22");
    ssh_options_set(session, SSH_OPTIONS_USER, "yogesh");

    // Connect to SSH server
    rc = ssh_connect(session);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Error connecting to SSH server: %s\n", ssh_get_error(session));
        ssh_free(session);
        return 1;
    }

    // Authenticate with SSH server using public key authentication
    rc = ssh_userauth_publickey_auto(session, NULL, NULL);
    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating with SSH server: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    // Open SCP session
    scp = ssh_scp_new(session, SSH_SCP_WRITE, dest);
    if (scp == NULL) {
        fprintf(stderr, "Error creating SCP session: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    // Initialize SCP session
    rc = ssh_scp_init(scp);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error initializing SCP session: %s\n", ssh_get_error(session));
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    // Copy file to destination
    rc = ssh_scp_push_file(scp, src, 0, 0644);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error copying file: %s\n", ssh_get_error(session));
        ssh_scp_free(scp);
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    }

    // Close SCP session
    ssh_scp_free(scp);

    // Disconnect from SSH server
    ssh_disconnect(session);
    ssh_free(session);

    return 0;
}

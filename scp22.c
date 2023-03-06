#include <libssh/libssh.h>
#include <stdio.h>
#include <stdlib.h>

int scp_put_file(const char *local_file_path, const char *remote_file_path, ssh_session session) 
{
    int rc;
    ssh_scp scp;
    scp = ssh_scp_new(session, SSH_SCP_WRITE, remote_file_path);
    if (scp == NULL) {
        fprintf(stderr, "Error: Failed to open scp session: %s\n", ssh_get_error(session));
        return SSH_ERROR;
    }

    rc = ssh_scp_init(scp);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error: Failed to initialize scp session: %s\n", ssh_get_error(session));
        ssh_scp_free(scp);
        return rc;
    }

    FILE *local_file = fopen(local_file_path, "r");
    if (local_file == NULL) {
        fprintf(stderr, "Error: Failed to open local file %s\n", local_file_path);
        ssh_scp_free(scp);
        return SSH_ERROR;
    }

    char buffer[8192];
    size_t bytes_read;
    do {
        bytes_read = fread(buffer, 1, sizeof(buffer), local_file);
        if (bytes_read > 0) {
            rc = ssh_scp_write(scp, buffer, bytes_read);
            if (rc != SSH_OK) {
                fprintf(stderr, "Error: Failed to write to scp session: %s\n", ssh_get_error(session));
                fclose(local_file);
                ssh_scp_free(scp);
                return rc;
            }
        }
    } while (bytes_read > 0);

    fclose(local_file);
    ssh_scp_close(scp);
    ssh_scp_free(scp);
    return SSH_OK;
}

int main() 
{
    int PORT_NUMBER=22;
    ssh_session session;
    session = ssh_new();
    if (session == NULL)
    {
        fprintf(stderr, "Error: Failed to create ssh session\n");
        exit(1);
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, "192.168.100.115");
    ssh_options_set(session, SSH_OPTIONS_USER, "yogesh");
    ssh_options_set(session, SSH_OPTIONS_PORT, &PORT_NUMBER);

    int rc = ssh_connect(session);
    if (rc != SSH_OK) {
        fprintf(stderr, "Error: Failed to connect: %s\n", ssh_get_error(session));
        ssh_free(session);
        exit(1);
    }

    rc = ssh_userauth_publickey_auto(session, NULL, NULL);
    if (rc != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error: Authentication failed: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        exit(1);
    }

    rc = scp_put_file("/home/yogesh/Music/source/file.txt", "user1@192.168.100.115:/home/yogesh/Music/destination/", session);
    if (rc != SSH_OK) 
    {
        fprintf(stderr, "Error: Failed to transfer file: %s\n", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        exit(1);
    }

    ssh_disconnect(session);
    ssh_free(session);
    return 0;
}

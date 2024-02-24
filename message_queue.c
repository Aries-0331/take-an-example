#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME "/my_message_queue"
#define MAX_MSG_SIZE 256
#define MAX_MSG_COUNT 10

int main() {
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_MSG_SIZE + 1];
    int msg_flags = O_CREAT | O_RDWR;
    mode_t mode = S_IRUSR | S_IWUSR; // Permissions for the message queue

    // Set up the attributes of the message queue
    attr.mq_maxmsg = MAX_MSG_COUNT;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_flags = 0;

    // Create the message queue
    mq = mq_open(QUEUE_NAME, msg_flags, mode, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    printf("Message queue created.\n");

    // Send a message to the queue
    printf("Enter message to send: ");
    fgets(buffer, MAX_MSG_SIZE, stdin);

    if (mq_send(mq, buffer, strlen(buffer), 0) == -1) {
        perror("mq_send");
        exit(1);
    }

    printf("Message sent.\n");

    // Receive a message from the queue
    ssize_t bytes_read = mq_receive(mq, buffer, MAX_MSG_SIZE, NULL);
    if (bytes_read == -1) {
        perror("mq_receive");
        exit(1);
    }

    buffer[bytes_read] = '\0';
    printf("Received message: %s\n", buffer);

    // Close the message queue
    mq_close(mq);

    // Remove the message queue
    mq_unlink(QUEUE_NAME);

    return 0;
}

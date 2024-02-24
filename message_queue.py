import sysv_ipc

# Create a new message queue.
mq = sysv_ipc.MessageQueue(123456, flags=sysv_ipc.IPC_CREAT)

# Send a message.
message = "Hello, World!"
mq.send(message)

# Receive a message.
message, _ = mq.receive()
print(f"Received: {message.decode()}")

# Remove the message queue.
mq.remove()
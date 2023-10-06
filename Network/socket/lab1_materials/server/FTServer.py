#!/usr/bin/python3

import socket
import sys


def main(argv):
    # get port number from argv
    port = argv[1]

    # create socket and bind

    try:
        sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
        sockfd.bind(("127.0.0.1", int(port)))
    except socket.error as e:
        print(str(e))
        sys.exit(1)

    sockfd.listen(5)

    print("The server is ready to receive")

    while True:
        try:
            # accept new connection
            try:
                conn, addr = sockfd.accept()
                print(f"Connection Established. Here is remote peer info: {addr}")
            except Exception as e:
                raise RuntimeError(e)

            # receive file name/size message from client
            try:
                msg = conn.recv(1024).decode("ascii")
            except Exception as e:
                raise RuntimeError(e)

            # use Python string split function to retrieve file name and file size from the received message
            fname, filesize = msg.split(":")

            print("Open a file with name '%s' with size %s bytes" % (fname, filesize))

            # create a new file with fname
            try:
                fd = open(fname, "wb")
            except OSError:
                raise RuntimeError("File cannot be located")

            remaining = int(filesize)

            conn.send(b"OK")

            print("Start receiving . . .")
            while remaining > 0:
                # receive the file content into brmsg and write into the file
                try:
                    rmsg = conn.recv(1024)
                except socket.error:
                    raise RuntimeError("Exception has occured")

                try:
                    fd.write(rmsg)
                    remaining -= len(rmsg)
                except OSError:
                    raise RuntimeError("Cannot write")

            print("[Completed]")
            fd.close()
            conn.close()

        except RuntimeError as e:
            print(e)
            continue

    sockfd.close()


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 FTServer.py <Server_port>")
        sys.exit(1)
    main(sys.argv)

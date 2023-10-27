import socket
import sys
import threading


def game_hall():
    print("Entered Game Hall")


def main(argv):
    # get port number from argv
    port = argv[1]
    userInfoPath = argv[2]
    credentials = dict()
    with open(userInfoPath, "r") as f:
        lines = f.readlines()
        for credential in lines:
            user_name, passwd = credential.split(":")
            credentials[user_name] = passwd
    
    for k,v in credentials.items():
      print(k,v)

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
            conn, addr = sockfd.accept()
            print(f"Connection Established. Here is remote peer info: {addr}")
            msg = conn.recv(1024).decode()

            # use Python string split function to retrieve file name and file size from the received message
            req_type, user_name, passwd = msg.split(" ")
            print(msg)

            if (
                req_type == "/login"
                and user_name in credentials
                and credentials[user_name] == passwd
            ):
                conn.send("1001 Athentication successful".encode())
                print("success")
            else:
                conn.send("1002 Autentication failed".encode())
                print("fail")
            conn.close()

        except RuntimeError as e:
            print(e)
            break

    sockfd.close()


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 server.py <Server_port> <userInfoPath>")
        sys.exit(1)
    main(sys.argv)

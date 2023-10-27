import socket
import os.path
import sys


def authenticate(sockfd):
    while True:
        user_name = input("Please input your username: ")
        passwd = input("Please input your password: ")
        msg = f"/login {user_name} {passwd}"
        sockfd.send(msg.encode())

        # receive acknowledge - e.g., "OK"
        rmsg = sockfd.recv(1024).decode().split(" ")

        if rmsg[0] != "1001":
            print("Wrong credentials")
        else:
            print("Loggedin")
            break
    return


def game_hall(sockfd):
    while True:
        command = input().split()
        if command[0] == "/list":
            msg = f"/list"
            sockfd.send(msg.encode())
            print(sockfd.recv(1024).decode())
        elif command[0] == "/enter":
            msg = f"/enter {command[1]}"
            sockfd.send(msg.encode())
            msg = sockfd.recv(1024).decode().split()
            game_room(sockfd, msg)  # take 3011-3013
        elif command[0] == "/exit":
            msg = f"/exit"
            sockfd.send(msg.encode())
            msg = sockfd.recv(1024).decode().split()
            if msg[0] == "4001":
                return
        else:
            print("instruction unclear.")


def game_room(sockfd, msg):
    print(msg)
    if msg[0] == "3013":
        return

    if msg[0] == "3011":
        while msg[0] != "3012":
            msg = sockfd.recv(1024).decode().split()

    if msg[0] == "3012":
        done = 0
        while not done:
            command = input()
            sockfd.send(command.encode())
            response = sockfd.recv(1024).decode().split()
            print(response)
    return


def main(argv):
    sockfd = socket.socket()
    sockfd.connect((argv[1], int(argv[2])))
    print(int(argv[2]))

    print("Connection established. My socket address is", sockfd.getsockname())

    # user authenticate
    authenticate(sockfd)
    game_hall(sockfd)

    # close connection
    print("[Completed]")
    sockfd.close()


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 client.py <Server_addr> <Server_port>>")
        sys.exit(1)
    main(sys.argv)

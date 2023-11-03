import socket
import sys


def authenticate(sockfd):
    """
    Authenticate a user
    """
    tries = 0
    while True:
        user_name = input("Please input your username: ").strip()
        passwd = input("Please input your password: ").strip()
        msg = f"/login {user_name} {passwd}"
        sockfd.send(msg.encode())

        # receive acknowledge - e.g., "OK"
        rmsg = sockfd.recv(1024).decode().split(" ")

        if rmsg[0] != "1001":
            tries += 1
            print(f"{' '.join(rmsg)} {3 - tries} left")
            if tries == 3:
                return False
        else:
            print("Loggedin")
            return True


def game_hall(sockfd):
    """
    Takes care of game-hall communication
    """
    while True:
        command = input().split()
        if command[0] == "/list":
            msg = f"/list"
            sockfd.send(msg.encode())
            print(sockfd.recv(1024).decode())
        elif command[0] == "/enter" and len(command) == 2 and command[1].isnumeric():
            if int(command[1]) >= 10:
                print("There are only 10 rooms")
            else:
                msg = f"/enter {command[1]}"
                sockfd.send(msg.encode())
                msg = sockfd.recv(1024).decode().split()
                game_room(sockfd, msg)  # take 3011-3013
        elif command[0] == "/exit":
            msg = f"/exit"
            sockfd.send(msg.encode())
            msg = sockfd.recv(1024).decode().split()
            if msg[0] == "4001":
                print(" ".join(msg))
                return
        else:
            print("4002 Unrecognized message")


def game_room(sockfd, msg):
    """
    Takes care of game-room communication
    """
    print(" ".join(msg))
    if msg[0] == "3013":
        return

    if msg[0] == "3011":
        while msg[0] != "3012" and msg[0] != "3021":
            msg = sockfd.recv(1024).decode().split()

    if msg[0] == "3012":
        print(" ".join(msg))
        while True:
            command = input()
            sockfd.send(command.encode())
            response = sockfd.recv(1024).decode().split()
            print(" ".join(response))
            if response[0] != "4002":
                break

    if msg[0] == "3021":
        print(" ".join(msg))

    return


def main(argv):
    sockfd = socket.socket()
    sockfd.connect((argv[1], int(argv[2])))
    print(int(argv[2]))

    print("Connection established. My socket address is", sockfd.getsockname())

    # user authenticate
    if authenticate(sockfd):
        game_hall(sockfd)

    # close connection
    print("Client Ends")
    sockfd.close()


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 client.py <Server_addr> <Server_port>>")
        sys.exit(1)
    main(sys.argv)

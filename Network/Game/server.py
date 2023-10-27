import random
import socket
import sys
import threading

NUMBER_OF_ROOMS = 10

lock = threading.Lock()
done = [threading.Condition(lock) for _ in range(NUMBER_OF_ROOMS)]


def game_room(room, room_number):
    while True:
        user_quit = False
        if len(room) == 2:
            ans = ["true", "false"][random.randrange(2)]
            msg = "3012 Game started. Please guess true or false"
            bets = []
            for i, conn in enumerate(room):
                try:
                    conn.send(msg.encode())
                    while True:
                        resp = conn.recv(1024).decode().split()
                        if resp[0] == "/guess" and (
                            resp[1] == "true" or resp[1] == "false"
                        ):
                            bets.append(resp[1].lower())
                            break
                        else:
                            conn.send("4002 Unrecognized message".encode())
                except:
                    room[1 if i == 0 else 0].send("3021 You are the winner".encode())
                    user_quit = True
                    break

            if not user_quit:
                if bets[0] == bets[1]:
                    for conn in room:
                        msg = "3023 The result is a tie"
                        conn.send(msg.encode())
                elif bets[0] == ans:
                    room[0].send("3021 You are the winner".encode())
                    room[1].send("3022 You lost this game".encode())
                else:
                    room[0].send("3022 You lost this game".encode())
                    room[1].send("3021 You are the winner".encode())

            lock.acquire()
            room.pop()
            room.pop()
            done[room_number].notify_all()
            lock.release()


def game_hall(conn, rooms):
    while True:
        try:
            req = conn.recv(1024).decode().split()

            if req[0] == "/list":
                reply = f"3001 {len(rooms)}"
                for v in rooms.values():
                    reply += f" {len(v)}"
                conn.send(reply.encode())
            elif req[0] == "/enter":
                reply = "3011 Wait"
                conn.send(reply.encode())

                lock.acquire()
                if req[1] < 10 and len(rooms[int(req[1])]) < 2:
                    reply = "3011 Wait"
                    conn.send(reply.encode())
                    rooms[int(req[1])].append(conn)
                    cond = done[int(req[1])]
                    cond.wait()
                else:
                    reply = "3013 The room is full"
                    conn.send(reply.encode())
                lock.release()
            elif req[0] == "/exit":
                reply = "4001 Bye bye"
                conn.send(reply.encode())
        except Exception as e:
            print(e)
            break


def main(argv):
    # get port number from argv
    port = argv[1]
    userInfoPath = argv[2]
    credentials = dict()
    threads = []

    rooms = [[] for _ in range(NUMBER_OF_ROOMS)]

    with open(userInfoPath, "r") as f:
        lines = f.readlines()
        for credential in lines:
            user_name, passwd = credential.split(":")
            credentials[user_name] = passwd

    for i, room in enumerate(rooms):
        t = threading.Thread(
            target=game_room,
            args=(room, i),
        )
        t.start()
        threads.append(t)

    # create socket and bind

    try:
        sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
        sockfd.bind(("127.0.0.1", int(port)))
    except socket.error as e:
        print(str(e))
        sys.exit(1)

    sockfd.listen(2 * NUMBER_OF_ROOMS)

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
                t = threading.Thread(
                    target=game_hall,
                    args=(conn, rooms),
                )
                t.start()
                threads.append(t)
            else:
                conn.send("1002 Autentication failed".encode())
                print("fail")

            conn.close()

        except RuntimeError as e:
            print(e)
            for t in threads:
                t.join()
            sockfd.close()
            break

    sockfd.close()


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 server.py <Server_port> <userInfoPath>")
        sys.exit(1)
    main(sys.argv)
    sys.exit(1)

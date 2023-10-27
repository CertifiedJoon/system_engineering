import random
import socket
import sys
import threading

NUMBER_OF_ROOMS = 10

lock = threading.Lock()
done = [threading.Condition(lock) for _ in range(NUMBER_OF_ROOMS)]
rooms = [[] for _ in range(NUMBER_OF_ROOMS)]


def game_room(room_number):
    print(f"game room {room_number} started")
    while True:
        user_quit = False
        if len(rooms[room_number]) == 2:
            ans = ["true", "false"][random.randrange(2)]
            msg = "3012 Game started. Please guess true or false"
            bets = []
            for i, conn in enumerate(rooms[room_number]):
                print(f"ask player {i}: {msg}")
                try:
                    while True and not user_quit:
                        conn.send(msg.encode())
                        resp = conn.recv(1024).decode().split()
                        if not resp:
                            raise Exception
                        elif resp[0] == "/guess" and (
                            resp[1] == "true" or resp[1] == "false"
                        ):
                            bets.append(resp[1].lower())
                            break
                        else:
                            conn.send("4002 Unrecognized message".encode())
                except:
                    print(rooms[room_number])
                    rooms[room_number][1 if i == 0 else 0].send(
                        "3021 You are the winner".encode()
                    )
                    user_quit = True
                    break

            if not user_quit:
                if bets[0] == bets[1]:
                    for conn in rooms[room_number]:
                        msg = "3023 The result is a tie"
                        conn.send(msg.encode())
                elif bets[0] == ans:
                    rooms[room_number][0].send("3021 You are the winner".encode())
                    rooms[room_number][1].send("3022 You lost this game".encode())
                else:
                    rooms[room_number][0].send("3022 You lost this game".encode())
                    rooms[room_number][1].send("3021 You are the winner".encode())

            lock.acquire()
            rooms[room_number].pop()
            rooms[room_number].pop()
            done[room_number].notify_all()
            lock.release()


def game_hall(conn, rooms):
    while True:
        try:
            req = conn.recv(1024).decode().split()
            print(req)
            if req[0] == "/list":
                reply = f"3001 {len(rooms)}"
                for room in rooms:
                    reply += f" {len(room)}"
                print(reply)
                conn.send(reply.encode())
            elif req[0] == "/enter":
                lock.acquire()
                if int(req[1]) < 10 and len(rooms[int(req[1])]) < 2:
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
                break
        except Exception as e:
            print(e)
            conn.close()
            break


def main(argv):
    # get port number from argv
    port = argv[1]
    userInfoPath = argv[2]
    credentials = dict()
    threads = []

    with open(userInfoPath, "r") as f:
        lines = f.readlines()
        for credential in lines:
            user_name, passwd = credential.split(":")
            credentials[user_name] = passwd

    for i, room in enumerate(rooms):
        t = threading.Thread(
            target=game_room,
            args=(i,),
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

            tries = 0

            while tries < 3:
                msg = conn.recv(1024).decode()
                # use Python string split function to retrieve file name and file size from the received message
                req_type, user_name, passwd = msg.split(" ")[:3]
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
                    break
                else:
                    conn.send("1002 Autentication failed".encode())
                    print("fail")
                    tries += 1

            if tries == 3:
                raise Exception

        except KeyboardInterrupt:
            # for t in threads:
            #     t.join()
            sockfd.close()
            break

        except Exception as e:
            print(e)
            conn.close()


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 server.py <Server_port> <userInfoPath>")
        sys.exit(1)
    main(sys.argv)
    sys.exit(1)

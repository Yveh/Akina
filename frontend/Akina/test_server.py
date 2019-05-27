import sys
import socket
import json

server_socket   = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
server_ip       = socket.gethostname()
server_port     = 3012

client          = None
client_ip       = None

def init():
    sys.stderr.write("loading config...")

    try:
        config = open("config.json", 'r')
        config_json = json.loads(config.read())

        global server_ip, server_port
        server_ip = config_json["server_ip"]
        server_port = int(config_json["server_port"])

        sys.stderr.write("\033[1;32mOK\033[0m\n")
    except:
        sys.stderr.write("\033[1;31mFailed\033[0m\nusing default config.\n")


def server_start():
    init()
    server_socket.bind((server_ip, server_port))

def establish_connection():
    sys.stderr.write("started listening : " + server_ip + ":" + str(server_port) + "\n")
    sys.stderr.flush()
    server_socket.listen(2)
    global client, client_ip

    client, client_ip = server_socket.accept()
    sys.stderr.write("connected : "  + str(client_ip) + '\n')

def close_connection():
    global client, client_ip
    client.close()
    client, client_ip = None, None

def server_end():
    server_socket.close()


if __name__ == "__main__":

    server_start()

    while True:
        establish_connection()
        msg = client.recv(1048576).decode()
        sys.stderr.write("recieved : " + msg);
        a = input()
        client.send((a).encode("utf-8"))
        close_connection()

    server_end()


    


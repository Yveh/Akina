import sys
import socket
import json

client_socket   = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
server_ip       = "127.0.0.1"
server_port     = 3012
running 		= False

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
        sys.stderr.write("\033[1;31mFailed\033[0m, using default config.\n")

def client_start():
    init()
    sys.stderr.write("Connecting : " + server_ip + ":" + str(server_port) + '...')
    try:
        client_socket.connect((server_ip, server_port))
    except:
        sys.stderr.write("\033[1;31mFailed\033[0m\n")
        return False
    sys.stderr.write("\033[1;32mOK\033[0m\n")
    return True


def post_and_get(msg):
    if (not running):
        running = True
        client_start()
    client_socket.send(msg.encode("utf-8"))
    return client_socket.recv(1048576).decode()

def client_end():
    client_socket.close()


if __name__ == "__main__": # For testing
    client_start()
    print(post_and_get("hello"))
    client_end()


    

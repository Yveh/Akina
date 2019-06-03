import sys
import socket
import json

client_socket   = None
server_ip       = "127.0.0.1"
server_port     = 3012
loaded          = False


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
    sys.stderr.write("Connecting : " + server_ip + ":" + str(server_port) + '...')
    global client_socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client_socket.connect((server_ip, server_port))
    except:
        sys.stderr.write("\033[1;31mFailed\033[0m\n")
        return False
    sys.stderr.write("\033[1;32mOK\033[0m\n")
    return True

def client_end():
    global client_socket
    client_socket.close()
    client_socket = None

def post_and_get(msg):
    global loaded, client_socket
    if (not loaded):
        loaded = True
        init()
    if not client_start():
        sys.stderr.write("\033[1;31mMessage unsended\033[0m\n")
        return None
    
    client_socket.send(msg.encode("utf-8"))
    ret = client_socket.recv(1048576).decode()
    client_end()
#    except:
#        sys.stderr.write("\033[1;31mFailed\033[0m an error occured when transmiting.\n")        
#        return None
    return ret


if __name__ == "__main__": # For testing
    print(post_and_get("clean"))



    

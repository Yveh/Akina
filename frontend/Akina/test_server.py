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
        sys.stderr.write("recieved : " + msg)
        print(msg)
        if (msg == 'register 张三 zhangsan zhangsan@sjtu.edu.cn 12345678\n'):
            a = '2018\n'
        elif (msg == 'login 2018 zhangsan\n'):
            a = '1\n'
        elif (msg == 'query_profile 2018\n'):
            a = '张三 zhangsan@sjtu.edu.cn 12345678 2\n'
        elif (msg == 'modify_profile 2018 张三 zhangsan zhangsan@sjtu.edu.cn 87654321\n'):
            a = '1\n'
        elif (msg == 'modify_privilege 2018 2018 1\n'):
            a = '0\n'
        elif (msg == 'query_ticket 北京 上海 2018-03-28 CD\n'):
            a = 'c100 北京 2018-03-28 08:00 上海 2018-03-28 08:01 一等座 2000 765.50 二等座 2000 765.49 三等座 2000 765.48\n'
        elif (msg == 'query_transfer 北京 上海 2018-03-28 CD\n'):
            a = 'c101 北京 2018-03-28 08:00 夏威夷 2018-03-28 08:01 一等座 2000 765.50 二等座 2000 765.49 三等座 2000 765.48\nc102 夏威夷 2018-03-28 08:02 上海 2018-03-28 08:03 一等座 2000 765.50 二等座 2000 765.49 三等座 2000 765.48\n'
        elif (msg == 'buy_ticket 6666 1 C101 北京 夏威夷 2018-03-28 一等座\n'):
            a = '1\n'
        elif (msg == 'query_order 6666 2018-03-28 C\n'):
            a = '1\nc101 北京 2018-03-28 08:00 夏威夷 2018-03-28 08:01 一等座 1 765.50 二等座 0 765.49 三等座 0 765.48\n'
        elif (msg == 'refund_ticket 6666 1 C101 北京 夏威夷 2018-03-28 一等座\n'):
            a = '1\n'
        elif (msg == 'add_train abc123456 G123456 G 2 1 商务座\n北京 xx:xx 08:00 00:00 ￥0.0\n夏威夷 08:01 xx:xx 00:00 ￥1.5\n'):
            a = '1\n'
        elif (msg == 'sale_train abc123456\n'):
            a = '1\n'
        elif (msg == 'query_train abc123456\n'):
            a = 'abc123456 G123456 G 2 1 商务座\n北京 xx:xx 08:00 00:00 ￥0.0\n夏威夷 08:01 xx:xx 00:00 ￥1.5\n'
        elif (msg == 'delete_train abc123456\n'):
            a = '1\n'
        elif (msg == 'modify_train abc123456 G123456 G 2 1 商务座\n北京 xx:xx 08:00 00:00 ￥0.0\n夏威夷 08:02 xx:xx 00:00 ￥1.5\n'):
            a = '0\n'
        elif (msg == 'clean\n'):
            a = '1\n'
        elif (msg == 'exit\n'):
            a = 'BYE\n'
        else:
            a = 'command wrong\n'

        print(a)
        client.send((a).encode("utf-8"))
        close_connection()

    server_end()


    


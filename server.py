import socket
import time
import csv

class Web:
    
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket.bind((host, port))
    
    def send(self, msg):
        self.socket.sendto(msg.encode(), ((self.host, self.port)))
        
    def receive(self, buf = 1024):
        data = self.socket.recvfrom(buf)
        try: 
            return data[0].decode()
        except:
            return False
        
    def receiveRaw(self, buf = 1024):
        return self.socket.recvfrom(buf)[0]

server = Web('127.0.0.1', 8000)

tmp = ''
while True:
    char = server.receiveRaw()
    if (char == b'\x00'):
        print(tmp)
        tmp=''
    else:   
        char = char.decode()
        tmp = tmp + char
        curr_time = time.ctime().split()
        ms = int((time.time()*1000) % 1000)
        curr_time = f'{curr_time[0]} {curr_time[1]} {curr_time[2]} {curr_time[3]}:{ms} {curr_time[4]}'
        with open("chars.csv", 'a', encoding='utf-8') as w_file:
            file_writer = csv.writer(w_file, delimiter = ",", lineterminator="\r")
            file_writer.writerow([curr_time, char])  
            w_file.close()
import sys 
sys.path.insert(0,'/home/rohit/Desktop/Network/udp_project/Module/')
from base import * 
class tst(base_class):
    def __init__(self,server_ip:str ,client_ip:str,s_port:int,c_port:int):
        self.server_ip     = server_ip 
        self.client_ip     = client_ip 
        self.s_port        = s_port 
        self.return_data = defaultdict(str) 
        self.c_port        = c_port 
        self.client_sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) 
        self.client_sock.bind((self.client_ip,self.c_port))
        self.buffer_size = 64000
        ic(server_ip,s_port,client_ip,c_port)
    def setup(self,filename:str , delta: float):
        try:
            file = open(filename,'rb+')
        except Exceptions as e:
            err_exit(e)
        file.seek(0,2)
        file_size = file.tell() 
        file.seek(0,0)
        if file_size <= 1024000:
            
        else 
            self.buffer_size = 1024000 
            


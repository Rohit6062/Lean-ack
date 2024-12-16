import sys 
sys.path.insert(0,'/home/rohit/Desktop/Network/udp_project/Module/')
from base import *
class tst(base_class):
    def __init__(self,server_ip,client_ip,s_port,c_port):
        self.server_ip     = server_ip 
        self.client_ip     = client_ip 
        self.s_port        = s_port 
        self.return_data = defaultdict(str) 
        self.c_port        = c_port 
        self.client_sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) 
        self.client_sock.bind((self.client_ip,self.c_port))
        self.buffer_size = 10240.0
        ic(server_ip,s_port,client_ip,c_port)
    def setup(self,filename,delta):
        try:
            self.file = open(filename,'r+')
        except Exception as e:
            ic(e)
            exit(0)
        self.file.seek(0,2)
        self.file_size = self.file.tell()
        self.file.seek(0,0)
        if self.file_size < 1025:
            self.buffer_size = 1 
            self.n = self.file_size 
        elif self.file_size <= self.buffer_size: 
            self.buffer_size = int(ceil(self.file_size / 10.0))
            self.n = 10 
        else:
            self.n = int(ceil(self.file_size / (self.buffer_size*1.0)))
            self.buffer_size = 10240
        cnt = 0
        ic(self.buffer_size,self.file_size,self.n)
        recieved_data = None
        while recieved_data == None or recieved_data['data'][2:] != self.int_to_bytes(0,1):
            data = self.int_to_bytes(cnt,1) + self.int_to_bytes(self.file_size,4)+ self.int_to_bytes(self.n,4)+self.int_to_bytes(self.buffer_size+10,4)+filename     
            data = data
            data = self.checksum(data)+data 
            self.send(data,self.client_sock,self.server_ip,self.s_port)
            recieved_data = self.run_process(self.recieve,'recieve',[self.buffer_size+2,self.client_sock],5)
        cnt+=1 
        recieved_data = None
        ic("setup done -------------") 
        self.rsd = RSD(self.n,delta) 
    def send_file(self,filename,delta = 1):
        self.setup(filename,delta) 
        encode_symbols = dict()
        cnt = 1 
        return_data = Manager().dict()
        # return_data  = self.run_process(self.recieve ,'recieve',[self.buffer_size,self.client_sock],0.1)
        p1 = Process(target = self.recieve , args = (self.buffer_size,self.client_sock,return_data))
        p1.start()
        while True:
            if p1.is_alive():
                degree =  self.rsd.degree()
                seed = random.randint(1,2**32-1)
                random.seed(seed)
                selected_symbols = random.sample(range(self.n),degree)
                pair = [degree,seed]
                if tuple(pair) in encode_symbols:continue
                base = ''
                for i in selected_symbols:
                    self.file.seek(i*self.buffer_size)
                    read_data = self.file.read(self.buffer_size)
                    if len(read_data)<self.buffer_size: read_data = read_data + chr(0)*(self.buffer_size - len(read_data)) 
                    base = self.xor(base,read_data)
                encode_symbols[tuple(pair)]=1
                ic(pair,selected_symbols)
                data = self.int_to_bytes(cnt,1)+self.int_to_bytes(pair[0],4)+self.int_to_bytes(pair[1],4)+base  
                data = self.checksum(data) + data
                self.send(data,self.client_sock,self.server_ip,self.s_port)
                ic(cnt)
                # sleep(1)
                cnt = ((cnt + 1)%256)   
            elif return_data['data'] == 'recieved':
                break 
            else:
                break
                ic(return_data.keys())
                return_data  = self.run_process(self.recieve ,'recieve',[self.buffer_size,self.client_sock],30)
            # sleep(1)
        ic('transfer sucessfull')
if __name__ == "__main__":
    ip1 =   socket.gethostbyname(socket.gethostname())
    ip2 =   ip1
    port1 = 5001
    port2 = 5002
    a = tst(ip1,ip2,port1,port2)
    # filename = raw_input('Enter File u Want to send: ')
    filenames = ['small.txt','test1.txt','test.txt','intercity.sql']
    ic(filenames)
    x = int(input('filename:'))
    a.send_file(filenames[x])

import sys
sys.path.insert(0,'/home/rohit/Desktop/Network/udp_project/Module/')
from base import *
class tst(base_class):
    def __init__(self,server_ip,client_ip,s_port,c_port):
        self.return_data = {}
        self.server_ip     = server_ip 
        self.client_ip     = client_ip 
        self.s_port        = s_port 
        self.c_port        = c_port 
        self.server_sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        self.server_sock.bind((self.server_ip,self.s_port))
        self.header_size = 13
        self.buffer_size = 10240
        self.storage_file = open('.temporary_file','rb+')
        self.waiting_time = 5
        ic(server_ip,s_port,client_ip,c_port)
    def setup(self):
        manager = Manager()
        self.return_data = manager.dict()
        start_time = time() + self.waiting_time + 20 
        p1 = Process(target = self.recieve,args = (1000,self.server_sock,self.return_data))
        p1.start()  
        while p1.is_alive() and time() <  start_time :continue

        if p1.is_alive():p1.terminate();self.err_exit('sender not responding')

        if not self.return_data.get('data'):self.err_exit('return_data NULL')

        data = self.return_data['data'] 
        self.start_seq   = self.bytes_to_int(data[1:2],1) 
        self.file_size   = self.bytes_to_int(data[2:6],4)
        self.block_count = self.bytes_to_int(data[6:10],4)
        self.buffer_size = self.bytes_to_int(data[10:14],4)-10
        self.file_name   = data[14::]
        ic(self.start_seq,self.file_size,self.block_count,self.buffer_size,self.file_name)
        self.is_recived  = [False]* self.block_count
        self.output_file = open(self.file_name,'w+')
        self.bitVector = [False]*self.block_count
        self.boolVector = (ctypes.c_bool * self.block_count)()
        self.subVector = (ctypes.c_bool * self.block_count)()
        data = self.int_to_bytes(0,1) + self.int_to_bytes(0,1)
        data = self.checksum(data) + data
        self.send(data,self.server_sock,self.client_ip,self.c_port)
        ic('-> ',len(data))
        ic('basic setp done')
        self.set1 = (ctypes.c_int * (self.block_count))()
        self.set2 = (ctypes.c_int * (self.block_count))()
        return self.receive_it();
        
    def receive_it(self):
        self.testdict=  {}
        cnt = int(floor(self.block_count * 1.5))#int(floor(log(self.block_count,2))))) 
        prev_seq = 1
        manager = Manager()
        return_data = manager.dict()
        self.storage_file.seek(0)
        available_symbols = deque([])
        recieved_symbols = [False]*self.block_count 
        sleep(1)
        while cnt>0:
            start_time = time() + self.waiting_time + 10
            p1 = Process(target = self.recieve , args = (self.buffer_size+10,self.server_sock , self.return_data))
            p1.start() 
            while p1.is_alive() and time() <  start_time :continue
            if p1.is_alive(): p1.terminate();self.err_exit('something went wrong recieve it') 
            data = self.return_data['data']  
            if self.bytes_to_int(data[1:2],1) < prev_seq and prev_seq != 1:
                ic('repeated or currupted packet')
                prev_seq = (prev_seq + 1)%256
                continue
            prev_seq = self.bytes_to_int(data[1:2],1)
            degree = self.bytes_to_int(data[2:6],4)
            random.seed(self.bytes_to_int(data[6:10],4))
            curr_recived = (random.sample(range(self.block_count),degree))
            self.testdict[self.storage_file.tell()] = (curr_recived,prev_seq,len(data[10:]))
            if degree == 1:available_symbols.appendleft([self.storage_file.tell(),curr_recived])
            else:available_symbols.append([self.storage_file.tell(),curr_recived])
            ic(self.storage_file.tell(),curr_recived,data[10:])
            ln = self.storage_file.write(data[10:])
            for i in curr_recived:
                recieved_symbols[i] = True 
            cnt-=1 
        data = self.int_to_bytes(1,1) + 'recieved'
        data = self.checksum(data)+data 
        for _ in range(5):self.send(data,self.server_sock,self.client_ip,self.c_port);sleep(0.001)
        if not all(recieved_symbols):print('insufficient symbols');return False
        start = time()
        ic(self.testdict)
        a = self.__decode(available_symbols)
        ic(time() - start) 
        return a
       
    def __decode(self,available_symbols):
        is_visited = defaultdict(int)
        n = len(available_symbols)
        while n:
            n-=1 
            # ic(available_symbols)
            file_index , indexes = available_symbols.popleft()
            self.storage_file.seek(file_index)
            tmp1 = self.storage_file.read(self.buffer_size)
            ic(indexes , tmp1,self.storage_file.seek(file_index))
            ic(ord(tmp1))
            for i in range(n):
                changed_indexes = deepcopy(available_symbols[i][1])
                if self.is_subset(available_symbols[i][1],indexes): #available_symbols[i][1] - indexes
                    ic(available_symbols[i][1],indexes,self.subtract(available_symbols[i][1] , indexes)) # indexes - available_symbols
                    available_symbols[i][1] = self.subtract(available_symbols[i][1],indexes) #  a-b 
                    self.storage_file.seek(available_symbols[i][0])
                    ic(self.storage_file.tell())
                    ic(tmp2 := self.storage_file.read(self.buffer_size))
                    ic(tmp2 := self.xor(tmp1,tmp2))                
                    ic(ord(tmp2))
                    self.storage_file.seek(available_symbols[i][0])
                    if(ord(tmp2)==13):ic('-------------------------------------------------------------------------------')
                    ic(k := self.storage_file.write(tmp2))
                    if k == 0 and k!=None:ic('write error  : ', k,tmp2,len(tem1));exit(0)
                self.set_boolVector(self.boolVector,changed_indexes,False)
                self.set_boolVector(self.subVector,changed_indexes,False)
            if len(indexes)==1:
                tmp = indexes[0]
                if self.is_recived[tmp]:continue
                self.output_file.seek(tmp*self.buffer_size)
                ic(k:=self.output_file.write(tmp1))
                ic(tmp*self.buffer_size,tmp1)
                if not self.testdict.get(tmp*self.buffer_size):ic("erro in wiritin ");exit(0)
                self.is_recived[tmp] = True
            elif is_visited[tuple(indexes)]>3:continue 
            else:
                is_visited[tuple(indexes)]+=1
                available_symbols.append([file_index,indexes])
                n+=1
        ic(available_symbols,self.is_recived)
        return all(self.is_recived)

if __name__ == "__main__":
    ip1 =   socket.gethostbyname(socket.gethostname())
    ip2 =   ip1
    port1 = 5001
    port2 = 5002
    a = tst(ip1,ip2,port1,port2)
    ic(a.setup())

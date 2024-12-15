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
        self.recieved_data = dict() 
        self.header_size = 13
        self.buffer_size = 10240
        self.storage_file = open('.temporary_file','r+')
        ic(server_ip,s_port,client_ip,c_port)

    def setup(self):
        for i in range(10):
            return_data = self.run_process(self.recieve,'recieve',[1000,self.server_sock],5)
            if return_data :
                data = return_data['data']
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
                data = self.int_to_bytes(0,1) + self.int_to_bytes(0,1)
                data = self.checksum(data) + data
                self.send(data,self.server_sock,self.client_ip,self.c_port)
                ic('-> ',len(data))
                ic('basic setp done')
                return self.receive_it();
            else:
                data = self.int_to_bytes(0,0)+'start'#self.int_to_bytes(0,0)
                data = self.checksum(data)+data
                self.send(data,self.server_sock,self.client_ip,self.c_port)
        if not return_data:
            ic('server is not responding')
            return 
        
        # for i in a:self.bitVector[i] = val 
    def receive_it(self):
        cnt = int(floor(self.block_count * 3/2.0))#int(floor(log(self.block_count,2))))) 
        prev_seq = 1
        manager = Manager()
        return_data = manager.dict()
        self.storage_file.seek(0)
        available_symbols = deque([])
        recieved_symbols = [False]*self.block_count 
        sleep(1)
        while cnt>0:
            print(cnt,prev_seq)
            return_data = self.run_process(self.recieve,'recieve',[self.buffer_size+10,self.server_sock],5)
            # Process(target = self.run_process,name  = 'run_process', args = (self.recieve,'self.recieve',[self.buffer_size+10,self.server_sock,return_data],3))
            if not return_data or not return_data['data']:
                ic('return data is NULL')
                continue 
            data = return_data['data']  
            if self.bytes_to_int(data[1:2],1) < prev_seq and prev_seq != 1:
                ic('repeated or currupted packet')
                prev_seq = (prev_seq + 1)%256
                continue
            prev_seq = self.bytes_to_int(data[1:2],1)
            degree = self.bytes_to_int(data[2:6],4)
            random.seed(self.bytes_to_int(data[6:10],4))
            curr_recived = (random.sample(range(self.block_count),degree))
            if degree == 1:
               available_symbols.appendleft([self.storage_file.tell(),curr_recived])
            else:
                available_symbols.append([self.storage_file.tell(),curr_recived])
            ln = self.storage_file.write(data[10:])
            # ic(degree,curr_recived,ln)
            for i in curr_recived:
                recieved_symbols[i] = True 
            cnt-=1 
            # sleep(1)
        data = self.int_to_bytes(1,1) + 'recieved'
        data = self.checksum(data)+data 
        for i in range(10):
            self.send(data,self.server_sock,self.client_ip,self.c_port)
        if not all(recieved_symbols):
            print('insufficient symbols')
            return False
        start = time()
        a = self.__decode2(available_symbols)
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
            for i in range(n):
                changed_indexes = deepcopy(available_symbols[i][1])
                if self.is_subset(indexes,available_symbols[i][1]):
                    ic(available_symbols[i][1],indexes,self.subtract(indexes,available_symbols[i][1]))
                    available_symbols[i][1] = self.subtract(indexes,available_symbols[i][1]) 
                    self.storage_file.seek(available_symbols[i][0])
                    tmp2 = self.storage_file.read(self.buffer_size)
                    tmp2 = self.xor(tmp1,tmp2)                
                    self.storage_file.seek(available_symbols[i][0])
                    k = self.storage_file.write(tmp2)
                    if k == 0 and k!=None:
                        ic('write error  : ', k,tmp2,len(tem1))
                        exit(0)
                self.set_bitVector(changed_indexes,False)
            if len(indexes)==1 :
                tmp = indexes[0]
                if self.is_recived[tmp]:continue
                self.output_file.seek(tmp*self.buffer_size)
                self.output_file.write(tmp1)
                ic(tmp1,tmp)
                sleep(0.1)
                self.is_recived[tmp] = True
            elif is_visited[tuple(indexes)]>3:continue 
            else:
                is_visited[tuple(indexes)]+=1
                available_symbols.append([file_index,indexes])
                n+=1
        ic(self.is_recived)
        return all(self.is_recived)
    def __decode2(self,available_symbols):
        is_visited = defaultdict(int)
        source_symbols = deque([])
        intermediate_symbols = deque([])
        # available_symbols= deque(sorted(available_symbols,key=lambda x:len(x[1])))
        available_symbols = deque(available_symbols)
        n = len(available_symbols)
        while available_symbols:
            file_index1 , indexes1 = available_symbols.popleft()
            n-=1 
            self.storage_file.seek(file_index1)
            tmp1 = self.storage_file.read(self.buffer_size) 
            while available_symbols:
                file_index2 , indexes2 = available_symbols.popleft()
                changed_indexes = indexes2
                if self.numpy_subset(indexes2,indexes1):
                    ic(indexes2,indexes1,self.numpy_subset(indexes2,indexes1))
                    ic(indexes2 , indexes1 , np.setdiff1d(indexes2,indexes1))
                    indexes2 = np.setdiff1d(indexes2,indexes1)
                    self.storage_file.seek(file_index2)
                    tmp2 = self.storage_file.read(self.buffer_size)
                    tmp2 = self.xor(tmp2,tmp1)
                    self.storage_file.seek(file_index2)
                    k = self.storage_file.write(tmp2)
                    if k<0:
                        ic('write error',k,tmp2,len(tmp1));exit(0)
                self.set_bitVector(changed_indexes,False) 
                ic([i for i in self.boolVector],'sss')
                if len(indexes2)==1:source_symbols.append([file_index2,indexes2])
                else:intermediate_symbols.append([file_index2,indexes2])
            while source_symbols:
                available_symbols.append(source_symbols.popleft())
            while intermediate_symbols:
                available_symbols.append(intermediate_symbols.popleft())
            if len(indexes1)==1:
                tmp = indexes1[0]#.pop()
                if self.is_recived[tmp]:continue 
                self.output_file.seek(tmp*self.buffer_size)
                self.output_file.write(tmp1) 
                self.is_recived[tmp] = True 
            else:
                hash = tuple(indexes1)
                if is_visited[hash] > 3:continue
                available_symbols.append([file_index1,indexes1])
                is_visited[hash] += 1
        ic(self.is_recived)
        return all(self.is_recived) 
    def is_subset(self,a,b):
        self.set_bitVector(b,True)
        for i in a:
            if not self.bitVector[i]:return False 
        return not len(a) == len(b) 
    def subtract(self,a,b):
        for i in a:self.bitVector[i] = False 
        out = [0]*(len(b) - len(a)) 
        k = 0 
        for i in b:
            if self.bitVector[i]:
                out[k] = i 
                k+=1 
        return out
    def set_bitVector(self,a,x):
        ic([i for i in self.boolVector])
        A = (ctypes.c_int * len(a))()
        x = ctypes.c_bool(x)
        k = clib.set(self.boolVector,A,len(a),x)


    def numpy_subset(self,a,b):
        A = (ctypes.c_int * len(a))()
        B = (ctypes.c_int * len(b))() 
        for i in range(len(a)):A[i] = a[i]
        for i in range(len(b)):B[i] = b[i]
        ic(id(self.bitVector))
        ic([i for i in A],[i for i in B])
        k = clib.p(A,len(a),B,len(b),self.boolVector)
        ic(k)


if __name__ == "__main__":
    ip1 =   socket.gethostbyname(socket.gethostname())
    ip2 =   ip1
    port1 = 5001
    port2 = 5002
    a = tst(ip1,ip2,port1,port2)
    ic(a.setup())

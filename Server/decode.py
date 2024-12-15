from server import *
class d(tst):
    def __decode(self,available_symbols ):
        source_symbols = deque([])
        intermediate_symbols = deque([])
        n = len(available_symbols)
        while available_symbols:
            file_index1 , indexes1 = available_symbols.popleft()
            n-=1 
            tmp1 = self.storage_file.read(self.buffer_size) 
            while available_symbols:
                file_index2 , indexes2 = available_symbols.popleft()
                changed_indexes = indexes2
                if self.is_subset(indexes1,indexes2):
                    indexes2 = self.subtract(indexes1,indexes2)
                    self.storage_file.seek(file_index2)
                    tmp2 = self.storage_file.read(self.buffer_size)
                    tmp2 = self.xor(tmp2,tmp1)
                    self.storage_file.seek(file_index2)
                    k = self.storage_file.write(tmp2)
                    if k==0 or k!=None:
                        ic('write error',k,tmp2,len(tmp1));exit(0)
                self.set_bitVector(changed_indexes,False) 
                if len(indexes2)==1:source_symbols.append([file_index2,indexes2])
                else:intermediate_symbols.append(file_index2,indexes2)
            while source_symbols:
                available_symbols.append(source_symbols.popleft())
            while intermediate_symbols:
                available_symbols.append(intermediate_symbols.popleft())
            if len(indexes1)==1:
                tmp = indexes1.pop()
                if self.is_recived[tmp]:continue 
                self.output_file.seek(tmp.self.buffer_size)
                self.output_file.write(tmp1) 
                self.is_recived[tmp] = True 
            else:
                available_symbols.append([file_index1,indexes1])
        ic(self.is_recived)
        return reduce(lambda x,y:x and y,self.is_recived) 


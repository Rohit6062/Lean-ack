from multiprocessing import Process,Manager
from icecream import ic
from copy import deepcopy
from time import sleep,time 
import socket
from math import ceil,sqrt,log,floor
import random 
from collections import deque, defaultdict
from functools import reduce
import numpy as np
import ctypes 
import sys 
clib = ctypes.cdll.LoadLibrary('/home/rohit/Desktop/Network/udp_project/Module/clib.so')
encoding  = 'UTF-8'
class base_class:
    def __init__():
        self.return_data  = None

    def err_exit(self,msg):
        ic(msg)
        exit(1)

    def int_to_bytes(self,x,n):
        return x.to_bytes(n,byteorder = 'big')
    
    def bit_count(self,n):
        if not n:
            return 0 
        return (n & 1) + self.bit_count(n >> 1)

    def bytes_to_int(self,x,n):
        return int.from_bytes(x,byteorder='big')

    def checksum(self,s):
        cnt = 0
        for i in s:cnt+= self.bit_count(i)
        if cnt %2==0:return b'0'
        return b'1'

    def is_correct(self,data):
        return self.checksum(data[1:])==data[:1]

    def create_header(self,header , data):
        temp =header + data 
        return self.checksum(tmp) + header + data

    def send(self,data , sock, reciever_ip,reciever_port):
        if not sock or not data:
            ic('sock or data not appropiate')
            exit(0)
        ln = sock.sendto(data,(reciever_ip,reciever_port))
        ic(ln)
        if ln<0:ic('error in sento',ln,len(data));exit(0);
        return ln

    def recieve(self,buff_size ,sock ,return_data):
        data,addr = sock.recvfrom(2**16-1)
        if self.is_correct(data):
            return_data['data']=data
            return return_data['data']
        else:
            print("data incorrect")

    def xor(self,a,b):
        if not a:return b 
        if not b:return a 
        if len(a) != len(b) : ic("in xor sizes are not same");exit(0)
        out = [chr(0)]*len(a)
        for i in range(len(a)): out[i] = a[i] ^ b[i]
        return bytes(out)

    def is_subset(self,a,b): 
        self.set_boolVector(self.boolVector,a,True)
        for i in range(len(a)):self.set1[i] = a[i]
        for i in range(len(b)):self.set2[i] = b[i]
        k =  clib.is_subset(self.set1,len(a),self.set2,len(b),self.boolVector)
        return k
        
    def subtract(self,a,b):
        for i in b:self.subVector[i] = True
        out = [0]*(len(a) - len(b)) 
        k = 0 
        for i in a:
            if not self.subVector[i]:
                out[k] = i 
                k+=1 
        return out

    def set_boolVector(self,vector,a,x):
        for i in range(len(a)):self.set1[i] = a[i]
        k = clib.set(vector,self.set1,len(a),ctypes.c_bool(x))
        return True

    def _robust_soliton_distribution(self):
        distribution = [1 / self.n] + [1 / (i * (i - 1)) for i in range(2, self.n + 1)]
        tau = [self.S / (i * self.n) for i in range(1, int(ceil(self.n / self.S) + 1))]
        tau.extend([0] * (self.n - len(tau)))  # Pad to length k
        distribution = [distribution[i] + tau[i] for i in range(self.n)]
        total = sum(distribution)
        distribution = [d / total for d in distribution]
        return distribution

    def generate_symbols(self,seed,count,degree):
        random.seed(seed) 
        return random.sample(range(count),degree)

    def _choose_degree(self):
        cumulative = 0
        rand_value = random.random()
        for degree, prob in enumerate(self.degree_distribution, start=1):
            cumulative += prob
            if rand_value <= cumulative:
                return degree
        return self.n

    def do_it(self):
        self.degree_distribution = self._robust_soliton_distribution() 

    def degree(self):
        return self._choose_degree()
import math
import random

class RSD:
    def __init__(self, K, delta):
        """
        Initialize the Robust Soliton Distribution parameters.
        
        Parameters:
        - K: Number of input symbols.
        - delta: Failure probability (small value, e.g., 0.05).
        """
        self.K = K
        self.delta = delta
        self.distribution = self._generate_distribution()

    def _ideal_soliton_distribution(self):
        """
        Calculate the Ideal Soliton Distribution.
        
        Returns:
        - A list representing the ideal soliton distribution.
        """
        distribution = [0] * self.K
        distribution[0] = 1.0 / self.K  # Degree 1
        for i in range(2, self.K + 1):
            distribution[i - 1] = 1.0 / (i * (i - 1))
        return distribution

    def _generate_distribution(self):
        """
        Calculate the Robust Soliton Distribution.
        
        Returns:
        - A list representing the robust soliton distribution.
        """
        # Ideal Soliton Distribution
        ideal_dist = self._ideal_soliton_distribution()
        # ic(ideal_dist)
        # Robust adjustment (additional ripple)
        R = self.K / math.log(self.K / self.delta)
        tau = [0] * self.K
        for i in range(1, int(ceil(R)) + 1):
            tau[i - 1] = R / (i * self.K)
        tau[int(ceil(R)) - 1] += math.log(R / self.delta) / self.K  # Add adjustment

        # Combine ideal and robust distributions
        combined = [ideal + tau_val for ideal, tau_val in zip(ideal_dist, tau)]
        total = sum(combined)

        # Normalize to make it a valid probability distribution
        robust_dist = [x / total for x in combined]
        return robust_dist

    def degree(self):
        """
        - An integer degree sampled from the distribution.
        """
        cumulative = [sum(self.distribution[:i + 1]) for i in range(len(self.distribution))]
        random_value = random.random()
        for i, prob in enumerate(cumulative):
            if random_value <= prob:
                return i + 1  # Degrees are 1-indexed
        return len(self.distribution)  # Return max degree in case of rounding errors

    def get_distribution(self):
        """
        Get the robust soliton distribution.
        
        Returns:
        - A list of probabilities representing the robust soliton distribution.
        """
        return self.distribution


import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import linregress
from tkinder import Make_rapport
file2 = open("tempspyt.txt", "r")

f2=[]
for line in file2:
    f2+=line.split()
file2.close()

p=[]

for i in range(len(f2)):
    s=int(f2[i].split('m')[0])*60
    s+=int(f2[i].split('m')[1][:-1].split('.')[0])
    s+=int(f2[i].split('m')[1][:-1].split('.')[1])/(10**3)
    p+=[s]
    
file = open("thetime200.txt", 'r')
f=[]
for line in file:
    f+=line.split()
file.close()

t1=[]
t2=[]
t3=[]
t4=[]

i=0
l=5
for i in range(int(len(f)/2)):
   
    if f[2*i+1].split('-')[1]=='1':
        s=int(f[2*i].split('m')[0])*60
        s+=int(f[2*i].split('m')[1][:-1].split('.')[0])
        s+=int(f[2*i].split('m')[1][:-1].split('.')[1])/(10**3)
        t1+=[s/l]
        
        
    if f[2*i+1].split('-')[1]=='2':
        s=int(f[2*i].split('m')[0])*60
        s+=int(f[2*i].split('m')[1][:-1].split('.')[0])
        s+=int(f[2*i].split('m')[1][:-1].split('.')[1])/(10**3)
        t2+=[s/l]
        
        
    if f[2*i+1].split('-')[1]=='3':
        s=int(f[2*i].split('m')[0])*60
        s+=int(f[2*i].split('m')[1][:-1].split('.')[0])
        s+=int(f[2*i].split('m')[1][:-1].split('.')[1])/(10**3)
        t3+=[s/l]

    if f[2*i+1].split('-')[1]=='4':
        s=int(f[2*i].split('m')[0])*60
        s+=int(f[2*i].split('m')[1][:-1].split('.')[0])
        s+=int(f[2*i].split('m')[1][:-1].split('.')[1])/(10**3)
        t4+=[s/l]

def factorial(m):
    fac=1
    for i in range(1,m+1):
        fac*=i
    return fac   
def combination(n,k):
    return factorial(n)/((factorial(k))*(factorial(n-k)))
x=[]
x2=[]
p2=[]
for i in range(int(len(f)/8)):
    x+=[combination(5*i,2)]
    


#p2=p[:5]  
#plt.scatter(x2,p2)
plt.scatter(x,t1,c='g')
plt.scatter(x,t2,c='y')
plt.scatter(x,t3,c='orange')
plt.scatter(x,t4,c='r')
plt.xlabel('Number of combinations')
plt.ylabel('Time [s]')
plt.title('Performance test (k=2 vectors=200)')
plt.grid(True)
plt.legend(['c (unithread)','c (threads=2)','c (threads=3)','c (threads=4)'])
#'regression linéaire python','python',
#plt.semilogx()
#plt.loglog()
title= "Rapport de Performance k=2 vectors=200"
Make_rapport(title,p,t1,t2,t3,t4)
plt.show()

# print("estimated C processing time :",time)
# print("estimated Python processing time :", time2)

import matplotlib.pyplot as plt
import numpy as np
from tkinder import Make_rapport

def myfunc2(filename):
    file2 = open(filename, "r")

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
    return p

def myfunc(filename):
    file = open(filename, 'r')
    f=[]
    for line in file:
        f+=line.split()
    file.close()
    t1=[]
    t2=[]
    t3=[]
    t4=[]
    i=0
    for i in range(int(len(f)/2)):
        if f[2*i+1].split('-')[1]=='1':
            s=int(f[2*i].split('m')[0])*60
            s+=int(f[2*i].split('m')[1][:-1].split('.')[0])
            s+=int(f[2*i].split('m')[1][:-1].split('.')[1])/(10**3)
            t1+=[s/2]
            
            
        if f[2*i+1].split('-')[1]=='2':
            s=int(f[2*i].split('m')[0])*60
            s+=int(f[2*i].split('m')[1][:-1].split('.')[0])
            s+=int(f[2*i].split('m')[1][:-1].split('.')[1])/(10**3)
            t2+=[s/2]
            
            
        if f[2*i+1].split('-')[1]=='3':
            s=int(f[2*i].split('m')[0])*60
            s+=int(f[2*i].split('m')[1][:-1].split('.')[0])
            s+=int(f[2*i].split('m')[1][:-1].split('.')[1])/(10**3)
            t3+=[s/2]

        if f[2*i+1].split('-')[1]=='4':
            s=int(f[2*i].split('m')[0])*60
            s+=int(f[2*i].split('m')[1][:-1].split('.')[0])
            s+=int(f[2*i].split('m')[1][:-1].split('.')[1])/(10**3)
            t4+=[s/2]
    return t1,t2,t3,t4 
        
y1=myfunc("timek2p20.txt")
p1=myfunc2("tempsk2p20.txt")
t1=y1[0]
t2=y1[1]
t3=y1[2]
t4=y1[3]
plt.subplot(511)
x=[200,300,400,500,600,700,800,900,1000]
plt.scatter(x,p1)
plt.scatter(x,t1,c='g')
plt.scatter(x,t2,c='y')
plt.scatter(x,t3,c='orange')
plt.scatter(x,t4,c='r')
plt.xlabel('Number of vectors in file')
plt.ylabel('Time [s]')
plt.title('Performance test (-k 2 -p 20)')
plt.legend(['c (unithread)','c (threads=2)','c (threads=3)','c (threads=4)'])
#plt.loglog()

y2=myfunc("timek3p20.txt")
p2=myfunc2("tempsk3p20.txt")
t5=y2[0]
t6=y2[1]
t7=y2[2]
t8=y2[3]
plt.subplot(513)
#plt.scatter(x,p2)
plt.scatter(x,t5,c='g')
plt.scatter(x,t6,c='y')
plt.scatter(x,t7,c='orange')
plt.scatter(x,t8,c='r')
plt.xlabel('Number of vectors in file')
plt.ylabel('Time [s]')
plt.title('Performance test (-k 3 -p 20)')
plt.legend(['c (unithread)','c (threads=2)','c (threads=3)','c (threads=4)'])
#plt.loglog()

y3=myfunc("timek4p20.txt")
p3=myfunc2("tempsk4p20.txt")
t9=y3[0]
t10=y3[1]
t11=y3[2]
t12=y3[3]
plt.subplot(515)
#plt.scatter(x,p3)
plt.scatter(x,t9,c='g')
plt.scatter(x,t10,c='y')
plt.scatter(x,t11,c='orange')
plt.scatter(x,t12,c='r')
plt.xlabel('Number of vectors in file')
plt.ylabel('Time [s]')
plt.title('Performance test (-k 4 -p 20)')
#plt.loglog()
plt.legend(['c (unithread)','c (threads=2)','c (threads=3)','c (threads=4)'])

title1="Rapport de Performance k=2 p=20"
title2="Rapport de Performance k=3 p=20"
title3="Rapport de Performance k=4 p=20"

Make_rapport(title1,p1,t1,t2,t3,t4)
Make_rapport(title2,p2,t5,t6,t7,t8)
Make_rapport(title3,p3,t9,t10,t11,t12)
    
plt.show()

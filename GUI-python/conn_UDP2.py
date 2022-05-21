import socket
import binascii
import csv
import numpy as np
import matplotlib.pyplot as plt
import time



write_name = 'test1.csv'

HOST = '192.168.1.115'       #主机
PORT = 3000            #端口
BUFFSIZE = 4096          #缓冲区大小
ADDR = ( HOST, PORT )    #地址

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


n_channel = 16
collecttime = 10
two = []
MAXRANGE = 65535
AMPL = 6
SAMPLEPERBLOCK = 32

bin_ori = ''
writecache=[]
drawcache = []
for i in range(n_channel):
    writecache.append([])
    drawcache.append([])
ccc=[]
nnn=[]

print(1)
sock.bind(ADDR)
print(2)
for i in range(collecttime):

    # if n !=0:
    #     ccc.append(20

    #     nnn.append(n)
    ori,addr = sock.recvfrom(BUFFSIZE)
    # print(3)
    # print(ori)
    # orid=ori.decode(encoding='byte')
    # print(orid)
    bin_ori += str(binascii.b2a_hex(ori))[2:-1]
    # print(ori)
# print(bin_ori)

# bin_ori += str(binascii.b2a_hex(ori))[2:-1]
# print(ccc)
# print(nnn)
# print(len(bin_ori))
ffff_list = bin_ori.split('ffff00ffff')
# print(ffff_list)


wrong_no =0
for i in range(1,len(ffff_list)):
    if len(ffff_list[i]) != 4 * n_channel * SAMPLEPERBLOCK:
        print(i)
        print('receive wrong')
        wrong_no +=1
        print(ffff_list[i])
        continue
    # print(ffff_list[i])
    for k in range(SAMPLEPERBLOCK):
        Sample_temp = ffff_list[i][4 * n_channel * k : 4 * n_channel * ( k + 1 )]
        # print(len(Sample_temp))
        count4500 = 0
        for j in range(n_channel):
            channel_temp = Sample_temp[4*j:4*(j+1)]
            temp = int(channel_temp[2:4]+channel_temp[0:2],16)
            if temp > 45000 and j == 2:
                count4500 += 1
                print(channel_temp[2:4]+channel_temp[0:2])
            drawcache[j].append((temp - MAXRANGE / 2) / (MAXRANGE / 2) * AMPL)
            writecache[j].append(str(temp))
        print('iter'+str(i))
        print(count4500)
print(wrong_no)
print(wrong_no/len(ffff_list))
fig = plt.figure()

if n_channel == 10 and len(two) == 0:
    for i in range(n_channel):
        ax = fig.add_subplot('2','5',i+1)
        ax.set_title("channel " + str(i))
        nlen = len(drawcache[i])
        x = np.arange(nlen)
        pltdraw = np.array(drawcache[i])
        ax.plot(x,pltdraw,'-')

if n_channel != 10 and len(two) == 0:
    for i in range(n_channel):
        ax = fig.add_subplot('4', '4', i + 1)
        ax.set_title("channel " + str(i))
        nlen = len(drawcache[i])
        x = np.arange(nlen)
        pltdraw = np.array(drawcache[i])
        ax.plot(x, pltdraw, '-')
#plt.tight_layout()

if len(two) == 2:
    ax = fig.add_subplot('2','1',1)
    ax.set_title("channel " + str(two[0]))
    nlen = len(drawcache[two[0]])
    x = np.arange(nlen)
    pltdraw = np.array(drawcache[two[0]])
    ax.plot(x,pltdraw,'-')

    ax = fig.add_subplot('2','1',2)
    ax.set_title("channel " + str(two[1]))
    nlen = len(drawcache[two[1]])
    x = np.arange(nlen)
    pltdraw = np.array(drawcache[two[1]])
    ax.plot(x,pltdraw,'-')






with open(write_name, 'w', newline='') as csvfile:
    file_writer = csv.writer(csvfile, dialect='excel')
    for i in range(len(writecache[0])):
        temp = []
        for j in range(n_channel):
            temp.append(writecache[j][i])
        file_writer.writerow(temp)


plt.show()

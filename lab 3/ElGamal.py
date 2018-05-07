
# coding: utf-8

# In[1]:


import numpy as np
import random


# In[2]:


def Sbox(X):
    x0 = X & 0xFF
    x1 = (X >> 8) & 0xFF
    x2 = (X >> 16) & 0xFF
    x3 = (X >> 24) & 0xFF
    s = [0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16]
    y0 = s[x0]
    y1 = s[x1]
    y2 = s[x2]
    y3 = s[x3]
    Y = y3*(1<<24) + y2*(1<<16) + y1*(1<<8) + y0
    return Y


# In[3]:


def circ_shift_left(num, shift):
    if (shift > 32):
        raise NameError("Shift can't be bigger than 32")
        return None
    else:
        return (num << shift)&0xFFFFFFFF|(num >> ((1<<5) - shift))


# In[4]:


def F(K, R):
    return circ_shift_left(Sbox(K ^ R), 13)


# In[5]:


def get_byte_arr(M):
    byte_arr = []
    for i in range(8):
        byte_arr.append((M >> 8*i) & 0xFF)
    return byte_arr


# In[6]:


def round_keys(Key):
    K = []
    K.append(None)
    byte = get_byte_arr(Key)
    K.append(byte[4]*(1<<24) + byte[5]*(1<<16) + byte[6]*(1<<8) + byte[7])
    K.append(byte[0]*(1<<24) + byte[1]*(1<<16) + byte[2]*(1<<8) + byte[3])
    K.append(K[2] ^ 0xFFFFFFFF)
    K.append(K[1] ^ 0xFFFFFFFF)
    return K


# In[7]:


def E(M, Key):
    L = []
    R = []
    K = round_keys(Key)
    byte = get_byte_arr(M)
    L.append(byte[4]*(1<<24) + byte[5]*(1<<16) + byte[6]*(1<<8) + byte[7])
    R.append(byte[0]*(1<<24) + byte[1]*(1<<16) + byte[2]*(1<<8) + byte[3])
    for i in range(1,5):
        L.append(F(K[i], R[i-1]) ^ L[i-1])
        R.append(L[i-1])
    byte =  get_byte_arr((1<<32)*L[4] + R[4])
    CT = 0
    for i in range(8):
        CT = CT + (1 << 8*i)*byte[7 - i]
    return CT


# In[8]:


def Hash(Mess):
    last_block = Mess.bit_length() % 64
    last_block = (last_block + (8 - last_block % 8)) % 64
    Mess = (Mess << 1) | 1
    last_block = last_block + 1
    Mess = Mess << (64 - last_block)
    M = []
    M.append(None)
    t = Mess.bit_length()
    t = (t + (64 - t % 64)) // 64
    for i in range(1, t + 1):
        M.append((Mess >> 64*(t - i)) & 0xFFFFFFFFFFFFFFFF)
    H = []
    H.append(0)
    for i in range(1, t + 1):
        H.append(E(M[i], H[i - 1]) ^ M[i] ^ H[i - 1])
    return H[t]


# In[9]:


def gen_secret_key(p):
    sk = random.randint(1, p - 1)
    return sk


# In[10]:


def gen_public_key(a, x, p):
    pk = pow(a, x, p)
    return pk


# In[11]:


def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, x, y = egcd(b % a, a)
        return (g, y - (b // a) * x, x)


# In[12]:


def mulinv(b, n):
    g, x, _ = egcd(b, n)
    if g == 1:
        return x % n


# In[13]:


def gen_sign(M, a, x, p, q):
    byte = get_byte_arr(Hash(M))
    H = 0
    for i in range(8):
        H = H + (1 << 8*i)*byte[7 - i]
    H = (0xFFFFFFFFFFFF<<72) + H
    U = random.randint(1, p - 1)
    #U = 0x287245802C0B291785EBC76B89FEE063
    Z = pow(a, U, p)
    k = ((U*Z) * mulinv(H*x - Z, q)) % q
    g = ((H * x - Z) * mulinv(Z, q)) % q
    S = pow(a, g, p)
    Sign = [S, k]
    return H, Sign, U, Z, g


# In[14]:


def check_sign(H, Sign, a, y, p ,q):
    S = Sign[0]
    k = Sign[1]
    y1 = pow(y, H, p)
    y2 = pow(a*S, pow(S, k, p), p)
    if (y1 == y2):
        return True
    else:
        return False


# In[15]:


def read_mess(file_name):
    M = 0
    f = open(file_name, "rb")
    if (not f):
        raise NameError("File doesn't exist")
    while True:
        byte = f.read(1)
        if not byte:
            break
        M = (M << 8) + int.from_bytes(byte, byteorder='big')
    f.close()
    return M


# In[16]:


def make_sign_files(req_file):
    p = 0xAF5228967057FE1CB84B92511BE89A47
    q = 0x57A9144B382BFF0E5C25C9288DF44D23
    a = 0x9E93A4096E5416CED0242228014B67B5
    M = read_mess(req_file)
    x = gen_secret_key(p)
    y = gen_public_key(a, x, p)
    H, Sign, U, Z, G = gen_sign(M, a, x, p, q)
    resp_file = req_file + ".sig"
    f = open(resp_file, "w+")
    if (not f):
        raise NameError("Can't create file")
    f.write("-"*30 + ";\n")
    f.write(req_file + ";\n")
    f.write("H = " + hex(H)[2:] + ";\n")
    f.write("Y = " + hex(y)[2:] + ";\n")
    f.write("K = " + hex(Sign[1])[2:] + ";\n")
    f.write("S = " + hex(Sign[0])[2:] + ";\n")
    f.write("-"*30)
    f.close()
    
    resp_file = req_file + ".sig.add"
    f = open(resp_file, "w+")
    if (not f):
        raise NameError("Can't create file")
    f.write("-"*30 + ";\n")
    f.write(req_file + ";\n")
    f.write("U = " + hex(U)[2:] + ";\n")
    f.write("Z = " + hex(Z)[2:] + ";\n")
    f.write("G = " + hex(G)[2:] + ";\n")
    f.write("-"*30)
    f.close()


# In[17]:


def make_check_file(req_file, req_file_add):
    p = 0xAF5228967057FE1CB84B92511BE89A47
    q = 0x57A9144B382BFF0E5C25C9288DF44D23
    a = 0x9E93A4096E5416CED0242228014B67B5
    f = open(req_file, "rb")
    if (not f):
        raise NameError("File doesn't exist")
    data = f.readlines()
    H = int(data[2][3:-3], 16)
    y = int(data[3][3:-3], 16)
    k = int(data[4][3:-3], 16)
    S = int(data[5][3:-3], 16)
    f.close()
    Sign = [S, k]
    f = open(req_file_add, "rb")
    if (not f):
        raise NameError("File doesn't exist")
    data = f.readlines()
    U = int(data[2][3:-3], 16)
    Z = int(data[3][3:-3], 16)
    G = int(data[4][3:-3], 16)
    f.close()
    if (check_sign(H, Sign, a, y, p, q)):
        print("Підпис вірний")
    else:
        print("Підпис невірний")


import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-sign", type = str)
parser.add_argument("-check", type = str, nargs = '+')
args = parser.parse_args()
if (args.sign != None):
    req_file = args.sign
    if (req_file != ""):
        make_sign_files(req_file)

if (args.check != None):
    req_file = args.check[0]
    req_file_add = args.check[1]
    if (req_file != "")&(req_file_add != ""):
        make_check_file(req_file, req_file_add)

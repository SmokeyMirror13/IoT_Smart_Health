import sys
from cryptography.fernet import Fernet
val_str=sys.stdin.readline()
val_str=val_str[1:-1]
key=b'ZiVh5xTiJRi9RijE4Rrql9mHrt-hqMmKJM2QFInbNGc='
f=Fernet(key)
print(str(f.decrypt(val_str.encode()).decode()))



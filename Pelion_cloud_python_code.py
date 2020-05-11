# ---------------------------------------------------------------------------
# Pelion Device Management SDK
# (C) COPYRIGHT 2017 Arm Limited

from mbed_cloud import ConnectAPI
import os
from sklearn.externals import joblib 
from cryptography.fernet import Fernet
key = Fernet.generate_key()
f = Fernet(key)
print(key)
BUTTON_RESOURCE = "/3303/3/5700"
BUTTON_RESOURCE2 = "/3303/4/5700"

def _current_val(value):
    # Print the current value
    print("Current temp value: %r" % (value))
def _current_val2(value2):
    # Print the current value
    print("Current heart value: %r" % (value2))


def _subscription_handler(device_id, path, value):
    print("Device: %s, Resoure path: %s, Current value: %r" % (device_id, path, value))
    


def _main():
    api = ConnectAPI()
    # calling start_notifications is required for getting/setting resource synchronously
    api.start_notifications()
    devices = api.list_connected_devices().data
    if not devices:
        raise Exception("No connected devices registered. Aborting")

    # Synchronously get the initial/current value of the resource
    while True:
        value = api.get_resource_value(devices[0].id, BUTTON_RESOURCE)
        _current_val(value)
        val1 = value.encode()
        e1 = f.encrypt(val1)
       
        os.system("mosquitto_pub -h localhost -t temp -m "+str(e1)+" -u \"roy\" -P \"roy\"")
        value2 = api.get_resource_value(devices[0].id, BUTTON_RESOURCE2)
        _current_val2(value2)
        
        val2 = value2.encode()
        
        e2 = f.encrypt(val2)
        os.system("mosquitto_pub -h localhost -t heart -m "+str(e2)+" -u \"roy\" -P \"roy\"")
        a=[[float(value),float(value2)]]
        predict_from_joblib = joblib.load('/home/roy/disco/predict.pkl')  
        b=predict_from_joblib.predict(a)
        pre=b[0]
        print(pre)
        os.system("mosquitto_pub -h localhost -t predict -m "+str(pre))
        
            
    
   

    # Register a subscription for new values
    api.add_resource_subscription_async(devices[0].id, BUTTON_RESOURCE, _subscription_handler)

    # Run forever
    ##while True:
      ##  pass
    
if __name__ == "__main__":
    _main()

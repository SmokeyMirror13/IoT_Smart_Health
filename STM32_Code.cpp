
#ifndef MBED_TEST_MODE

#include "mbed.h"
#include "simple-mbed-cloud-client.h"
#include "LittleFileSystem.h"

// Default network interface object. Don't forget to change the WiFi SSID/password in mbed_app.json if you're using WiFi.
NetworkInterface *net;

// Default block device available on the target board
BlockDevice* bd = BlockDevice::get_default_instance();
SlicingBlockDevice sd(bd, 0, 2*1024*1024);

#if COMPONENT_SD || COMPONENT_NUSD
// Use FATFileSystem for SD card type blockdevices
FATFileSystem fs("fs");
#else
// Use LittleFileSystem for non-SD block devices to enable wear leveling and other functions
LittleFileSystem fs("fs");
#endif


// Default User button for GET example and for resetting the storage
InterruptIn button(BUTTON1);

DigitalOut led1(D13, 1);
DigitalOut led2(D7, 1);
DigitalOut led3(D6, 1);

// How often to fetch sensor data (in seconds)
#define SENSORS_POLL_INTERVAL 3.0



AnalogIn bt_temp(A0);
AnalogIn body_hrt(A1);

// Declaring pointers for access to Pelion Client resources outside of main()
MbedCloudClientResource *res_button;
MbedCloudClientResource *res_led1;
MbedCloudClientResource *res_led2;
MbedCloudClientResource *res_led3;
MbedCloudClientResource *res_body_temp;

MbedCloudClientResource *res_body_hrt;


// An event queue is a very useful structure to debounce information between contexts (e.g. ISR and normal threads)
// This is great because things such as network operations are illegal in ISR, so updating a resource in a button's fall() function is not allowed
EventQueue eventQueue;

// When the device is registered, this variable will be used to access various useful information, like device ID etc.
static const ConnectorClientEndpointInfo* endpointInfo;

/**
 * PUT handler
 * @param resource The resource that triggered the callback
 * @param newValue Updated value for the resource
 */
void put_callback(MbedCloudClientResource *resource, m2m::String newValue) {
    printf("*** PUT received, new value: %s                             \n", newValue.c_str());
    led1 = atoi(newValue.c_str());
}
void put_callback2(MbedCloudClientResource *resource, m2m::String newValue) {
    printf("*** PUT received, new value: %s                             \n", newValue.c_str());
    led2 = atoi(newValue.c_str());
}
void put_callback3(MbedCloudClientResource *resource, m2m::String newValue) {
    printf("*** PUT received, new value: %s                             \n", newValue.c_str());
    led3 = atoi(newValue.c_str());
}

/**
 * Button function triggered by the physical button press.
 */
void button_press() {
    int v = res_button->get_value_int() + 1;
    res_button->set_value(v);
    led3=1;
    printf("*** Button clicked %d times                                 \n", v);
}

/**
 * Notification callback handler
 * @param resource The resource that triggered the callback
 * @param status The delivery status of the notification
 */
void button_callback(MbedCloudClientResource *resource, const NoticationDeliveryStatus status) {
    printf("*** Button notification, status %s (%d)                     \n", MbedCloudClientResource::delivery_status_to_string(status), status);
}

/**
 * Registration callback handler
 * @param endpoint Information about the registered endpoint such as the name (so you can find it back in portal)
 */
void registered(const ConnectorClientEndpointInfo *endpoint) {
    printf("Registered to Pelion Device Management. Endpoint Name: %s\n", endpoint->internal_endpoint_name.c_str());
    endpointInfo = endpoint;
}

/**
 * Update sensors and report their values.
 * This function is called periodically.
 */
void sensors_update() {
    float bt_temp_val,hrt_val;
    
    bt_temp_val=(bt_temp.read()*330)+32;
    hrt_val=(body_hrt.read())*97.7;

    printf("Body_temperature: %.2f Fareinheit        \n",bt_temp_val);
    printf("Body_Heart rate: %.2f BPM        \n",hrt_val);
    
    printf("\r\033[8A");

    if (endpointInfo) {

        res_body_temp->set_value(bt_temp_val);
        res_body_hrt->set_value(hrt_val);
        
#endif /* SEND_ALL_SENSORS */
    }
}

int main(void) {
    printf("\nStarting Simple Pelion Device Management Client example\n");

    int storage_status = fs.mount(&sd);
    if (storage_status != 0) {
        printf("Storage mounting failed.\n");
    }
    // If the User button is pressed ons start, then format storage.
    bool btn_pressed = (button.read() == MBED_CONF_APP_BUTTON_PRESSED_STATE);
    if (btn_pressed) {
        printf("User button is pushed on start...\n");
    }

    if (storage_status || btn_pressed) {
        printf("Formatting the storage...\n");
        int storage_status = StorageHelper::format(&fs, &sd);
        if (storage_status != 0) {
            printf("ERROR: Failed to reformat the storage (%d).\n", storage_status);
        }
    } else {
        printf("You can hold the user button during boot to format the storage and change the device identity.\n");
    }

  

    // Connect to the internet (DHCP is expected to be on)
    printf("Connecting to the network using Wifi...\n");
    net = NetworkInterface::get_default_instance();

    nsapi_error_t net_status = -1;
    for (int tries = 0; tries < 3; tries++) {
        net_status = net->connect();
        if (net_status == NSAPI_ERROR_OK) {
            break;
        } else {
            printf("Unable to connect to network. Retrying...\n");
        }
    }

    if (net_status != NSAPI_ERROR_OK) {
        printf("ERROR: Connecting to the network failed (%d)!\n", net_status);
        return -1;
    }

    printf("Connected to the network successfully. IP address: %s\n", net->get_ip_address());

    printf("Initializing Pelion Device Management Client...\n");

    // SimpleMbedCloudClient handles registering over LwM2M to Pelion DM
    SimpleMbedCloudClient client(net, bd, &fs);
    int client_status = client.init();
    if (client_status != 0) {
        printf("ERROR: Pelion Client initialization failed (%d)\n", client_status);
        return -1;
    }

    // Creating resources, which can be written or read from the cloud
    res_button = client.create_resource("3200/0/5501", "Button Count");
    res_button->set_value(0);
    res_button->methods(M2MMethod::GET);
    res_button->observable(true);
    res_button->attach_notification_callback(button_callback);

    res_led1 = client.create_resource("3201/0/5853", "LED State");
    res_led1->set_value(0);
    res_led1->methods(M2MMethod::GET | M2MMethod::PUT);
    res_led1->attach_put_callback(put_callback);
    
    res_led2 = client.create_resource("3201/1/5853", "LED State2");
    res_led2->set_value(0);
    res_led2->methods(M2MMethod::GET | M2MMethod::PUT);
    res_led2->attach_put_callback(put_callback2);
    
    res_led3 = client.create_resource("3201/2/5853", "LED State3");
    res_led3->set_value(0);
    res_led3->methods(M2MMethod::GET | M2MMethod::PUT);
    res_led3->attach_put_callback(put_callback3);

#ifdef SEND_ALL_SENSORS
    // Sensor resources
    
    res_body_temp = client.create_resource("/3303/3/5700", "Temperature body (C)");
    res_body_temp->set_value(0);
    res_body_temp->methods(M2MMethod::GET);
    res_body_temp->observable(true);
    
    res_body_hrt = client.create_resource("/3303/4/5700", "Heart Rate body (C)");
    res_body_hrt->set_value(0);
    res_body_hrt->methods(M2MMethod::GET);
    res_body_hrt->observable(true);
    
#endif /* SEND_ALL_SENSORS */

    printf("Initialized Pelion Client. Registering...\n");

    // Callback that fires when registering is complete
    client.on_registered(&registered);

    // Register with Pelion DM
    client.register_and_connect();

    int i = 600; // wait up 60 seconds before attaching sensors and button events
    while (i-- > 0 && !client.is_client_registered()) {
        wait_ms(100);
    }

    button.fall(eventQueue.event(&button_press));

    // The timer fires on an interrupt context, but debounces it to the eventqueue, so it's safe to do network operations
    Ticker timer;
    timer.attach(eventQueue.event(&sensors_update), SENSORS_POLL_INTERVAL);

    // You can easily run the eventQueue in a separate thread if required
    eventQueue.dispatch_forever();
}

//#endif

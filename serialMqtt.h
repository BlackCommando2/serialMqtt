#include <Arduino_JSON.h>

Stream *serial;
String MQTTCommand = "";
String type;
JSONVar MQTTData;
JSONVar handleType;
char single;
bool dt = false;
int handlerIndex = 0;
void (*dataRecieve[100])(JSONVar);
// JSONVar MQTTData = JSONVar();

void registerSerialToMQTT(Stream *serialObj)
{
    serial = serialObj;
    Serial.println("Serial2");
}

void sendCommandOnSerialMQTT(String type, JSONVar params)
{
    JSONVar json;
    json["type"] = type;
    json["params"] = params;
    // Serial.println("command");
    // Serial.println(JSON.stringify(json));
    serial->println(JSON.stringify(json));
}

void MQTTSerialListener()
{
    // Serial.println("listen");
    while (serial->available() > 0)
    {
        // single = char(serial->read());
        MQTTCommand += char(serial->read());
        // Serial.println(MQTTCommand);
        // if(single=='\n')
        // {
        //     dt = true;
        // }
    }
    if (MQTTCommand != "" )
    {
        // Serial.println("if: "+MQTTCommand);
        // MQTTData = JSONVar();
        MQTTData = JSON.parse(MQTTCommand);
        MQTTCommand = "";
        type = (const char*)MQTTData["type"];
        int typeIndex = handleType[type];
        dataRecieve[typeIndex](MQTTData);
    }
    // else
    // {
    //     return false;
    // }
}
void addSubscriber(String topic, int qos = 1)
{
    JSONVar params;
    params["topic"] = topic;
    params["qos"] = qos;
    // serial->println(JSON.stringify(params));
    sendCommandOnSerialMQTT("addSub", params);
}

void addPublisher(String topic, int qos, JSONVar msg, bool duplicate = true)
{
    JSONVar params;
    params["topic"] = topic;
    params["qos"] = qos;
    params["payload"] = msg;
    params["dup"] = duplicate;
    // serial->println(JSON.stringify(params));
    sendCommandOnSerialMQTT("addPub", params);
}

void addPublisher(String topic, int qos, String payload, bool duplicate = true)
{
    JSONVar params;
    params["topic"] = topic;
    params["qos"] = qos;
    params["payload"] = payload;
    params["dup"] = duplicate;
    // serial->println(JSON.stringify(params));
    sendCommandOnSerialMQTT("addPub", params);
}

void connectWifi(String ssid, String password)
{
    JSONVar params;
    params["ssid"] = ssid;
    params["pass"] = password;
    // serial->println(JSON.stringify(params));
    sendCommandOnSerialMQTT("wifi", params);
}

void setOnRecieve(void (*f)(JSONVar), String type = "")
{
    handleType[type] = handlerIndex;
    Serial.println("HandlerIndex: " + handlerIndex);
    Serial.println("Type: " + type);
    dataRecieve[handlerIndex++] = f;
    // Serial.println("f: "+String(f));
}
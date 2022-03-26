/*
 * Simple MQTT Client
 *
 * Copyright (C) 2022 Marc S. Ressl
 */

#include <string>
#include <vector>
#include <cstring>

#include <mosquitto.h>

class MQTTMessage
{
public:
    std::string topic;
    std::vector<char> payload;
};

class MQTTClient
{
public:
    MQTTClient(std::string clientId);
    ~MQTTClient();

    bool connect(std::string host, int port, std::string username, std::string password);
    bool isConnected();
    void disconnect();

    bool publish(std::string topic, std::vector<char> &payload);

    template <typename T>
    bool publishType(std::string topic, T data)
    {
        std::vector<char> payload;
        payload.resize(sizeof(data));
        std::memcpy(payload.data(), &data, sizeof(data));

        return (publish(topic, payload));
    }

    bool subscribe(std::string topic);
    bool unsubscribe(std::string topic);

    std::vector<MQTTMessage> getMessages();

    template <typename T>
    T convertMessage(std::vector<char> &vector)
    {
        void *pt = vector.data();
        return (*(T *)pt);
    }

private:
    struct mosquitto *mosquittoInstance;
    bool connected;

    std::vector<MQTTMessage> lastMessages;

    friend void onMQTTMessage(struct mosquitto *mosquittoClient,
                              void *context,
                              const struct mosquitto_message *message);
};

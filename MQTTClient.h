/**
 * @file MQTTClient.h
 * @author Marc S. Ressl
 * @brief Simple MQTT Client
 * @version 0.1
 * @date 2022-03-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

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

    /**
     * @brief Hace un publish de un dato tipo dato (float, int, char, double...)
     *
     * @tparam T Tipo de dato
     * @param topic
     * @param data
     * @return true Éxito
     * @return false Fallo
     */
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

    /**
     * @brief Toma un payload MQTT dado y lo devuelve interpretado en algún tipo de dato
     *
     * @tparam T Tipo de dato dado
     * @param vector Payload a procesar
     * @return T Valor procesado
     */
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

#endif // MQTT_CLIENT_H

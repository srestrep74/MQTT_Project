# MQTT Protocol Implementation with QoS 0

## Introduction
This project is an implementation of the MQTT (Message Queuing Telemetry Transport) protocol with Quality of Service (QoS) 0. MQTT is a lightweight, bandwidth-efficient messaging protocol designed for devices with limited resources and unreliable network connections. It operates on top of the TCP/IP protocol and follows a publish-subscribe model, allowing devices to exchange messages in a decentralized and asynchronous manner. 

The MQTT protocol's lightweight nature makes it well-suited for IoT (Internet of Things) applications, where devices often have constrained processing power, memory, and battery life. By minimizing the size of protocol headers and reducing the overhead of message transmission, MQTT enables efficient communication between devices over constrained networks such as cellular, satellite, or low-power wireless networks.

QoS levels in MQTT dictate the delivery guarantees for messages exchanged between clients and brokers. QoS 0, also known as "fire and forget," provides the lowest level of assurance, where messages are delivered to subscribers without acknowledgment. While this level of QoS does not guarantee delivery, it is suitable for applications where occasional message loss is acceptable, and real-time responsiveness is prioritized.

The implementation provided in this project focuses specifically on MQTT with QoS 0, aiming to deliver a lightweight and reliable messaging solution for IoT and M2M deployments. By emphasizing simplicity and efficiency, this implementation enables devices to exchange data seamlessly, even in resource-constrained environments.


## Development
### 1. Client-Server Architecture
The development of the client-server architecture involved designing and implementing the communication framework for MQTT clients and the central MQTT server. This included:

- Establishing TCP/IP connections between clients and the server.
- Handling connection requests and managing client sessions.
- Implementing message routing and delivery from publishers to subscribers.
- Ensuring scalability and robustness of the server to handle multiple concurrent connections.

### 2. Packet Structure
The development of the packet structure focused on defining a standardized format for MQTT packets to facilitate communication between clients and the server. This included:

- Defining the structure of MQTT packets, including the fixed header, variable-length fields, and payload.
- Implementing getters and setters for accessing and modifying packet fields.
- Ensuring compliance with the MQTT protocol specification to support interoperability with other MQTT implementations.

### 3. Handlers
Handlers were developed to process incoming MQTT packets and execute appropriate actions based on the packet type and content. This involved:

- Implementing connection handlers to manage client connections, including establishing, maintaining, and terminating connections.
- Developing subscription handlers to handle client subscriptions to topics and maintain subscription records.
- Creating publication handlers to process messages published by clients and distribute them to subscribed clients.

### 4. Subscribe and Publish Functions
Functions for subscribing to topics and publishing messages were developed to enable clients to interact with the MQTT network. This included:

- Implementing subscribe functions to allow clients to subscribe to specific topics of interest.
- Developing publish functions to enable clients to publish messages to topics, including specifying the QoS level.
- Ensuring efficient message distribution and delivery to subscribed clients based on their topic subscriptions.

### 5. Quality of Service (QoS) 0
The development focused on implementing Quality of Service (QoS) level 0, which guarantees message delivery at least once without acknowledgment. This involved:

- Ensuring that messages published by clients are delivered to subscribed clients without acknowledgment or retry mechanism.
- Implementing mechanisms to handle message transmission and distribution efficiently while prioritizing simplicity and efficiency.
- Testing and validating the reliability and performance of message delivery under different network conditions and message loads.


## Achieved and Unachieved Aspects
### Achieved
- Basic implementation of the MQTT protocol with QoS 0.
- Client-server connection handling.
- Support for subscriptions and publications.
- Concurrent server.
- Packets for subscribe, publish, connect, connack, disconnect and unsubscribe.
- Utf-8 encoders and decoders for connect.
- Subscription with wildcarts.
- Use of a tree for organize topics and messages.
- Log functionality to store the actions for clients and server.
- Unsubscription for one or more topics for users.


### Unachieved
- Complete implementation of all QoS levels, specifically into the subscribe Packet.
- Use the mqtt utf-8 encoders and decoders into the strings in the payload and fixed header of the subscribe and publish Packet.
- Ack responses for the subscribe Packet.
- The data of the client is not deleted when a client disconnects from the server.

## How to execute the protocol
### Compile server
```bash
gcc src/actions/publish.c \
    src/packet/packet.c \
    src/server/server.c \
    src/server/topic_tree/topic_tree.c \
    src/encoders/utf8.c \
    src/client/utils.c \
    src/server/handlers.c \
    src/actions/subscribe.c \
    src/encoders/client_encoders.c \
    src/encoders/server_encoders.c \
    src/actions/unsubscribe.c \
    src/log/log.c \
    -o server
```
### Execute server
```bash
./server SERVER_IP PORT <path/log.log>
```

### Compile client
```bash
gcc src/packet/packet.c
    src/encoders/utf8.c
    src/client/utils.c
    src/client/client.c
    src/encoders/client_encoders.c
    src/actions/subscribe.c
    src/server/topic_tree/topic_tree.c
    src/log/log.c  -o client
```
### Execute server
```bash
./client <path/log.log>
```


## Conclusions
This project has successfully established a solid foundation for the implementation of the MQTT protocol, focusing on the Quality of Service (QoS) level 0. Throughout the development, we have gained a deeper understanding of the internal dynamics and technical challenges inherent in the MQTT protocol. Below are the key points:

1. Satisfactory Basic Implementation: We have developed and demonstrated a functional implementation of the MQTT protocol that supports QoS 0, allowing for one-way communications without multiple delivery guarantees. This implementation efficiently handles basic connections and message transmission between clients and servers.

2. Identified Areas for Improvement: Despite the project's successes, key areas for future improvements have been identified. Primarily, error management needs to be strengthened to increase system robustness. Moreover, the full implementation of higher QoS levels (1 and 2), which ensure message delivery at least once and exactly once respectively, is necessary for more critical applications that require higher reliability guarantees.

3. Technical Challenges and Solutions: The project faced several technical challenges, including efficient memory management and handling multiple simultaneous connections. These challenges were valuable learning opportunities that allowed us to explore and apply advanced C programming techniques.

4. Contribution to Knowledge and Community: By tackling the implementation of such a fundamental protocol for IoT, this project not only increases our technical knowledge but also contributes to the developer community by providing an example of how to approach the implementation of complex communication protocols in embedded systems and IoT applications.

5. Perspectives for Future Research: The work done lays the groundwork for future research and development, where the complete implementation of all QoS levels and more sophisticated error management could be the focus. Additionally, the integration of advanced security measures and performance optimization are areas that could be further explored.

In conclusion, this project has not only met its goal of implementing the basic level of the MQTT protocol but has also established a firm starting point from which the implementation can be expanded and improved. The lessons learned and challenges overcome along the way have enriched our technical understanding and set the stage for future advancements in this vital field of technology.

## References
- [MQTT.org](https://mqtt.org/)
- [MQTT Version 3.1 Specification](https://public.dhe.ibm.com/software/dw/webservices/ws-mqtt/mqtt-v3r1.html)

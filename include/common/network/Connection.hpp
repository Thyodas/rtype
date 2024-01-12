/*
** EPITECH PROJECT, 2023
** server
** File description:
** Connection.hpp
*/

#pragma once

#include "common.hpp"
#include "TsQueue.hpp"
#include "Message.hpp"

namespace rtype::net {

    template<typename T>
    class Connection : public std::enable_shared_from_this<Connection<T>> {
        public:

            enum class owner {
                server,
                client
            };

        public:
            /**
             * @brief Construct a new Connection object
             *
             * @param parent Specifies owner (Server or Client)
             * @note A connection is "owned" by either a server or a client, and its
             * behaviour is slightly different bewteen the two.
             * @param asioContext The asio context object that handles the data transfer
             * @param socket The socket connection
             * @param qIn The incoming message queue reference
             */
            Connection(owner parent, asio::io_context& asioContext, asio::ip::udp::socket &socket,
                TsQueue<OwnedMessage<T>>& qIn);

            virtual ~Connection() = default;

            /**
             * @brief Get the ID object
             * This ID is used system wide - its how clients will understand other clients
             * exist across the whole system.
             *
             * @return uint32_t
             */
            [[nodiscard]] uint32_t getID() const;

        public:
            void connectToClient(uint32_t uid = 0);

            void connectToServer(const asio::ip::udp::resolver::results_type& endpoints);

            void disconnect();

            [[nodiscard]] bool isConnected() const;

            /**
             * @brief Start listening for incoming messages
             * Prime the connection to wait for incoming messages
             * @note Asynchronous
             */
            void startListening();

        public:
            /**
             * @brief Send a message to the remote side
             * Connection is one-to-one so no need to specify the target
             * For a client, the target is the server and vice versa
             *
             * @param msg The message to send
             * @note Asynchronous
             */
            void send(const Message<T>& msg);

        private:
            /**
             * @brief Prime context to write a message header
             * @note Asynchronous
             */
            void writeHeader();

            /**
             * @brief Prime context to write a message body
             * @note Asynchronous
             */
            void writeBody();

            /**
             * @brief Prime context ready to read a message header
             * @note Asynchronous
             */
            void readHeader();

            /**
             * @brief Prime context ready to read a message body
             * @note Asynchronous
             */
            void readBody();

            /**
             * @brief Add a message to the incoming queue
             * @note Asynchronous
             */
            void addToIncomingMessageQueue();

        protected:
            asio::ip::udp::socket _socket;
            asio::ip::udp::endpoint remoteEndpoint;
            asio::io_context& _asioContext;
            TsQueue<Message<T>> _messagesOut;

            // This references the incoming queue of the parent object
            TsQueue<OwnedMessage<T>>& _messagesIn;

            /**
             * @brief The temporary message object
             * It will later be sent to the incoming queue with addToIncomingMessageQueue()
             * @note Asynchronous
             */
            Message<T> _msgTemporaryIn;

            /**
             * @brief The owner of the connection
             * It decides how some of the connection behaves
             * @note Asynchronous
             */
            owner _ownerType = owner::server;

            uint32_t id = 0;
    };

    template<typename T>
    Connection<T>::Connection(owner parent, asio::io_context& asioContext, asio::ip::udp::socket &socket,
        TsQueue<OwnedMessage<T>>& qIn)
        : _socket(std::move(socket)), _asioContext(asioContext), _messagesIn(qIn)
    {
        _ownerType = parent;
    }

    template<typename T>
    uint32_t Connection<T>::getID() const
    {
        return id;
    }

    template<typename T>
    void Connection<T>::connectToClient(uint32_t uid) {
        if (_ownerType == owner::server) {
            id = uid;
            std::cout << "[" << id << "] Connected to client with UID: " << uid << "\n";
        }
    }

    template<typename T>
    void Connection<T>::connectToServer(const asio::ip::udp::resolver::results_type& endpoints) {
        remoteEndpoint = *endpoints.begin();
        readHeader();
    }

    template<typename T>
    void Connection<T>::readHeader() {
        _socket.async_receive_from(asio::buffer(&_msgTemporaryIn.header, sizeof(MessageHeader<T>)), remoteEndpoint,
            [this](std::error_code ec, std::size_t length) {
                if (ec) {
                    std::cout << "[" << id << "] Read Header Fail.\n";
                    _socket.close();
                    return;
                }
                // If the header was read successfully, resize the body buffer to fit the incoming message
                _msgTemporaryIn.body.resize(_msgTemporaryIn.header.size);
                readBody();
            });
    }

    template<typename T>
    void Connection<T>::readBody()
    {
        _socket.async_receive_from(asio::buffer(&_msgTemporaryIn, sizeof(Message<T>)), remoteEndpoint,
            [this](std::error_code ec, std::size_t length) {
                if (ec) {
                    std::cout << "[" << id << "] Read Body Fail.\n";
                    _socket.close();
                    return;
                }
                // extract message id from body, remove it from the body, and store it in the msg.header.id
                std::memcpy(&_msgTemporaryIn.header.id, _msgTemporaryIn.body.data(), sizeof(uint32_t));
                _msgTemporaryIn.body.erase(_msgTemporaryIn.body.begin(), _msgTemporaryIn.body.begin() + sizeof(uint32_t));

                // Access remote endpoint information here
                const asio::ip::address senderAddress = remoteEndpoint.address();
                const uint16_t senderPort = remoteEndpoint.port();
                std::cout << "Received from: " << senderAddress << ":" << senderPort << std::endl;
                addToIncomingMessageQueue();
                readBody(); // Continue listening for the next packet
            });
    }

    template<typename T>
    void Connection<T>::writeHeader() {
        _socket.async_send_to(_socket, asio::buffer(&_messagesOut.front().header, sizeof(MessageHeader<T>)),
            remoteEndpoint,
            [this](std::error_code ec, std::size_t length) {
                if (ec) {
                    std::cout << "[" << id << "] Write Header Fail.\n";
                    _socket.close();
                    return;
                }
                writeBody();
            });
    }

    template<typename T>
    void Connection<T>::writeBody() {
        // extract msg.header.id and prepend it to the body
        _messagesOut.front().body.insert(_messagesOut.front().body.begin(),
            (uint8_t*)&_messagesOut.front().header.id,
            (uint8_t*)&_messagesOut.front().header.id + sizeof(uint32_t));

        _socket.async_send_to(_socket, asio::buffer(_messagesOut.front().body.data(), _messagesOut.front().body.size()),
            remoteEndpoint,
            [this](std::error_code ec, std::size_t length) {
                if (ec) {
                    std::cout << "[" << id << "] Write Body Fail.\n";
                    _socket.close();
                    return;
                }
                _messagesOut.pop_front();
                if (!_messagesOut.empty()) {
                    writeHeader();
                }
            });
    }

    template<typename T>
    void Connection<T>::disconnect()
    {
        if (!isConnected())
            return;
        _messagesOut.waitUntilEmpty();
        asio::post(_asioContext, [this]() {
            _socket.close();
        });
    }

    template<typename T>
    bool Connection<T>::isConnected() const
    {
        return _socket.is_open();
    }

    template<typename T>
    void Connection<T>::startListening()
    {
        asio::post(_asioContext, [this]() {
            readBody();
        });
    }

    template<typename T>
    void Connection<T>::send(const Message<T>& msg)
    {
        asio::post(_asioContext,
            [this, msg]() {
                //_socket.send_to(msg.body, remoteEndpoint);
            });
    }

    template<typename T>
    void Connection<T>::addToIncomingMessageQueue()
    {
        _messagesIn.push_back({this->shared_from_this(), _msgTemporaryIn});
    }
}

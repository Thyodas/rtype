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
            Connection(owner parent, asio::io_context& asioContext, asio::ip::udp::socket socket,
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
    Connection<T>::Connection(owner parent, asio::io_context& asioContext, asio::ip::udp::socket socket,
        TsQueue<OwnedMessage<T>>& qIn)
        : _socket(std::move(socket)), _asioContext(asioContext), _messagesIn(qIn)
    {
        _ownerType = parent;
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
                _socket.send_to(asio::buffer(msg.data(), msg.size()), remoteEndpoint);
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

                // Access remote endpoint information here
                asio::ip::address senderAddress = remoteEndpoint.address();
                const uint16_t senderPort = remoteEndpoint.port();
                std::cout << "Received from: " << senderAddress << ":" << senderPort << std::endl;

                addToIncomingMessageQueue();
                readBody(); // Continue listening for the next packet
            });
    }

    template<typename T>
    void Connection<T>::addToIncomingMessageQueue()
    {
        _messagesIn.push_back({this->shared_from_this(), _msgTemporaryIn});
    }
}

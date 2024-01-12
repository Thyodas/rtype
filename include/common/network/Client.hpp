/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Client.hpp
*/

#pragma once

#include "common.hpp"

namespace rtype::net {
    template<typename T>
    class ClientInterface {
        public:
            ClientInterface();

            virtual ~ClientInterface();

            /**
             * @brief Connect to server with hostname/ip-address and port
             * @param host String of hostname/ip-address
             * @param port Port number
             * @return bool True if connection was successful, false otherwise
             */
            bool connect(const std::string& host, uint16_t port);

            /**
             * @brief Disconnect from server
             */
            void disconnect();

            /**
             * @brief Check if client is actually connected to a server
             * @return bool True if client is connected, false otherwise
             */
            bool isConnected();

        public:
            /**
             * @brief Send message to server
             * @param msg Message to send
             */
            void send(const Message<T>& msg);

            /**
             * @brief Retrieve queue of messages from server
             * @return TsQueue<OwnedMessage<T>>& Queue of messages
             */
            TsQueue<OwnedMessage<T>>& getIncoming();

        protected:
            asio::io_context m_context;
            /**
             * @brief Thread context for asio context
             */
            std::thread thrContext;

            std::unique_ptr<Connection<T>> m_connection;

        private:
            /**
             * @brief Thread safe queue of incoming messages from server
             */
            TsQueue<OwnedMessage<T>> m_qMessagesIn;
    };

    template<typename T>
    ClientInterface<T>::ClientInterface() = default;

    template<typename T>
    ClientInterface<T>::~ClientInterface()
    {
        // If the client is destroyed, always try and disconnect from server
        disconnect();
    }

    template<typename T>
    bool ClientInterface<T>::connect(const std::string& host, const uint16_t port)
    {
        try {
            asio::ip::udp::resolver resolver(m_context);
            asio::ip::udp::resolver::results_type endpoints = resolver.resolve(asio::ip::udp::v4(), host, std::to_string(port));
            asio::ip::udp::socket socket(m_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port));

            // Replace TCP socket with UDP socket
            m_connection = std::make_unique<Connection<T>>(Connection<T>::owner::client, m_context, socket, m_qMessagesIn);

            // Update to use UDP-specific connectToServer function
            m_connection->connectToServer(endpoints);

        } catch (std::exception& e) {
            std::cerr << "Client Exception: " << e.what() << "\n";
            return false;
        }
        return true;
    }

    template<typename T>
    void ClientInterface<T>::disconnect()
    {
        if (isConnected()) {
            m_connection->disconnect();
        }

        m_context.stop();
        if (thrContext.joinable())
            thrContext.join();

        m_connection.release();
    }

    template<typename T>
    bool ClientInterface<T>::isConnected()
    {
        if (m_connection)
            return m_connection->isConnected();
        return false;
    }

    template<typename T>
    void ClientInterface<T>::send(const Message<T>& msg)
    {
        if (isConnected())
            m_connection->send(msg);
    }

    template<typename T>
    TsQueue<OwnedMessage<T>>& ClientInterface<T>::getIncoming()
    {
        return m_qMessagesIn;
    }
}

/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** IClock
*/

#pragma once

namespace client {
    class IClock {
        public:
            virtual ~IClock() = default;
            virtual void restart() = 0;
            virtual double getElapsedTime() const = 0;
    };
}

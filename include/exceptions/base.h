//
// Created by Gabriel Lisboa on 21/03/24.
//

#ifndef LISBENGINE_BASE_H
#define LISBENGINE_BASE_H

#include <iostream>

class ExceptionBase: public std::exception {
public:
    const std::string m_msg;

    ExceptionBase(const char* message) : m_msg(message) {}

    virtual const char* what() const throw ()
    {
        return m_msg.c_str();
    }
};

#endif //LISBENGINE_BASE_H

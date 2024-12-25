#include <worker.h>

#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <thread>
#include <zmq.hpp>
#include <cstdlib>
#include <sys/types.h>
#include <csignal>
#include <sys/wait.h>
#include <chrono>

std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
bool timerRunning = false;
std::chrono::milliseconds accumulatedTime(0);  

void Worker(int id, int sockId) {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::rep);
    try {
        socket.bind("tcp://127.0.0.1:" + std::to_string(5555 + sockId));
    } catch (zmq::error_t&) {
        context.close();
        exit(-1);
    }

    while (true) {
        zmq::message_t request;
        if (socket.recv(request, zmq::recv_flags::none)) {
            std::string msg(static_cast<char*>(request.data()), request.size());

            std::istringstream iss(msg);
            std::string command;
            iss >> command;
            if (command == "exec") {
                std::string subcommand;
                iss >> subcommand;

                if (subcommand == "start") {
                    if (!timerRunning) {
                        startTime = std::chrono::high_resolution_clock::now();
                        timerRunning = true;
                    }
                    std::ostringstream oss;
                    oss << "Ok:" << id;
                    socket.send(zmq::buffer(oss.str()), zmq::send_flags::none);
                } else if (subcommand == "stop") {
                    if (timerRunning) {
                        auto now = std::chrono::high_resolution_clock::now();
                        accumulatedTime += std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
                        timerRunning = false;
                    }
                    std::ostringstream oss;
                    oss << "Ok:" << id;
                    socket.send(zmq::buffer(oss.str()), zmq::send_flags::none);
                } else if (subcommand == "time") {
                    auto now = std::chrono::high_resolution_clock::now();
                    auto currentTime = timerRunning ? std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime) : std::chrono::milliseconds(0);
                    std::ostringstream oss;
                    oss << "Ok:" << id << ": " << (accumulatedTime + currentTime).count();
                    socket.send(zmq::buffer(oss.str()), zmq::send_flags::none);
                } else {
                    socket.send(zmq::buffer("Error: Unknown subcommand"), zmq::send_flags::none);
                }
            } else if (command == "ping") {
                socket.send(zmq::buffer("Ok"), zmq::send_flags::none);
            } else if (command == "exit") {
                socket.send(zmq::buffer("Ok"), zmq::send_flags::none);
                socket.close();
                context.close();
                exit(0);
            } else {
                socket.send(zmq::buffer("!" + command + "!"), zmq::send_flags::none);
            }
        }
    }
}
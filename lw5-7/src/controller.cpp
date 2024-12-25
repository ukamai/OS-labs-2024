#include <controller.h>
#include <tools.h>

#include <iostream>
#include <string>
#include <unordered_set>
#include <memory>
#include <sstream>
#include <thread>
#include <zmq.hpp>
#include <unordered_map>
#include <future>
#include <vector>
#include <istream>

std::string Exec(const std::shared_ptr<Node>& node, const int id, const std::string& oss) {
    std::string result;
    try {
        zmq::message_t message(oss.data(), oss.size());
        node->socket.send(message, zmq::send_flags::none);

        zmq::message_t reply;
        if (node->socket.recv(reply, zmq::recv_flags::none)) {
            result = reply.to_string() + "\n";
        } else {
            result = "Error:" + std::to_string(id) + ": Node is unavailable\n";
        }
    } catch (zmq::error_t&) {
        result = "Error:" + std::to_string(id) + ": Node is unavailable\n";
    } catch (std::exception& e) {
        result = "Error:" + std::to_string(id) + ": " + e.what() + "\n";
    }
    return result;
}

void Controller(std::istream& stream, bool test) {
    std::vector<std::future<std::string>> futures;

    while (true) {
        if (!test) {
            std::cout << "> ";
            std::cout.flush();
        }
        std::string command;
        if (std::getline(stream, command)) {
            std::istringstream iss(command);

            std::string cmdType;
            iss >> cmdType;

            if (cmdType == "create") {
                int id, parent;
                iss >> id >> parent;

                if (id == -1) {
                    std::cout << "Error: Invalid id\n";
                    continue;
                }

                if (parent == -1) {
                    // Creating root node
                    if (root == nullptr) {
                        root = InsertNode(root, id);
                        if (root) {
                            std::cout << "Ok: " << root->pid << "\n";
                        } else {
                            std::cout << "Error: Already exists\n";
                        }
                    } else {
                        // Root already exists
                        std::cout << "Error: Root already exists\n";
                    }
                } else {
                    // Creating child node
                    auto parentNode = FindNode(root, parent);
                    if (!parentNode) {
                        std::cout << "Error: Parent not found\n";
                        continue;
                    }

                    try {
                        zmq::message_t message("ping", 4);
                        parentNode->socket.send(message, zmq::send_flags::none);

                        zmq::message_t reply;
                        if (!parentNode->socket.recv(reply, zmq::recv_flags::none)) {
                            std::cout << "Error: Parent is unavailable\n";
                            continue;
                        }
                    } catch (zmq::error_t&) {
                        std::cout << "Error: Parent is unavailable\n";
                        continue;
                    }

                    auto newNode = InsertNode(parentNode, id);
                    if (newNode) {
                        std::cout << "Ok: " << newNode->pid << "\n";
                    } else {
                        std::cout << "Error: Already exists\n";
                    }
                }
            } else if (cmdType == "exec") {
                int id;
                iss >> id;

                auto node = FindNode(root, id);
                if (!node) {
                    std::cout << "Error:" << id << ": Not found\n";
                    continue;
                }

                std::string subcommand;
                iss >> subcommand;

                std::ostringstream oss;
                oss << "exec " << subcommand;

                try {
                    futures.push_back(std::async(std::launch::async, Exec, node, id, oss.str()));
                } catch (std::exception& e) {
                    std::cout << "Error:" << id << ": " << e.what() << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            } else if (cmdType == "ping") {
                int id;
                iss >> id;

                auto node = FindNode(root, id);
                if (!node) {
                    std::cout << "Error:" << id << ": Not found\n";
                    continue;
                }

                try {
                    zmq::message_t message("ping", 4);
                    node->socket.send(message, zmq::send_flags::none);

                    zmq::message_t reply;
                    if (node->socket.recv(reply, zmq::recv_flags::none)) {
                        std::string response = reply.to_string();
                        if (response == "Ok") {
                            std::cout << "Ok: 1\n";
                        } else {
                            std::cout << "Ok: 0\n";
                        }
                    } else {
                        std::cout << "Ok: 0\n";
                    }
                } catch (zmq::error_t&) {
                    std::cout << "Ok: 0\n";
                }
            } else if (cmdType == "exit") {
                TerminateNodes(root);
                globalContext.close();
                break;
            } else {
                std::cout << "Error: Unknown command\n";
            }
        }

        for (auto it = futures.begin(); it != futures.end();) {
            if (it->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
                std::cout << it->get();
                it = futures.erase(it);
            } else {
                ++it;
            }
        }
    }
}
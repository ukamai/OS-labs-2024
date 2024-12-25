#include <tools.h>

#include <iostream>
#include <string>
#include <thread>
#include <sys/types.h>
#include <csignal>
#include <worker.h>
#include <sys/wait.h>

Node::Node(int id) : id(id), pid(-1) {
    socket = zmq::socket_t(globalContext, zmq::socket_type::req);
    socket.set(zmq::sockopt::rcvtimeo, 2000);
    socket.set(zmq::sockopt::linger, 0);
    sockId = id;
    while (true) {
        try {
            socket.connect("tcp://127.0.0.1:" + std::to_string(5555 + sockId));
            break;
        } catch (zmq::error_t&) {
            ++sockId;
        }
    }
}

std::shared_ptr<Node> FindNode(std::shared_ptr<Node> node, int id) {
    if (!node) {
        return nullptr;
    }
    if (node->id == id) {
        return node;
    }
    if (id < node->id) {
        return FindNode(node->left, id);
    }
    return FindNode(node->right, id);
}

std::shared_ptr<Node> InsertNode(std::shared_ptr<Node>& node, int id) {
    if (!node) {
        node = std::make_shared<Node>(id);
        pid_t pid = fork();
        if (pid == 0) {
            Worker(id, node->sockId);
            exit(0);
        }
        node->pid = pid;
        return node;
    }
    if (id < node->id) {
        return InsertNode(node->left, id);
    } else if (id > node->id) {
        return InsertNode(node->right, id);
    }
    return nullptr; // Node with id already exists
}

void PingNodes(const std::shared_ptr<Node>& node, std::unordered_set<int>& unavailable_nodes) {
    if (!node) {
        return;
    }

    try {
        zmq::message_t message("ping", 4);
        node->socket.send(message, zmq::send_flags::none);

        zmq::message_t reply;
        if (!node->socket.recv(reply, zmq::recv_flags::none) || reply.to_string() != "Ok") {
            unavailable_nodes.insert(node->id);
        }
    } catch (zmq::error_t&) {
        unavailable_nodes.insert(node->id);
    }

    PingNodes(node->left, unavailable_nodes);
    PingNodes(node->right, unavailable_nodes);
}

void TerminateNodes(const std::shared_ptr<Node>& node) {
    if (!node) {
        return;
    }

    TerminateNodes(node->left);
    TerminateNodes(node->right);

    bool killed = false;

    if (waitpid(node->pid, nullptr, WNOHANG) != node->pid) {
        zmq::message_t message("exit", 4);
        try {
            node->socket.send(message, zmq::send_flags::none);
            zmq::message_t reply;
            if (node->socket.recv(reply, zmq::recv_flags::none) && reply.to_string() == "Ok") {
                killed = true;
            }
        } catch (zmq::error_t&) {
            // Ignore
        }
    }

    try {
        node->socket.close();
    } catch (const zmq::error_t& e) {
        std::cerr << "Error closing socket for node " << node->id << ": " << e.what() << "\n";
    }

    if (!killed) {
        kill(node->pid, SIGKILL);
        waitpid(node->pid, nullptr, 0);
    }
}
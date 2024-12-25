#include <gtest/gtest.h>
#include <tools.h>

// Тест на создание корневого узла
TEST(NodeCreationTest, CreateRootNode) {
    root = nullptr; 
    root = InsertNode(root, 1);
    ASSERT_NE(root, nullptr); 
    EXPECT_EQ(root->id, 1); 
}

// Тест на создание дочернего узла
TEST(NodeCreationTest, CreateChildNode) {
    root = nullptr;
    root = InsertNode(root, 1); 
    auto child = InsertNode(root, 2); 
    ASSERT_NE(child, nullptr); 
    EXPECT_EQ(child->id, 2); 
}

// Тест на поиск узла
TEST(NodeCreationTest, FindNode) {
    root = nullptr; 
    root = InsertNode(root, 1); 
    InsertNode(root, 2); 
    InsertNode(root, 3); 

    auto foundNode = FindNode(root, 2); 
    ASSERT_NE(foundNode, nullptr); 
    EXPECT_EQ(foundNode->id, 2); 
}

// Тест на проверку доступности узлов
TEST(NodeCreationTest, PingNode) {
    root = nullptr; 
    root = InsertNode(root, 1);
    std::unordered_set<int> unavailable_nodes;
    PingNodes(root, unavailable_nodes); 
    EXPECT_FALSE(unavailable_nodes.empty()); 
}

// Тест на завершение работы узлов
TEST(NodeCreationTest, TerminateNodes) {
    root = nullptr;
    root = InsertNode(root, 1); 
    TerminateNodes(root); 
    EXPECT_EQ(root->left, nullptr); 
    EXPECT_EQ(root->right, nullptr);
}
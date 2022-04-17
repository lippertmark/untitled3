#include <catch2/catch.hpp>

#include <algorithm>  // find, min_element, max_element
#include <sstream>    // stringstream
#include <vector>

#include "utils.hpp"                    // print_tree
#include "assignment/cartesian_tree.hpp"  // CartesianTree

using assignment::CartesianTree;
using assignment::Node;
using Catch::Contains;

void check_children_null(const Node* node) {
  CHECK(node->left == nullptr);
  CHECK(node->right == nullptr);
}

void check_key_value(const Node* node, int key, int value) {
  CHECK(node->key == key);
  CHECK(node->priority == value);
}


TEST_CASE("CartesianTree constructor") {

  CartesianTree tree;  // default constructor

  CHECK(tree.IsEmpty());  // tree.IsEmpty() == true?
  CHECK(tree.root() == nullptr);
}

TEST_CASE("CartesianTree::Clear") {
  CartesianTree tree;

  const auto keys = std::vector<int>{8, 3, 10, 1, 6, 14, 4, 7, 13};
  const auto values = std::vector<int>{0, 1, 2, 3, 4, 6, 9, 10, 13};

  for (int index = 0; index < keys.size(); ++index) {
    tree.Insert(keys[index], values[index]);
  }

  REQUIRE_FALSE(tree.IsEmpty());
  REQUIRE(tree.root() != nullptr);

  tree.Clear();

  CHECK(tree.IsEmpty());
  CHECK(tree.root() == nullptr);
}

TEST_CASE("CartesianTree::Split") {

}

TEST_CASE("CartesianTree::Merge") {

}

TEST_CASE("CartesianTree::Insert") {
  CartesianTree tree;

  REQUIRE(tree.IsEmpty());

  // 0
  tree.Insert(7, 10);

  CHECK_FALSE(tree.IsEmpty());

  const Node* check_node = tree.root();

  REQUIRE(check_node != nullptr);
  check_children_null(check_node);
  check_key_value(check_node, 7, 10);

  // 1
  tree.Insert(4, 6);

  check_node = tree.root()->left;

  REQUIRE(check_node != nullptr);
  check_children_null(check_node);
  check_key_value(check_node, 4, 6);

  // 2
  tree.Insert(13, 8);

  check_node = tree.root()->right;

  REQUIRE(check_node != nullptr);
  check_children_null(check_node);
  check_key_value(check_node, 13, 8);

  // 3
  tree.Insert(2, 4);

  check_node = tree.root()->left->left;

  REQUIRE(check_node != nullptr);
  check_children_null(check_node);
  check_key_value(check_node, 2, 4);

  // 4
  tree.Insert(6, 2);

  check_node = tree.root()->left->right;

  REQUIRE(check_node != nullptr);
  check_children_null(check_node);
  check_key_value(check_node, 6, 2);

  // 5
  tree.Insert(9, 7);

  check_node = tree.root()->right->left;

  REQUIRE(check_node != nullptr);
  check_children_null(check_node);
  check_key_value(check_node, 9, 7);

  // 6
  tree.Insert(14, 4);

  check_node = tree.root()->right->right;

  REQUIRE(check_node != nullptr);
  check_children_null(check_node);
  check_key_value(check_node, 14, 4);

  // 7
  tree.Insert(0, 3);

  check_node = tree.root()->left->left->left;

  REQUIRE(check_node != nullptr);
  check_children_null(check_node);
  check_key_value(check_node, 0, 3);

  // 8
  tree.Insert(3, 3);

  check_node = tree.root()->left->left->right;

  REQUIRE(check_node != nullptr);
  check_children_null(check_node);
  check_key_value(check_node, 3, 3);

  // 9
  tree.Insert(5, 1);

  check_node = tree.root()->left->right->left;

  REQUIRE(check_node != nullptr);
  check_children_null(check_node);
  check_key_value(check_node, 5, 1);

  // 10
  tree.Insert(11, 3);

  check_node = tree.root()->right->left->right;

  REQUIRE(check_node != nullptr);
  check_children_null(check_node);
  check_key_value(check_node, 11, 3);




  // check nullptr nodes
  CHECK(tree.root()->left->right->right == nullptr);
  CHECK(tree.root()->right->left->left == nullptr);
  CHECK(tree.root()->right->right->left == nullptr);
  CHECK(tree.root()->right->right->right == nullptr);

  // check tree's string representation (BFS)
  const auto tree_str = utils::tree_as_str(tree.root());
  CHECK_THAT(tree_str, Contains("7 4 13 2 6 9 14 0 3 5 11"));
}

TEST_CASE("CartesianTree::Contains") {
  CartesianTree tree;

  const auto keys = std::vector<int>{8, 3, 10, 1, 6, 14, 4, 7, 13};
  const auto values = std::vector<int>{0, 1, 2, 3, 4, 6, 9, 10, 13};

  for (int index = 0; index < keys.size(); ++index) {
    tree.Insert(keys[index], values[index]);
  }

  REQUIRE_FALSE(tree.IsEmpty());
  REQUIRE(tree.root() != nullptr);

  SECTION("contains existing keys") {
    for (int key : keys) {
      CHECK(tree.Contains(key));
    }
  }

  SECTION("contains non-existing keys") {
    const auto pred = [&keys](int key) { return std::find(keys.cbegin(), keys.cend(), key) == keys.cend(); };
    const int key = GENERATE_REF(filter(pred, range(-50, 50)));

    CHECK_FALSE(tree.Contains(key));
  }
}

TEST_CASE("CertasianTree::Remove") {
  CartesianTree tree;

  auto keys = std::vector<int>{8, 3, 10, 1, 6, 14, 4, 7, 13};
  const auto values = std::vector<int>{0, 1, 2, 3, 4, 6, 9, 10, 13};

  for (int index = 0; index < keys.size(); ++index) {
    tree.Insert(keys[index], values[index]);
  }

  REQUIRE_FALSE(tree.IsEmpty());
  REQUIRE(tree.root() != nullptr);

  SECTION("remove nodes with no children") {
    const int remove_key = GENERATE(1, 4, 7, 13);

    keys.erase(std::remove(keys.begin(), keys.end(), remove_key), keys.end());

    CHECK(tree.Remove(remove_key));
    CHECK_FALSE(tree.Contains(remove_key));

    // check tree's string representation (BFS)
    std::stringstream stream;

    for (int key : keys) {
      stream << key << ' ';
    }

    const auto tree_bfs_str = stream.str();

    const auto tree_str = utils::tree_as_str(tree.root());
    CHECK_THAT(tree_str, Contains(tree_bfs_str));
  }

  SECTION("remove nodes with one child") {

    SECTION("remove node with left child") {
      const int remove_key = 14;

      CHECK(tree.Remove(remove_key));
      CHECK_FALSE(tree.Contains(remove_key));

      // check tree's string representation (BFS)
      const auto tree_str = utils::tree_as_str(tree.root());
      CHECK_THAT(tree_str, Contains("8 3 10 1 6 13 4 7"));
    }

    SECTION("remove node with right child") {
      const int remove_key = 10;

      CHECK(tree.Remove(remove_key));
      CHECK_FALSE(tree.Contains(remove_key));

      // check tree's string representation (BFS)
      const auto tree_str = utils::tree_as_str(tree.root());
      CHECK_THAT(tree_str, Contains("8 3 14 1 6 13 4 7"));
    }
  }
}



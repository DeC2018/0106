#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>

using namespace std;

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

class Solution {
 public:
  TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
    unordered_map<int, int> inToIndex;
    for (int i = 0; i < inorder.size(); ++i)
      inToIndex[inorder[i]] = i;
    return build(inorder, 0, inorder.size() - 1, postorder, 0,
                 postorder.size() - 1, inToIndex);
  }

 private:
  TreeNode* build(const vector<int>& inorder, int inStart, int inEnd,
                  const vector<int>& postorder, int postStart, int postEnd,
                  const unordered_map<int, int>& inToIndex) {
    if (inStart > inEnd)
      return nullptr;

    const int rootVal = postorder[postEnd];
    const int rootInIndex = inToIndex.at(rootVal);
    const int leftSize = rootInIndex - inStart;

    TreeNode* root = new TreeNode(rootVal);
    root->left = build(inorder, inStart, rootInIndex - 1, postorder, postStart,
                       postStart + leftSize - 1, inToIndex);
    root->right = build(inorder, rootInIndex + 1, inEnd, postorder,
                        postStart + leftSize, postEnd - 1, inToIndex);
    return root;
  }
};

// Helper function to print tree in level order (as per example output)
void printTree(TreeNode* root) {
    if (!root) {
        cout << "[]" << endl;
        return;
    }
    queue<TreeNode*> q;
    q.push(root);
    vector<string> res;
    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        if (node) {
            res.push_back(to_string(node->val));
            q.push(node->left);
            q.push(node->right);
        } else {
            res.push_back("null");
        }
    }
    // Remove trailing "null"s
    while (!res.empty() && res.back() == "null") res.pop_back();
    cout << "[";
    for (size_t i = 0; i < res.size(); ++i) {
        if (i) cout << ",";
        cout << res[i];
    }
    cout << "]" << endl;
}

// Helper to parse input string like "[9,3,15,20,7]"
vector<int> parseInput(const string& s) {
    vector<int> res;
    string t = s;
    t.erase(remove(t.begin(), t.end(), '['), t.end());
    t.erase(remove(t.begin(), t.end(), ']'), t.end());
    stringstream ss(t);
    string item;
    while (getline(ss, item, ',')) {
        // Remove whitespace
        item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
        if (!item.empty())
            res.push_back(stoi(item));
    }
    return res;
}

int main() {
    vector<pair<string, string>> inputs = {
        {"[9,3,15,20,7]", "[9,15,7,20,3]"},
        {"[-1]", "[-1]"}
    };

    Solution solution;

    for (const auto& input : inputs) {
        cout << "Input: inorder = " << input.first << ", postorder = " << input.second << endl;
        vector<int> inorder = parseInput(input.first);
        vector<int> postorder = parseInput(input.second);
        TreeNode* root = solution.buildTree(inorder, postorder);
        cout << "Output: ";
        printTree(root);
    }
    return 0;
}

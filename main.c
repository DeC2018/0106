#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 10000

// Definition for a binary tree node.
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

// Globals for the build logic
int g_preorder[MAX_NODES];
int g_preorderSize = 0;
int *g_inorder;
int g_inorderSize;
int g_index[6001]; // for values in range [-3000, 3000]
int g_preIndex = 0;

// Build the tree recursively
void buildChildTree(int inStart, int inEnd, struct TreeNode **root) {
    if (g_preIndex >= g_inorderSize) {
        *root = NULL;
        return;
    }
    *root = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    int val = g_preorder[g_preIndex];
    (*root)->val = val;
    (*root)->left = NULL;
    (*root)->right = NULL;

    if (g_index[val + 3000] < inEnd) {
        g_preIndex += 1;
        buildChildTree(g_index[val + 3000] + 1, inEnd, &((*root)->right));
    }
    if (g_index[val + 3000] > inStart) {
        g_preIndex += 1;
        buildChildTree(inStart, g_index[val + 3000] - 1, &((*root)->left));
    }
}

// Main function to build tree from inorder and postorder
struct TreeNode* buildTree(int* inorder, int inorderSize, int* postorder, int postorderSize) {
    struct TreeNode *root = NULL;
    if (inorderSize == 0) {
        return NULL;
    }
    g_preorderSize = 0;
    g_inorder = inorder;
    g_inorderSize = inorderSize;
    g_preIndex = 0;
    for (int i = 0; i < inorderSize; i++) {
        g_index[inorder[i] + 3000] = i;
    }
    for (int i = postorderSize - 1; i >= 0; i--) {
        g_preorder[g_preorderSize++] = postorder[i];
    }
    buildChildTree(0, inorderSize - 1, &root);
    return root;
}

// Helper to parse input string like "[9,3,15,20,7]" into array
int parseInput(const char* s, int* arr) {
    int n = 0;
    int i = 0, sign = 1, num = 0, inNum = 0;
    while (s[i]) {
        if (s[i] == '-' || (s[i] >= '0' && s[i] <= '9')) {
            sign = 1;
            if (s[i] == '-') {
                sign = -1;
                ++i;
            }
            num = 0;
            inNum = 0;
            while (s[i] >= '0' && s[i] <= '9') {
                num = num * 10 + (s[i] - '0');
                ++i;
                inNum = 1;
            }
            if (inNum) arr[n++] = num * sign;
        } else {
            ++i;
        }
    }
    return n;
}

// Helper to print tree in level order (as per example output)
void printTree(struct TreeNode* root) {
    if (!root) {
        printf("[]\n");
        return;
    }
    struct TreeNode* queue[MAX_NODES];
    int front = 0, rear = 0;
    char* res[MAX_NODES];
    int res_len = 0;
    queue[rear++] = root;
    while (front < rear) {
        struct TreeNode* node = queue[front++];
        if (node) {
            char* buf = (char*)malloc(16);
            sprintf(buf, "%d", node->val);
            res[res_len++] = buf;
            queue[rear++] = node->left;
            queue[rear++] = node->right;
        } else {
            res[res_len++] = strdup("null");
        }
    }
    // Remove trailing "null"s
    while (res_len > 0 && strcmp(res[res_len-1], "null") == 0)
        res_len--;
    printf("[");
    for (int i = 0; i < res_len; ++i) {
        if (i) printf(",");
        printf("%s", res[i]);
        free(res[i]);
    }
    printf("]\n");
}

// Helper to free the memory of a tree
void deleteTree(struct TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

int main() {
    const char* inorderInputs[] = {
        "[9,3,15,20,7]",
        "[-1]"
    };
    const char* postorderInputs[] = {
        "[9,15,7,20,3]",
        "[-1]"
    };
    int num_cases = sizeof(inorderInputs) / sizeof(inorderInputs[0]);

    for (int i = 0; i < num_cases; ++i) {
        printf("Input: inorder = %s, postorder = %s\n", inorderInputs[i], postorderInputs[i]);
        int inorder[MAX_NODES], postorder[MAX_NODES];
        int inSize = parseInput(inorderInputs[i], inorder);
        int postSize = parseInput(postorderInputs[i], postorder);
        struct TreeNode* root = buildTree(inorder, inSize, postorder, postSize);
        printf("Output: ");
        printTree(root);
        deleteTree(root);
    }
    return 0;
}

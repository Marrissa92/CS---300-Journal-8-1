/*
 * ProjectTwo.cpp
 *
 *  Created on: Oct 14, 2026
 *      Author: Marrissa Turner
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Structure to hold individual course data fields
struct Course {
    string courseNumber;          // Unique identifier code (e.g., CSCI100)
    string title;                 // Full display name of the course
    vector<string> prerequisites; // List of prerequisite course codes
};

// Internal node structure for the Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    // Constructor to initialize a node with course data
    Node(Course aCourse) {
        course = aCourse;
        left = NULL ;
        right = NULL ;
    }
};

// ============================================================================
// Class definition containing methods to implement the BST structure
// ============================================================================
class BinarySearchTree {

private:
    Node* root;

    // Helper functions for recursive tree navigation
    Node* addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* destroyTree(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(Course course);
    void InOrder();
    Course Search(string courseNumber);
};

// --- Binary Search Tree Constructor and Memory Management ---
BinarySearchTree::BinarySearchTree() {
    root = NULL ;
}

BinarySearchTree::~BinarySearchTree() {
    root = destroyTree(root);
}

Node* BinarySearchTree::destroyTree(Node* node) {
    if (node != NULL ) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
    return NULL ;
}

// --- BST Insertion Mechanics ---
void BinarySearchTree::Insert(Course course) {
    if (root == NULL ) {
        root = new Node(course);
    } else {
        this->addNode(root, course);
    }
}

Node* BinarySearchTree::addNode(Node* node, Course course) {
    if (course.courseNumber.compare(node->course.courseNumber) < 0) {
        if (node->left == NULL ) {
            node->left = new Node(course);
        } else {
            this->addNode(node->left, course);
        }
    } else if (course.courseNumber.compare(node->course.courseNumber) > 0) {
        if (node->right == NULL ) {
            node->right = new Node(course);
        } else {
            this->addNode(node->right, course);
        }
    }
    return node;
}

// --- Alphanumeric Print Mechanics (In-Order Traversal) ---
void BinarySearchTree::InOrder() {
    this->inOrder(root);
}

void BinarySearchTree::inOrder(Node* node) {
    if (node != NULL ) {
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.title << endl;
        inOrder(node->right);
    }
}

// --- Target Course Search Mechanics ---
Course BinarySearchTree::Search(string courseNumber) {
    Node* current = root;
    while (current != NULL ) {
        if (current->course.courseNumber.compare(courseNumber) == 0) {
            return current->course;
        } else if (courseNumber.compare(current->course.courseNumber) < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    Course emptyCourse;
    return emptyCourse;
}

// ============================================================================
// Native File Parsing Function
// ============================================================================
void loadCourses(string csvPath, BinarySearchTree* bst) {
	ifstream file(csvPath.c_str());
    if (!file.is_open()) {
        cout << "Error: Target file could not be opened. Check file path name." << endl;
        return;
    }

    string line;
    int count = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 2) {
            cout << "Skipping broken input line data row." << endl;
            continue;
        }

        Course course;
        course.courseNumber = tokens[0];
        course.title = tokens[1];

        // Any remaining non-empty tokens are added as prerequisites
               for (size_t i = 2; i < tokens.size(); ++i) {
                   string prereq = tokens[i];

                   // Clean out hidden Windows carriage returns if they exist
                   if (!prereq.empty() && prereq[prereq.length() - 1] == '\r') {
                       prereq.erase(prereq.length() - 1);
                   }

                   // Only add it if it's a real course code token
                   if (!prereq.empty()) {
                       course.prerequisites.push_back(prereq);
                   }
               }

        bst->Insert(course);
        count++;
    }
    file.close();
    cout << "Successfully loaded " << count << " academic courses." << endl;
}

// ============================================================================
// Main Interactive Application Loop Window
// ============================================================================
int main() {
    BinarySearchTree* bst = new BinarySearchTree();
    string csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
    string searchKey;
    Course matchedCourse;

    int choice = 0;
    while (choice != 9) {
        cout << "\n===== ABCU Advising Assistance Program =====" << endl;
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course Details" << endl;
        cout << "9. Exit Program" << endl;
        cout << "Enter your selection option: ";

        if (!(cin >> choice)) {
            cout << "Error: Invalid entry parameters. Please use selection digits." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                loadCourses(csvPath, bst);
                break;

            case 2:
                cout << "\nHere is your alphanumeric curriculum schedule:" << endl;
                bst->InOrder();
                break;

            case 3:
                cout << "What course number are you looking for? ";
                cin >> searchKey;
                // Transform user input code to uppercase string matching rules
                transform(searchKey.begin(), searchKey.end(), searchKey.begin(), ::toupper);

                matchedCourse = bst->Search(searchKey);
                if (!matchedCourse.courseNumber.empty()) {
                    cout << "\nCourse ID: " << matchedCourse.courseNumber << endl;
                    cout << "Course Title: " << matchedCourse.title << endl;
                    cout << "Prerequisites: ";
                    if (matchedCourse.prerequisites.empty()) {
                        cout << "None" << endl;
                    } else {
                        for (size_t i = 0; i < matchedCourse.prerequisites.size(); ++i) {
                            cout << matchedCourse.prerequisites[i];
                            if (i < matchedCourse.prerequisites.size() - 1) cout << ", ";
                        }
                        cout << endl;
                    }
                } else {
                    cout << "Error: Course ID code " << searchKey << " not found in local system records." << endl;
                }
                break;

            case 9:
                cout << "Thank you for using the Advising Assistance Program. Goodbye!" << endl;
                break;

            default:
                cout << "Error: " << choice << " is not a valid parameter option selection." << endl;
                break;
        }
    }

    delete bst;
    return 0;
}

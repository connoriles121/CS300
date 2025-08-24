#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>

using namespace std;


class Course;

// Node structure for the Binary Search Tree
struct Node {
    Course* course;
    Node* left;
    Node* right;

    // Default constructor
    Node() {
        course = nullptr;
        left = nullptr;
        right = nullptr;
    }

    // Parameterized constructor
    Node(Course* aCourse) : Node() {
        this->course = aCourse;
    }
};

// Course class to hold course information
class Course {
private:
    string courseId;
    string title;
    vector<string> prerequisites;

public:
    // Default constructor
    Course() {
        this->courseId = "";
        this->title = "";
    }

    // Parameter constructor
    Course(const string& aCourseId, const string& aTitle, const vector<string>& aPrereqs) {
        this->courseId = aCourseId;
        this->title = aTitle;
        this->prerequisites = aPrereqs;
    }

    // Accessors
    string getCourseId() const {
        return this->courseId;
    }

    string getTitle() const {
        return this->title;
    }

    vector<string> getPrerequisites() const {
        return this->prerequisites;
    }
};

//  class to manage course data
class BinarySearchTree {
private:
    Node* root;

    //  add a course to the BST
    void addNode(Node* node, Course* course) {
        // If the new course ID is less than the current node's course ID, traverse left
        if (course->getCourseId().compare(node->course->getCourseId()) < 0) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            } else {
                addNode(node->left, course);
            }
        }
        // If course ID is  greater than or equal to the current node's course ID, traverse right
        else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            } else {
                addNode(node->right, course);
            }
        }
    }

    //  print all courses in order using an in-order traversal
    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left); // Traverse left subtree
            cout << node->course->getCourseId() << ", " << node->course->getTitle() << endl; // Visit node
            inOrder(node->right); // Traverse right subtree
        }
    }

    // search for a course by its ID
    Course* searchNode(Node* node, const string& courseId) {
        // Base case: if node is null or courseId good, return  course
        if (node == nullptr || node->course->getCourseId().compare(courseId) == 0) {
            if (node != nullptr) {
                return node->course;
            }
            return nullptr;
        }

        // If courseId is less than the current node's courseId, search left
        if (courseId.compare(node->course->getCourseId()) < 0) {
            return searchNode(node->left, courseId);
        }
        // If courseId is greater than the current node's courseId, search right
        else {
            return searchNode(node->right, courseId);
        }
    }

    //  delete all nodes
    void deleteTree(Node* node) {
        if (node == nullptr) {
            return;
        }
        deleteTree(node->left);
        deleteTree(node->right);
        delete node->course;
        delete node;
    }

public:
    // Default constructor
    BinarySearchTree() {
        root = nullptr;
    }

    // clean up memory
    ~BinarySearchTree() {
        deleteTree(root);
    }

    //  add a new course
    void addCourse(Course* course) {
        if (root == nullptr) {
            root = new Node(course);
        } else {
            addNode(root, course);
        }
    }

    //  print all courses in alphanumeric order
    void printAllCourses() {
        inOrder(root);
    }

    // search for a course by its ID
    Course* searchCourse(const string& courseId) {
        return searchNode(root, courseId);
    }
};

// Function to parse a line from  CSV
Course* parseCourse(const string& line) {
    stringstream ss(line);
    string token;
    vector<string> courseData;

    // Split the line by commas and store tokens
    while (getline(ss, token, ',')) {
        courseData.push_back(token);
    }

    // Check if the line  course ID and title
    if (courseData.size() < 2) {
        return nullptr;
    }

    string courseId = courseData[0];
    string title = courseData[1];
    vector<string> prerequisites;

    // If prerequisites, add to vector
    if (courseData.size() > 2) {
        for (size_t i = 2; i < courseData.size(); ++i) {
            prerequisites.push_back(courseData[i]);
        }
    }

    return new Course(courseId, title, prerequisites);
}

// load course data from a csv
void loadCourses(BinarySearchTree* bst) {
    string filename;
    cout << "Enter the name of the file to load: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    int count = 0;
    while (getline(file, line)) {
        Course* course = parseCourse(line);
        if (course != nullptr) {
            bst->addCourse(course);
            count++;
        }
    }

    file.close();
    cout << count << " courses loaded successfully." << endl;
}

//  print the menu and get user input
int displayMenu() {
    int choice;
    cout << "\nMenu:" << endl;
    cout << "1. Load Data" << endl;
    cout << "2. Print All Courses" << endl;
    cout << "3. Find Course Information" << endl;
    cout << "9. Exit" << endl;
    cout << "Enter your choice: ";

    cin >> choice;

    // Handle invalid input
    while (cin.fail()) {
        cout << "Invalid input. Please enter a number." << endl;
        cin.clear(); // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the bad input
        cout << "Enter your choice: ";
        cin >> choice;
    }

    return choice;
}

int main() {
    BinarySearchTree* bst = new BinarySearchTree();
    int choice = 0;

    cout << "Welcome to the ABCU Advising Program!" << endl;

    while (choice != 9) {
        choice = displayMenu();

        switch (choice) {
            case 1:
                loadCourses(bst);
                break;

            case 2:
                cout << "\nHere is a list of all courses in alphanumeric order:" << endl;
                bst->printAllCourses();
                break;

            case 3: {
                string courseId;
                cout << "What course do you want to know about? ";
                cin >> courseId;

                // Capitalize the input
                for (char& c : courseId) {
                    c = toupper(c);
                }

                Course* course = bst->searchCourse(courseId);
                if (course != nullptr) {
                    cout << "\nCourse ID: " << course->getCourseId() << endl;
                    cout << "Title: " << course->getTitle() << endl;
                    vector<string> prereqs = course->getPrerequisites();
                    if (!prereqs.empty()) {
                        cout << "Prerequisites: ";
                        for (size_t i = 0; i < prereqs.size(); ++i) {
                            cout << prereqs[i];
                            if (i < prereqs.size() - 1) {
                                cout << ", ";
                            }
                        }
                        cout << endl;
                    } else {
                        cout << "Prerequisites: None" << endl;
                    }
                } else {
                    cout << "Course with ID " << courseId << " not found." << endl;
                }
                break;
            }

            case 9:
                cout << "Thank you for using the advising program. Goodbye!" << endl;
                break;

            default:
                cout << "Invalid menu choice. Please try again." << endl;
                break;
        }
    }

    // Clean memory
    delete bst;

    return 0;
}


// WEEK 1 - Student Management Implementation
// Features: Student class, Add students, View students

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

// Student Class Definition
class Student {
private:
    string indexNumber;
    string name;
    string department;
    int level;

public:
    // Constructor
    Student() : indexNumber(""), name(""), department(""), level(0) {}
    
    Student(string idx, string n, string dept, int lvl) {
        indexNumber = idx;
        name = n;
        department = dept;
        level = lvl;
    }

    // Getters
    string getIndexNumber() { return indexNumber; }
    string getName() { return name; }
    string getDepartment() { return department; }
    int getLevel() { return level; }

    // Display student info
    void display() {
        cout << left << setw(15) << indexNumber 
             << setw(25) << name 
             << setw(15) << department 
             << setw(10) << level << endl;
    }

    // Convert to string for file storage
    string toString() {
        return indexNumber + "," + name + "," + department + "," + to_string(level);
    }

    // Create student from string
    static Student fromString(string data) {
        stringstream ss(data);
        string idx, name, dept, lvlStr;
        
        getline(ss, idx, ',');
        getline(ss, name, ',');
        getline(ss, dept, ',');
        getline(ss, lvlStr, ',');
        
        return Student(idx, name, dept, stoi(lvlStr));
    }
};

// Student Management System
class StudentManagementSystem {
private:
    vector<Student> students;

public:
    // Constructor - Load existing students
    StudentManagementSystem() {
        loadStudents();
    }

    // Register new student
    void registerStudent() {
        system("cls");
        cout << "\n==========================================\n";
        cout << "        REGISTER NEW STUDENT\n";
        cout << "==========================================\n\n";
        
        string indexNumber, name, department;
        int level;

        cout << "Enter Index Number: ";
        cin >> indexNumber;
        
        // Check if student already exists
        if (findStudent(indexNumber) != nullptr) {
            cout << "\nERROR: Student with this index number already exists!\n";
            cout << "Press any key to continue...";
            cin.ignore();
            cin.get();
            return;
        }

        cout << "Enter Student Name: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "Enter Department: ";
        getline(cin, department);
        
        cout << "Enter Level (100, 200, etc.): ";
        cin >> level;

        students.push_back(Student(indexNumber, name, department, level));
        saveStudents();
        
        cout << "\nSUCCESS: Student registered successfully!\n";
        cout << "\nStudent Details:";
        cout << "\nIndex Number: " << indexNumber;
        cout << "\nName: " << name;
        cout << "\nDepartment: " << department;
        cout << "\nLevel: " << level;
        
        cout << "\n\nPress any key to continue...";
        cin.get();
    }

    // View all students
    void viewAllStudents() {
        system("cls");
        cout << "\n==========================================\n";
        cout << "        ALL REGISTERED STUDENTS\n";
        cout << "==========================================\n\n";
        
        if (students.empty()) {
            cout << "No students registered yet.\n";
        } else {
            cout << left << setw(15) << "Index Number" 
                 << setw(25) << "Name" 
                 << setw(15) << "Department" 
                 << setw(10) << "Level" << endl;
            cout << string(65, '-') << endl;

            for (int i = 0; i < students.size(); i++) {
                students[i].display();
            }
            
            cout << "\nTotal Students: " << students.size() << endl;
        }
        
        cout << "\nPress any key to continue...";
        cin.ignore();
        cin.get();
    }

    // Search student by index number
    void searchStudent() {
        system("cls");
        cout << "\n==========================================\n";
        cout << "        SEARCH STUDENT\n";
        cout << "==========================================\n\n";
        
        string indexNumber;
        cout << "Enter Index Number to search: ";
        cin >> indexNumber;

        Student* student = findStudent(indexNumber);
        
        if (student != nullptr) {
            cout << "\nSTUDENT FOUND:\n";
            cout << left << setw(15) << "Index Number" 
                 << setw(25) << "Name" 
                 << setw(15) << "Department" 
                 << setw(10) << "Level" << endl;
            cout << string(65, '-') << endl;
            student->display();
        } else {
            cout << "\nStudent not found!\n";
        }
        
        cout << "\nPress any key to continue...";
        cin.ignore();
        cin.get();
    }

    // Find student helper function
    Student* findStudent(string indexNumber) {
        for (int i = 0; i < students.size(); i++) {
            if (students[i].getIndexNumber() == indexNumber) {
                return &students[i];
            }
        }
        return nullptr;
    }

    // Save students to file
    void saveStudents() {
        ofstream file("students.txt");
        if (file.is_open()) {
            for (int i = 0; i < students.size(); i++) {
                file << students[i].toString() << endl;
            }
            file.close();
            cout << "\nData saved to students.txt\n";
        } else {
            cout << "\nERROR: Could not save to file!\n";
        }
    }

    // Load students from file
    void loadStudents() {
        ifstream file("students.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                if (!line.empty()) {
                    students.push_back(Student::fromString(line));
                }
            }
            file.close();
            cout << "Loaded " << students.size() << " students from file.\n";
        }
    }

    // Display menu
    void displayMenu() {
        cout << "\n==========================================\n";
        cout << "   WEEK 1: STUDENT MANAGEMENT SYSTEM\n";
        cout << "==========================================\n\n";
        
        cout << "1. Register New Student\n";
        cout << "2. View All Students\n";
        cout << "3. Search Student by Index\n";
        cout << "4. Save to File\n";
        cout << "5. Load from File\n";
        cout << "0. Exit\n";
        cout << "\nEnter your choice: ";
    }

    // Run the system
    void run() {
        int choice;
        do {
            displayMenu();
            cin >> choice;

            switch (choice) {
                case 1: registerStudent(); break;
                case 2: viewAllStudents(); break;
                case 3: searchStudent(); break;
                case 4: saveStudents(); 
                        cout << "\nPress any key to continue...";
                        cin.ignore();
                        cin.get();
                        break;
                case 5: loadStudents();
                        cout << "\nPress any key to continue...";
                        cin.ignore();
                        cin.get();
                        break;
                case 0:
                    saveStudents();
                    cout << "\nThank you for using Student Management System!\n";
                    break;
                default:
                    cout << "\nInvalid choice! Please try again.\n";
                    cout << "Press any key to continue...";
                    cin.ignore();
                    cin.get();
            }
        } while (choice != 0);
    }
};

// Main function
int main() {
    StudentManagementSystem system;
    system.run();
    return 0;
}
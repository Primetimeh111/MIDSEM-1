#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <map>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Enum for attendance status
enum AttendanceStatus { PRESENT, ABSENT, LATE };

// Student Class
class Student {
private:
    string indexNumber;
    string name;
    
public:
    Student() {}
    
    Student(string idx, string n) {
        indexNumber = idx;
        name = n;
    }
    
    string getIndexNumber() const { return indexNumber; }
    string getName() const { return name; }
    
    void setIndexNumber(string idx) { indexNumber = idx; }
    void setName(string n) { name = n; }
    
    void display() const {
        cout << "Index: " << indexNumber << " | Name: " << name << endl;
    }
    
    // Convert to CSV format for saving
    string toCSV() const {
        return indexNumber + "," + name;
    }
    
    // Create from CSV string
    static Student fromCSV(const string& csv) {
        size_t commaPos = csv.find(',');
        if(commaPos != string::npos) {
            string idx = csv.substr(0, commaPos);
            string name = csv.substr(commaPos + 1);
            return Student(idx, name);
        }
        return Student();
    }
};

// AttendanceSession Class
class AttendanceSession {
private:
    string courseCode;
    string date;
    string startTime;
    string duration;
    vector<string> studentIndices;
    map<string, AttendanceStatus> attendanceRecords;
    
public:
    AttendanceSession() {}
    
    AttendanceSession(string code, string d, string time, string dur) {
        courseCode = code;
        date = d;
        startTime = time;
        duration = dur;
    }
    
    // Getters
    string getCourseCode() const { return courseCode; }
    string getDate() const { return date; }
    string getStartTime() const { return startTime; }
    string getDuration() const { return duration; }
    vector<string> getStudentIndices() const { return studentIndices; }
    
    // Setters
    void setCourseCode(string code) { courseCode = code; }
    void setDate(string d) { date = d; }
    void setStartTime(string time) { startTime = time; }
    void setDuration(string dur) { duration = dur; }
    
    void addAllStudents(const vector<Student>& allStudents) {
        studentIndices.clear();
        for(const auto& student : allStudents) {
            studentIndices.push_back(student.getIndexNumber());
        }
    }
    
    void markAttendance(string index, AttendanceStatus status) {
        attendanceRecords[index] = status;
    }
    
    AttendanceStatus getAttendanceStatus(string index) const {
        auto it = attendanceRecords.find(index);
        if(it != attendanceRecords.end()) {
            return it->second;
        }
        return ABSENT;
    }
    
    bool isAttendanceMarked() const {
        return !attendanceRecords.empty();
    }
    
    void getSummary(int &present, int &absent, int &late) const {
        present = absent = late = 0;
        for(const auto& record : attendanceRecords) {
            switch(record.second) {
                case PRESENT: present++; break;
                case ABSENT: absent++; break;
                case LATE: late++; break;
            }
        }
    }
    
    void display() const {
        cout << "Course: " << courseCode << " | Date: " << date 
             << " | Time: " << startTime << " | Duration: " << duration << "hrs\n";
        if(isAttendanceMarked()) {
            int p, a, l;
            getSummary(p, a, l);
            cout << "Attendance: P:" << p << " A:" << a << " L:" << l << "\n";
        } else {
            cout << "Attendance: Not marked yet\n";
        }
    }
    
    string getFilename() const {
        string filename = "session_";
        filename += courseCode + "_";
        filename += date + ".txt";
        return filename;
    }
    
    // Save session to file
    bool saveToFile() const {
        string filename = getFilename();
        ofstream file(filename);
        
        if(!file.is_open()) {
            return false;
        }
        
        // Save session header
        file << "COURSE:" << courseCode << endl;
        file << "DATE:" << date << endl;
        file << "TIME:" << startTime << endl;
        file << "DURATION:" << duration << endl;
        file << "STUDENTS:" << studentIndices.size() << endl;
        
        // Save student indices
        for(const auto& index : studentIndices) {
            file << "INDEX:" << index << endl;
        }
        
        // Save attendance records if any
        file << "ATTENDANCE:" << attendanceRecords.size() << endl;
        for(const auto& record : attendanceRecords) {
            file << record.first << ":" << static_cast<int>(record.second) << endl;
        }
        
        file.close();
        return true;
    }
    
    // Load session from file
    bool loadFromFile(const string& filename, const vector<Student>& allStudents) {
        ifstream file(filename);
        if(!file.is_open()) {
            return false;
        }
        
        string line;
        string header;
        int studentCount = 0;
        int attendanceCount = 0;
        
        while(getline(file, line)) {
            size_t colonPos = line.find(':');
            if(colonPos == string::npos) continue;
            
            header = line.substr(0, colonPos);
            string value = line.substr(colonPos + 1);
            
            if(header == "COURSE") {
                courseCode = value;
            } else if(header == "DATE") {
                date = value;
            } else if(header == "TIME") {
                startTime = value;
            } else if(header == "DURATION") {
                duration = value;
            } else if(header == "STUDENTS") {
                studentCount = stoi(value);
            } else if(header == "INDEX") {
                studentIndices.push_back(value);
            } else if(header == "ATTENDANCE") {
                attendanceCount = stoi(value);
            } else if(attendanceCount > 0) {
                // This is an attendance record
                int statusInt = stoi(value);
                attendanceRecords[header] = static_cast<AttendanceStatus>(statusInt);
                attendanceCount--;
            }
        }
        
        file.close();
        return true;
    }
};

// Global vectors
vector<Student> students;
vector<AttendanceSession> sessions;

// File paths
const string STUDENT_FILE = "students.txt";

// Function prototypes
void displayMainMenu();
void registerStudent();
void viewAllStudents();
void searchStudentByIndex();
void displaySessionMenu();
void createLectureSession();
void viewAllSessions();
void selectAndMarkAttendance();
void viewSessionReport();
void markAttendanceForSession(AttendanceSession &session);
void saveAllData();
void loadAllData();
string toUpperCase(string str);
bool isValidIndexNumber(string index);
bool isValidDate(string date);
bool isValidTime(string time);
bool isValidDuration(string dur);
string statusToString(AttendanceStatus status);
char statusToChar(AttendanceStatus status);
AttendanceStatus charToStatus(char c);

int main() {
    // Load existing data at startup
    loadAllData();
    
    cout << "========================================\n";
    cout << "   DIGITAL ATTENDANCE SYSTEM - FINAL    \n";
    cout << "========================================\n\n";
    
    int choice;
    do {
        displayMainMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1:
                registerStudent();
                saveAllData(); // Save after each modification
                break;
            case 2:
                viewAllStudents();
                break;
            case 3:
                searchStudentByIndex();
                break;
            case 4:
                displaySessionMenu();
                break;
            case 5:
                cout << "\nExiting program. Goodbye!\n";
                saveAllData(); // Final save before exit
                break;
            default:
                cout << "\nInvalid choice! Please enter a number between 1-5.\n";
        }
        cout << endl;
    } while(choice != 5);
    
    return 0;
}

void displayMainMenu() {
    cout << "\n-------- MAIN MENU --------\n";
    cout << "1. Register New Student\n";
    cout << "2. View All Students\n";
    cout << "3. Search Student by Index\n";
    cout << "4. Attendance Session Management\n";
    cout << "5. Exit\n";
    cout << "---------------------------\n";
}

// File handling functions
void saveAllData() {
    // Save students
    ofstream studentFile(STUDENT_FILE);
    if(studentFile.is_open()) {
        for(const auto& student : students) {
            studentFile << student.toCSV() << endl;
        }
        studentFile.close();
        cout << "\n✓ Students saved to file.\n";
    } else {
        cout << "\n✗ Error: Could not save students to file.\n";
    }
    
    // Save sessions
    for(const auto& session : sessions) {
        if(session.saveToFile()) {
            cout << "✓ Session saved: " << session.getFilename() << endl;
        }
    }
}

void loadAllData() {
    // Load students
    ifstream studentFile(STUDENT_FILE);
    if(studentFile.is_open()) {
        string line;
        while(getline(studentFile, line)) {
            if(!line.empty()) {
                Student s = Student::fromCSV(line);
                students.push_back(s);
            }
        }
        studentFile.close();
        cout << "✓ Loaded " << students.size() << " students from file.\n";
    }
    
    // Load sessions - look for session_*.txt files
    int sessionCount = 0;
    for(const auto& entry : fs::directory_iterator(".")) {
        string filename = entry.path().filename().string();
        if(filename.find("session_") == 0 && filename.length() > 8) {
            AttendanceSession session;
            if(session.loadFromFile(filename, students)) {
                sessions.push_back(session);
                sessionCount++;
            }
        }
    }
    
    if(sessionCount > 0) {
        cout << "✓ Loaded " << sessionCount << " sessions from files.\n";
    }
    cout << endl;
}

string toUpperCase(string str) {
    for(char &c : str) {
        c = toupper(c);
    }
    return str;
}

bool isValidIndexNumber(string index) {
    if(index.empty()) return false;
    
    for(const auto& student : students) {
        if(toUpperCase(student.getIndexNumber()) == toUpperCase(index)) {
            cout << "\nError: Index number already exists!\n";
            return false;
        }
    }
    return true;
}

void registerStudent() {
    cout << "\n--- REGISTER NEW STUDENT ---\n";
    
    string indexNumber, name;
    
    cout << "Enter Index Number: ";
    getline(cin, indexNumber);
    
    indexNumber = toUpperCase(indexNumber);
    
    if(!isValidIndexNumber(indexNumber)) {
        return;
    }
    
    cout << "Enter Student Name: ";
    getline(cin, name);
    
    Student newStudent(indexNumber, name);
    students.push_back(newStudent);
    
    cout << "\n✓ Student registered successfully!\n";
    cout << "Total students: " << students.size() << endl;
}

void viewAllStudents() {
    cout << "\n--- ALL REGISTERED STUDENTS ---\n";
    
    if(students.empty()) {
        cout << "No students registered yet.\n";
        return;
    }
    
    cout << "Total students: " << students.size() << "\n\n";
    cout << left << setw(5) << "No." << setw(15) << "Index" << "Name\n";
    cout << "----------------------------------------\n";
    
    for(size_t i = 0; i < students.size(); i++) {
        cout << left << setw(5) << (i + 1) 
             << setw(15) << students[i].getIndexNumber() 
             << students[i].getName() << endl;
    }
}

void searchStudentByIndex() {
    cout << "\n--- SEARCH STUDENT BY INDEX ---\n";
    
    if(students.empty()) {
        cout << "No students registered yet.\n";
        return;
    }
    
    string searchIndex;
    cout << "Enter Index Number to search: ";
    getline(cin, searchIndex);
    
    searchIndex = toUpperCase(searchIndex);
    
    bool found = false;
    for(const auto& student : students) {
        if(student.getIndexNumber() == searchIndex) {
            cout << "\n✓ Student Found!\n";
            cout << "Index: " << student.getIndexNumber() << endl;
            cout << "Name: " << student.getName() << endl;
            found = true;
            break;
        }
    }
    
    if(!found) {
        cout << "\n✗ No student found with index number: " << searchIndex << endl;
    }
}

void displaySessionMenu() {
    int choice;
    
    do {
        cout << "\n----- SESSION MANAGEMENT -----\n";
        cout << "1. Create New Lecture Session\n";
        cout << "2. View All Sessions\n";
        cout << "3. Mark Attendance for a Session\n";
        cout << "4. View Session Report\n";
        cout << "5. Back to Main Menu\n";
        cout << "-------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1:
                createLectureSession();
                break;
            case 2:
                viewAllSessions();
                break;
            case 3:
                selectAndMarkAttendance();
                break;
            case 4:
                viewSessionReport();
                break;
            case 5:
                cout << "\nReturning to main menu...\n";
                break;
            default:
                cout << "\nInvalid choice!\n";
        }
    } while(choice != 5);
}

bool isValidDate(string date) {
    if(date.length() != 10) return false;
    if(date[4] != '-' || date[7] != '-') return false;
    
    for(int i = 0; i < 10; i++) {
        if(i == 4 || i == 7) continue;
        if(!isdigit(date[i])) return false;
    }
    
    // Basic range validation
    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));
    
    if(year < 2024 || year > 2026) return false;
    if(month < 1 || month > 12) return false;
    if(day < 1 || day > 31) return false;
    
    return true;
}

bool isValidTime(string time) {
    if(time.length() != 5) return false;
    if(time[2] != ':') return false;
    
    for(int i = 0; i < 5; i++) {
        if(i == 2) continue;
        if(!isdigit(time[i])) return false;
    }
    
    int hours = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));
    
    return (hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59);
}

bool isValidDuration(string dur) {
    if(dur.empty()) return false;
    for(char c : dur) {
        if(!isdigit(c)) return false;
    }
    int duration = stoi(dur);
    return (duration > 0 && duration <= 4);
}

void createLectureSession() {
    cout << "\n--- CREATE NEW LECTURE SESSION ---\n";
    
    if(students.empty()) {
        cout << "Error: No students registered. Please register students first.\n";
        return;
    }
    
    string courseCode, date, startTime, duration;
    
    cout << "Enter Course Code (e.g., EEE227): ";
    getline(cin, courseCode);
    courseCode = toUpperCase(courseCode);
    
    do {
        cout << "Enter Date (YYYY-MM-DD): ";
        getline(cin, date);
        if(!isValidDate(date)) {
            cout << "Invalid date format! Please use YYYY-MM-DD (2024-2026)\n";
        }
    } while(!isValidDate(date));
    
    do {
        cout << "Enter Start Time (HH:MM, 24-hour format): ";
        getline(cin, startTime);
        if(!isValidTime(startTime)) {
            cout << "Invalid time format! Please use HH:MM (00-23:00-59)\n";
        }
    } while(!isValidTime(startTime));
    
    do {
        cout << "Enter Duration (hours, 1-4): ";
        getline(cin, duration);
        if(!isValidDuration(duration)) {
            cout << "Invalid duration! Please enter a number between 1-4\n";
        }
    } while(!isValidDuration(duration));
    
    AttendanceSession newSession(courseCode, date, startTime, duration);
    newSession.addAllStudents(students);
    sessions.push_back(newSession);
    
    // Save immediately
    newSession.saveToFile();
    
    cout << "\n✓ Lecture session created successfully!\n";
    cout << "Session Details:\n";
    cout << "--------------------------------\n";
    newSession.display();
    cout << "--------------------------------\n";
    cout << "Total students in session: " << students.size() << endl;
}

void viewAllSessions() {
    cout << "\n--- ALL LECTURE SESSIONS ---\n";
    
    if(sessions.empty()) {
        cout << "No sessions created yet.\n";
        return;
    }
    
    cout << "Total sessions: " << sessions.size() << "\n\n";
    
    for(size_t i = 0; i < sessions.size(); i++) {
        cout << "Session #" << i + 1 << ":\n";
        sessions[i].display();
        cout << "File: " << sessions[i].getFilename() << "\n";
        cout << "--------------------------------\n";
    }
}

string statusToString(AttendanceStatus status) {
    switch(status) {
        case PRESENT: return "PRESENT";
        case ABSENT: return "ABSENT";
        case LATE: return "LATE";
        default: return "UNKNOWN";
    }
}

char statusToChar(AttendanceStatus status) {
    switch(status) {
        case PRESENT: return 'P';
        case ABSENT: return 'A';
        case LATE: return 'L';
        default: return '?';
    }
}

AttendanceStatus charToStatus(char c) {
    c = toupper(c);
    switch(c) {
        case 'P': return PRESENT;
        case 'A': return ABSENT;
        case 'L': return LATE;
        default: return ABSENT;
    }
}

void selectAndMarkAttendance() {
    cout << "\n--- MARK ATTENDANCE ---\n";
    
    if(sessions.empty()) {
        cout << "No sessions available. Please create a session first.\n";
        return;
    }
    
    cout << "\nAvailable Sessions:\n";
    for(size_t i = 0; i < sessions.size(); i++) {
        cout << i + 1 << ". ";
        sessions[i].display();
    }
    
    int sessionChoice;
    cout << "\nSelect session number to mark attendance (0 to cancel): ";
    cin >> sessionChoice;
    cin.ignore();
    
    if(sessionChoice == 0 || sessionChoice > static_cast<int>(sessions.size())) {
        cout << "Operation cancelled.\n";
        return;
    }
    
    markAttendanceForSession(sessions[sessionChoice - 1]);
}

void markAttendanceForSession(AttendanceSession &session) {
    cout << "\n--- MARKING ATTENDANCE ---\n";
    cout << "Session: ";
    session.display();
    cout << "\nInstructions: Enter P for Present, A for Absent, L for Late\n";
    cout << "------------------------------------------------\n";
    
    vector<string> studentIndices = session.getStudentIndices();
    
    for(const auto& index : studentIndices) {
        string studentName;
        for(const auto& student : students) {
            if(student.getIndexNumber() == index) {
                studentName = student.getName();
                break;
            }
        }
        
        char statusChar;
        bool validInput = false;
        
        do {
            cout << "Student: " << studentName << " [" << index << "] - Status (P/A/L): ";
            cin >> statusChar;
            cin.ignore();
            
            statusChar = toupper(statusChar);
            if(statusChar == 'P' || statusChar == 'A' || statusChar == 'L') {
                validInput = true;
            } else {
                cout << "Invalid input! Please enter P, A, or L.\n";
            }
        } while(!validInput);
        
        AttendanceStatus status = charToStatus(statusChar);
        session.markAttendance(index, status);
    }
    
    // Save after marking
    session.saveToFile();
    
    cout << "\n✓ Attendance marked and saved successfully!\n";
    
    int p, a, l;
    session.getSummary(p, a, l);
    cout << "\nSummary for this session:\n";
    cout << "Present: " << p << " | Absent: " << a << " | Late: " << l << endl;
    cout << "Total: " << (p + a + l) << " students\n";
}

void viewSessionReport() {
    cout << "\n--- VIEW SESSION REPORT ---\n";
    
    if(sessions.empty()) {
        cout << "No sessions available.\n";
        return;
    }
    
    cout << "\nAvailable Sessions:\n";
    for(size_t i = 0; i < sessions.size(); i++) {
        cout << i + 1 << ". ";
        sessions[i].display();
    }
    
    int sessionChoice;
    cout << "\nSelect session number to view report (0 to cancel): ";
    cin >> sessionChoice;
    cin.ignore();
    
    if(sessionChoice == 0 || sessionChoice > static_cast<int>(sessions.size())) {
        cout << "Operation cancelled.\n";
        return;
    }
    
    AttendanceSession &session = sessions[sessionChoice - 1];
    
    cout << "\n========== ATTENDANCE REPORT ==========\n";
    cout << "Course: " << session.getCourseCode() << "\n";
    cout << "Date: " << session.getDate() << "\n";
    cout << "Time: " << session.getStartTime() << "\n";
    cout << "Duration: " << session.getDuration() << " hours\n";
    cout << "File: " << session.getFilename() << "\n";
    cout << "========================================\n\n";
    
    if(!session.isAttendanceMarked()) {
        cout << "Attendance has not been marked for this session yet.\n";
        return;
    }
    
    cout << "ATTENDANCE LIST:\n";
    cout << "------------------------------------------------\n";
    cout << left << setw(15) << "Index" << setw(30) << "Name" << "Status\n";
    cout << "------------------------------------------------\n";
    
    vector<string> indices = session.getStudentIndices();
    for(const auto& index : indices) {
        string studentName;
        for(const auto& student : students) {
            if(student.getIndexNumber() == index) {
                studentName = student.getName();
                break;
            }
        }
        
        AttendanceStatus status = session.getAttendanceStatus(index);
        cout << left << setw(15) << index 
             << setw(30) << studentName 
             << "[" << statusToChar(status) << "] " << statusToString(status) << endl;
    }
    
    cout << "------------------------------------------------\n";
    
    int present, absent, late;
    session.getSummary(present, absent, late);
    int total = present + absent + late;
    
    cout << "\nSUMMARY:\n";
    cout << "Total Students: " << total << endl;
    cout << "Present: " << present << " (" << fixed << setprecision(1) 
         << (total > 0 ? (present * 100.0 / total) : 0) << "%)\n";
    cout << "Absent: " << absent << " (" << (total > 0 ? (absent * 100.0 / total) : 0) << "%)\n";
    cout << "Late: " << late << " (" << (total > 0 ? (late * 100.0 / total) : 0) << "%)\n";
    cout << "========================================\n";
}
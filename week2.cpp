// Add these includes at the top if not already present
#include <iomanip>
#include <sstream>

// AttendanceSession Class - Add after Student class
class AttendanceSession {
private:
    string courseCode;
    string date;
    string startTime;
    string duration;
    vector<string> studentIndices; // Store indices of students in this session
    
public:
    // Constructors
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
    
    // Add all current students to this session
    void addAllStudents() {
        studentIndices.clear();
        for(const auto& student : students) {
            studentIndices.push_back(student.getIndexNumber());
        }
    }
    
    // Display session info
    void display() const {
        cout << "Course: " << courseCode << " | Date: " << date 
             << " | Time: " << startTime << " | Duration: " << duration << "hrs\n";
        cout << "Students in session: " << studentIndices.size() << endl;
    }
};

// Global vector to store sessions
vector<AttendanceSession> sessions;

// New function prototypes
void displaySessionMenu();
void createLectureSession();
void viewAllSessions();
void displayMainMenuUpdated(); // Replace your old displayMainMenu

// Updated main function
int main() {
    int choice;
    
    cout << "========================================\n";
    cout << "   DIGITAL ATTENDANCE SYSTEM - WEEK 2   \n";
    cout << "========================================\n\n";
    
    do {
        displayMainMenuUpdated();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1:
                registerStudent();
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
                break;
            default:
                cout << "\nInvalid choice! Please enter a number between 1-5.\n";
        }
        cout << endl;
    } while(choice != 5);
    
    return 0;
}

// New menu functions
void displayMainMenuUpdated() {
    cout << "\n-------- MAIN MENU --------\n";
    cout << "1. Register New Student\n";
    cout << "2. View All Students\n";
    cout << "3. Search Student by Index\n";
    cout << "4. Attendance Session Management\n";
    cout << "5. Exit\n";
    cout << "---------------------------\n";
}

void displaySessionMenu() {
    int choice;
    
    do {
        cout << "\n----- SESSION MANAGEMENT -----\n";
        cout << "1. Create New Lecture Session\n";
        cout << "2. View All Sessions\n";
        cout << "3. Back to Main Menu\n";
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
                cout << "\nReturning to main menu...\n";
                break;
            default:
                cout << "\nInvalid choice!\n";
        }
    } while(choice != 3);
}

bool isValidDate(string date) {
    // Simple validation: format YYYY-MM-DD
    if(date.length() != 10) return false;
    if(date[4] != '-' || date[7] != '-') return false;
    
    // Check if year, month, day are digits
    for(int i = 0; i < 10; i++) {
        if(i == 4 || i == 7) continue;
        if(!isdigit(date[i])) return false;
    }
    return true;
}

bool isValidTime(string time) {
    // Simple validation: format HH:MM
    if(time.length() != 5) return false;
    if(time[2] != ':') return false;
    
    // Check if hours and minutes are digits
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
    return (duration > 0 && duration <= 4); // Max 4 hours per lecture
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
            cout << "Invalid date format! Please use YYYY-MM-DD\n";
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
    
    // Create new session
    AttendanceSession newSession(courseCode, date, startTime, duration);
    newSession.addAllStudents(); // Add all registered students to this session
    sessions.push_back(newSession);
    
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
        cout << "--------------------------------\n";
    }
}
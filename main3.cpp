// Add this enum after the includes
enum AttendanceStatus { PRESENT, ABSENT, LATE };

// Update AttendanceSession class - Replace your existing one with this
class AttendanceSession {
private:
    string courseCode;
    string date;
    string startTime;
    string duration;
    vector<string> studentIndices;
    map<string, AttendanceStatus> attendanceRecords; // index -> status
    
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
    
    // Add all current students to this session
    void addAllStudents() {
        studentIndices.clear();
        for(const auto& student : students) {
            studentIndices.push_back(student.getIndexNumber());
        }
    }
    
    // Mark attendance for a student
    void markAttendance(string index, AttendanceStatus status) {
        attendanceRecords[index] = status;
    }
    
    // Get attendance status for a student
    AttendanceStatus getAttendanceStatus(string index) const {
        auto it = attendanceRecords.find(index);
        if(it != attendanceRecords.end()) {
            return it->second;
        }
        return ABSENT; // Default if not found
    }
    
    // Check if attendance has been marked for this session
    bool isAttendanceMarked() const {
        return !attendanceRecords.empty();
    }
    
    // Get attendance summary
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
    
    // Display session info
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
    
    // Get course code
    string getCourseCodeConst() const { return courseCode; }
    string getDateConst() const { return date; }
};

// New function prototypes
void displayAttendanceMenu();
void selectAndMarkAttendance();
void viewSessionReport();
void markAttendanceForSession(AttendanceSession &session);
string statusToString(AttendanceStatus status);
char statusToChar(AttendanceStatus status);

// Update displaySessionMenu function
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

// Helper functions for attendance status
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
    
    // Display available sessions
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
    
    // Mark attendance for selected session
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
        // Find student name
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
    
    cout << "\n✓ Attendance marked successfully!\n";
    
    // Show summary
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
    
    // Display available sessions
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
    cout << "Course: " << session.getCourseCodeConst() << "\n";
    cout << "Date: " << session.getDateConst() << "\n";
    cout << "Time: " << session.getStartTime() << "\n";
    cout << "Duration: " << session.getDuration() << " hours\n";
    cout << "========================================\n\n";
    
    if(!session.isAttendanceMarked()) {
        cout << "Attendance has not been marked for this session yet.\n";
        return;
    }
    
    // Display attendance list
    cout << "ATTENDANCE LIST:\n";
    cout << "------------------------------------------------\n";
    cout << left << setw(15) << "Index" << setw(30) << "Name" << "Status\n";
    cout << "------------------------------------------------\n";
    
    vector<string> indices = session.getStudentIndices();
    for(const auto& index : indices) {
        // Find student name
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
    
    // Display summary
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
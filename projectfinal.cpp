#include <bits/stdc++.h>
using namespace std;

struct Student {
    int id;
    string name;
    string fatherName;
    string motherName;
    string dob; 
    int sem;
    string dept;
};

string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool isValidName(const string& s) {
    static const regex rx(R"(^[A-Za-z][A-Za-z .'-]{0,98}[A-Za-z]$)");
    string t = trim(s);
    return regex_match(t, rx);
}

bool isValidDOB(const string& dob) {
    static const regex rx(R"(^([0][1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/(19|20)\d{2}$)");
    if (!regex_match(dob, rx)) return false;

    int d = stoi(dob.substr(0,2));
    int m = stoi(dob.substr(3,2));
    int y = stoi(dob.substr(6,4));

    auto isLeap = [](int Y){ return (Y%4==0 && Y%100!=0) || (Y%400==0); };
    int mdays[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (m == 2 && isLeap(y)) mdays[2] = 29;
    return d >= 1 && d <= mdays[m];
}

int readInt(const string& prompt) {
    while (true) {
        cout << prompt << flush; 
        int x;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cout << "Invalid number! Please enter digits only." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readLineValidated(const string& prompt, bool (*validator)(const string&), const string& err) {
    while (true) {
        cout << prompt << flush; 
        string s;
        getline(cin, s);
        s = trim(s);
        if (validator(s)) return s;
        cout << err << "\n";
    }
}

string readDOB(const string& prompt) {
    while (true) {
        cout << prompt << flush; 
        string s;
        getline(cin, s);
        s = trim(s);
        if (isValidDOB(s)) return s;
        cout << "Invalid DOB! Use DD/MM/YYYY (e.g., 05/09/2002) and a real date.\n";
    }
}

void addStudent() {
    ofstream file("students.txt", ios::app);
    if (!file) { cout << "Error: Cannot open students.txt for writing.\n"; return; }

    Student s;
    s.id = readInt("Enter ID (number): ");

    s.name = readLineValidated("Enter Name: ",
        isValidName, "Name can contain letters, spaces, ., ', - (no digits).");
    s.fatherName = readLineValidated("Enter Father Name: ",
        isValidName, "Father name can contain letters, spaces, ., ', - (no digits).");
    s.motherName = readLineValidated("Enter Mother Name: ",
        isValidName, "Mother name can contain letters, spaces, ., ', - (no digits).");

    s.dob = readDOB("Enter Date of Birth (DD/MM/YYYY): ");

    s.sem = readInt("Enter Semester (number): ");

    s.dept = readLineValidated("Enter Department: ",
        isValidName, "Department can contain letters, spaces, ., ', - (no digits).");

    file << s.id << "|" << s.name << "|" << s.fatherName << "|" << s.motherName
         << "|" << s.dob << "|" << s.sem << "|" << s.dept << "\n";
    file.close();
    cout << "Student Added Successfully!\n";
}

void displayStudents() {
    ifstream file("students.txt");
    if (!file) { cout << "No data file found.\n"; return; }
    string line;
    cout << "\n--- Student List ---\n";
    while (getline(file, line)) {
        cout << line << "\n";
    }
}

void searchStudent() {
    ifstream file("students.txt");
    if (!file) { cout << "No data file found.\n"; return; }

    int id = readInt("Enter ID to search: ");
    string line;
    bool found = false;
    const string key = to_string(id) + "|";
    while (getline(file, line)) {
        if (line.rfind(key, 0) == 0) { 
            cout << "Student Found: " << line << "\n";
            found = true;
            break;
        }
    }
    if (!found) cout << "Student Not Found!\n";
}

void updateStudent() {
    ifstream file("students.txt");
    if (!file) { cout << "No data file found.\n"; return; }

    ofstream temp("temp.txt");
    if (!temp) { cout << "Error: Cannot open temp.txt for writing.\n"; return; }

    int id = readInt("Enter ID to update: ");
    string line;
    bool found = false;
    const string key = to_string(id) + "|";

    while (getline(file, line)) {
        if (!found && line.rfind(key, 0) == 0) {
            Student s;
            s.id = id;

            s.name = readLineValidated("Enter New Name: ",
                isValidName, "Invalid. Letters/spaces/.'- only.");
            s.fatherName = readLineValidated("Enter New Father Name: ",
                isValidName, "Invalid. Letters/spaces/.'- only.");
            s.motherName = readLineValidated("Enter New Mother Name: ",
                isValidName, "Invalid. Letters/spaces/.'- only.");
            s.dob = readDOB("Enter New Date of Birth (DD/MM/YYYY): ");
            s.sem = readInt("Enter New Semester (number): ");
            s.dept = readLineValidated("Enter New Department: ",
                isValidName, "Invalid. Letters/spaces/.'- only.");

            temp << s.id << "|" << s.name << "|" << s.fatherName << "|" << s.motherName
                 << "|" << s.dob << "|" << s.sem << "|" << s.dept << "\n";
            found = true;
        } else {
            temp << line << "\n";
        }
    }
    file.close();
    temp.close();
    remove("students.txt");
    rename("temp.txt", "students.txt");
    if (found) cout << "Student Updated Successfully!\n";
    else cout << "Student Not Found!\n";
}

void deleteStudent() {
    ifstream file("students.txt");
    if (!file) { cout << "No data file found.\n"; return; }

    ofstream temp("temp.txt");
    if (!temp) { cout << "Error: Cannot open temp.txt for writing.\n"; return; }

    int id = readInt("Enter ID to delete: ");
    string line;
    bool found = false;
    const string key = to_string(id) + "|";

    while (getline(file, line)) {
        if (line.rfind(key, 0) == 0) {
            found = true; 
        } else {
            temp << line << "\n";
        }
    }
    file.close();
    temp.close();
    remove("students.txt");
    rename("temp.txt", "students.txt");
    if (found) cout << "Student Deleted Successfully!\n";
    else cout << "Student Not Found!\n";
}

void countStudents() {
    ifstream file("students.txt");
    if (!file) { cout << "Total Students: 0\n"; return; }
    string line;
    int count = 0;
    while (getline(file, line)) count++;
    cout << "Total Students: " << count << "\n";
}
int main() {
   int choice;
    do {
        cout << "\n--- Student Management System ---" << endl; 
        cout << "1. Add Student" << endl;                     
        cout << "2. Display Students" << endl;                
        cout << "3. Search Student" << endl;                  
        cout << "4. Update Student" << endl;                  
        cout << "5. Delete Student" << endl;                  
        cout << "6. Count All Students" << endl;             
        cout << "7. Exit" << endl;                            
        cout << "Enter your choice: " << flush;               

        if (!(cin >> choice)) {
            cout << "Invalid choice! Please enter a number 1-7.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: countStudents(); break;
            case 7: cout << "Exiting...\n"; break;
            default: cout << "Invalid Choice!\n";
        }
    } while (choice != 7);
    return 0;
}
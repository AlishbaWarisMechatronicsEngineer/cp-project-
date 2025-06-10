#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 

using namespace std;

// Constants for limits
const int MAX_STUDENTS = 100;
const int MAX_ENTRIES = 20;

// Data arrays
string names[MAX_STUDENTS];
string rollNos[MAX_STUDENTS];
float quizzes[MAX_STUDENTS][MAX_ENTRIES];
float assignments[MAX_STUDENTS][MAX_ENTRIES];
float mids[MAX_STUDENTS];
float finals[MAX_STUDENTS];
float projects[MAX_STUDENTS];
float totalMarks[MAX_STUDENTS];
char grades[MAX_STUDENTS];

int studentCount = 0, quizCount = 0, assignmentCount = 0;

// Safely convert string to float
float safeAtof(const string &value) {
    return value.empty() ? 0.0f : atof(value.c_str());
}

// Load CSV file data
bool readCSVFile(const string &fileName) {
    ifstream file(fileName.c_str());
    if (!file.is_open()) {
        cout << "Could not open file.\n";
        return false;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string value;

        getline(ss, names[studentCount], ',');
        getline(ss, rollNos[studentCount], ',');

        // Read quiz scores
        int qIndex = 0;
        while (qIndex < MAX_ENTRIES && getline(ss, value, ',')) {
            if (value.empty()) break;
            quizzes[studentCount][qIndex++] = safeAtof(value);
        }
        if (quizCount == 0) quizCount = qIndex;

        // Read assignment scores
        int aIndex = 0;
        while (aIndex < MAX_ENTRIES && getline(ss, value, ',')) {
            if (value.empty()) break;
            assignments[studentCount][aIndex++] = safeAtof(value);
        }
        if (assignmentCount == 0) assignmentCount = aIndex;

        // Read remaining values
        getline(ss, value, ','); mids[studentCount] = safeAtof(value);
        getline(ss, value, ','); finals[studentCount] = safeAtof(value);
        getline(ss, value, ','); projects[studentCount] = safeAtof(value);

        studentCount++;
    }

    file.close();
    cout << "Data successfully read from the file.\n";
    return true;
}

// Input weightages and ensure they sum to 100
void getWeightages(float &quizWeight, float &assignmentWeight, float &midWeight, float &finalWeight, float &projectWeight) {
    cout << "\nEnter the weightage (in %):\n";
    cout << "Quiz: "; cin >> quizWeight;
    cout << "Assignment: "; cin >> assignmentWeight;
    cout << "Mids: "; cin >> midWeight;
    cout << "Final: "; cin >> finalWeight;
    cout << "Project: "; cin >> projectWeight;

    float total = quizWeight + assignmentWeight + midWeight + finalWeight + projectWeight;
    if (total != 100.0f) {
        cout << "Total weightage must be 100%. Exiting.\n";
        exit(1);
    }
}

// Calculate total marks for each student
void calculateTotalMarks(float qw, float aw, float mw, float fw, float pw) {
    for (int i = 0; i < studentCount; i++) {
        float qSum = 0, aSum = 0;
        for (int j = 0; j < quizCount; j++) qSum += quizzes[i][j];
        for (int j = 0; j < assignmentCount; j++) aSum += assignments[i][j];

        float qAvg = (quizCount > 0) ? qSum / quizCount : 0;
        float aAvg = (assignmentCount > 0) ? aSum / assignmentCount : 0;

        totalMarks[i] = (qAvg * qw / 100) + (aAvg * aw / 100)
                      + (mids[i] * mw / 100) + (finals[i] * fw / 100)
                      + (projects[i] * pw / 100);
    }
}

// Compute average marks of the class
float computeClassAverage() {
    float sum = 0;
    for (int i = 0; i < studentCount; i++) {
        sum += totalMarks[i];
    }
    return (studentCount > 0) ? (sum / studentCount) : 0;
}

// Assign grades based on average
void assignStudentGrades(float avg) {
    for (int i = 0; i < studentCount; i++) {
        if (totalMarks[i] >= avg + 10) grades[i] = 'A';
        else if (totalMarks[i] >= avg) grades[i] = 'B';
        else if (totalMarks[i] >= avg - 10) grades[i] = 'C';
        else grades[i] = 'D';
    }
}

// Displaystudent results
void showClassResults() {
    cout << "\nName\tRoll No\tTotal\tGrade\n";
    for (int i = 0; i < studentCount; i++) {
        cout << names[i] << "\t" << rollNos[i] << "\t" << totalMarks[i] << "\t" << grades[i] << endl;
    }
}

// Search by roll number
void searchStudentByRollNumber() {
    string roll;
    cout << "Enter roll number to search: ";
    cin >> roll;

    bool found = false;
    for (int i = 0; i < studentCount; i++) {
        if (rollNos[i] == roll) {
            cout << "\nName: " << names[i]
                 << "\nRoll No: " << rollNos[i]
                 << "\nTotal Marks: " << totalMarks[i]
                 << "\nGrade: " << grades[i] << endl;
            found = true;
            break;
        }
    }
    if (!found) cout << "? Student not found.\n";
}

// Main program
int main() {
    string fileName;
    float quizWeight, assignmentWeight, midWeight, finalWeight, projectWeight;
    int fileChoice;
    bool fileLoaded = false;

    while (!fileLoaded) {
        cout << "Choose a file to load:\n1. nca.csv\n2. cp.csv\nChoose option: ";
        cin >> fileChoice;

        if (fileChoice == 1)
            fileName = "C:\\Users\\HP\\OneDrive\\Desktop\\New folder\\NCA.csv";
        else if (fileChoice == 2)
            fileName = "C:\\Users\\HP\\OneDrive\\Desktop\\New folder\\CP.csv";
        else {
            cout << "Invalid option. Try again.\n";
            continue;
        }

        fileLoaded = readCSVFile(fileName);
        if (!fileLoaded) cout << "? File load failed. Try again.\n";
    }

    getWeightages(quizWeight, assignmentWeight, midWeight, finalWeight, projectWeight);
    calculateTotalMarks(quizWeight, assignmentWeight, midWeight, finalWeight, projectWeight);

    float classAverage = computeClassAverage();
    assignStudentGrades(classAverage);

    int userChoice;
    do {
        cout << "\nMenu:\n1. Show Class Result\n2. Search Student\n3. Exit\nChoose option: ";
        cin >> userChoice;

        if (userChoice == 1) showClassResults();
        else if (userChoice == 2) searchStudentByRollNumber();
        else if (userChoice != 3) cout << "Invalid choice. Try again.\n";

    } while (userChoice != 3);

    return 0;
}


#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <string>
#include <regex>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <unordered_map> // Include this header for unordered_map

using namespace std;

// Data Structures
struct Voter {
    string cnic; // Add CNIC to the Voter struct
    string name;
    string father_name;
    string date_of_birth;
    string gender; // Add gender field
    bool voted = false; // Indicates if the voter has voted
};

struct Candidate {
    string candidate_id; // Add Candidate ID to the Candidate struct
    string name;
    string party;
    string date_of_birth;
    int votes = 0; // Initialize votes to 0
};

// Data Storage
list<Voter> voter_database;  // List of voters
list<Candidate> candidate_database;  // List of candidates
unordered_map<string, int> votes;  // Key: Candidate ID, Value: Vote Count

// Admin credentials
const string ADMIN_USERNAME = "admin";
const string ADMIN_PASSWORD = "admin123";

// Function Declarations
bool is_numeric(const string &str);
bool is_alphabetic(const string &str);
bool is_valid_date(const string &date);
string get_valid_cnic();
string get_valid_name(const string &prompt);
string get_valid_date();
void add_voter();
void update_voter();
void add_candidate();
void update_candidate();
void save_voters(const string &filename);
void save_candidates(const string &filename);
void load_voters(const string &filename);
void load_candidates(const string &filename);
void save_votes(const string &filename); // New function to save votes
void load_votes(const string &filename); // New function to load votes
bool authenticate_admin();
bool authenticate_voter(const string &cnic);
void cast_vote(const string &cnic, int candidate_index);
void display_all_voters();
void display_all_candidates();
void search_voter();
void search_candidate();
void search_voter_by_name();
void search_candidate_by_name();
void sort_and_display_voters();
void sort_and_display_candidates();
void display_results(); // Function to display election results

// Utility Functions
bool is_numeric(const string &str) {
    return all_of(str.begin(), str.end(), ::isdigit);
}

bool is_alphabetic(const string &str) {
    // Allow alphabetic characters and spaces
    return all_of(str.begin(), str.end(), [](char c) { return isalpha(c) || isspace(c); });
}

// Validate date in DD/MM/YYYY format
bool is_valid_date(const string &date) {
    regex date_pattern(R"(\d{2}/\d{2}/\d{4})"); // Match DD/MM/YYYY
    if (!regex_match(date, date_pattern)) {
        return false;
    }

    int day, month, year;
    sscanf(date.c_str(), "%d/%d/%d", &day, &month, &year);

    // Check if month is valid
    if (month < 1 || month > 12) {
        return false;
    }

    // Check if day is valid for the given month
    if (day < 1 || day > 31) {
        return false;
    }

    // Check for months with 30 days
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return false;
    }

    // Check for February
    bool is_leap_year = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    if (month == 2 && (is_leap_year ? day > 29 : day > 28)) {
        return false;
    }

    return true;
}

// Input Validation Wrappers
string get_valid_cnic() {
    string cnic;
    while (true) {
        cout << "Enter CNIC (13 digits): ";
        cin >> cnic;
        if (is_numeric(cnic) && cnic.length() == 13) {
            return cnic;
        } else {
            cout << "Invalid CNIC. It must be 13 digits. Try again.\n";
        }
    }
}

string get_valid_name(const string &prompt) {
    string name;
    while (true) {
        cout << prompt;
        getline(cin >> ws, name); // Use getline to read the full line and ignore leading whitespace
        if (!name.empty() && is_alphabetic(name)) {
            return name;
        } else {
            cout << "Invalid name. It must contain only alphabetic characters and spaces. Try again.\n";
        }
    }
}

string get_valid_date() {
    string date;
    while (true) {
        cout << "Enter Date of Birth (DD/MM/YYYY): ";
        cin >> date;
        if (is_valid_date(date)) {
            return date;
        } else {
            cout << "Invalid date. Please use the format DD/MM/YYYY. Try again.\n";
        }
    }
}

// Add Voter
void add_voter() {
    string cnic = get_valid_cnic();
    string name = get_valid_name("Enter Name: ");
    string father_name = get_valid_name("Enter Father's Name: ");
    string date_of_birth = get_valid_date();
    
    string gender;
    while (true) {
        cout << "Enter Gender (Male/Female/Other): ";
        cin >> gender;
        if (gender == "Male" || gender == "Female" || gender == "Other") {
            break; // Valid gender input
        } else {
            cout << "Invalid gender. Please enter Male, Female, or Other.\n";
        }
    }

    // Check if voter already exists
    for (const auto& voter : voter_database) {
        if (voter.cnic == cnic) {
            cout << "Voter with this CNIC already exists.\n";
            return;
        }
    }

    voter_database.push_back({cnic, name, father_name, date_of_birth, gender}); // Store voter
    cout << "Voter " << name << " added successfully.\n";
}
// update voters

void update_voter() {
    string cnic = get_valid_cnic();
    for (auto& voter : voter_database) {
        if (voter.cnic == cnic) {
            cout << "Updating details for " << voter.name << ":\n";
            while (true) {
                cout << "What would you like to update?\n";
                cout << "1. Name\n";
                cout << "2. Father's Name\n";
                cout << "3. Date of Birth\n";
                cout << "4. Gender\n";
                cout << "5. Exit Update\n";
                cout << "Enter your choice: ";
                int choice;
                cin >> choice;

                switch (choice) {
                    case 1: { // Update Name
                        string name = get_valid_name("Enter new Name : ");
                        if (!name.empty()) {
                            voter.name = name;
                        }
                        break;
                    }
                    case 2: { // Update Father's Name
                        string father_name = get_valid_name("Enter new Father's Name : ");
                        if (!father_name.empty()) {
                            voter.father_name = father_name;
                        }
                        break;
                    }
                    case 3: { // Update Date of Birth
                        string date_of_birth;
                        cout << "Enter Date of Birth (DD/MM/YYYY): ";
                        cin >> date_of_birth;
                        if (!date_of_birth.empty() && is_valid_date(date_of_birth)) {
                            voter.date_of_birth = date_of_birth;
                        } else if (!date_of_birth.empty()) {
                            cout << "Invalid date. Please use the format DD/MM/YYYY.\n";
                        }
                        break;
                    }
                    case 4: { // Update Gender
                        string gender;
                        cout << "Enter new Gender (Male/Female/Other): ";
                        cin >> gender;
                        if (gender == "Male" || gender == "Female" || gender == "Other") {
                            voter.gender = gender;
                        } else if (!gender.empty()) {
                            cout << "Invalid gender. Please enter Male, Female, or Other.\n";
                        }
                        break;
                    }
                    case 5: // Exit Update
                        cout <<voter.name<<" data is updated\n"<< "Exiting update.\n";
                        return;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
            }
        }
    }
    cout << "Voter with this CNIC does not exist.\n";
}

// Add Candidate
void add_candidate() {
    string candidate_id = get_valid_cnic();  // Candidate ID validation is similar to CNIC
    string name = get_valid_name("Enter Candidate Name: ");
    string party = get_valid_name("Enter Party Name: ");
    string date_of_birth = get_valid_date();

    for (const auto& candidate : candidate_database) {
        if (candidate.candidate_id == candidate_id) {
            cout << "Candidate with this ID already exists.\n";
            return;
        }
    }

    candidate_database.push_back({candidate_id, name, party, date_of_birth}); // Store candidate
    cout << "Candidate " << name << " added successfully.\n";
}

// Update Candidate
void update_candidate() {
    string candidate_id = get_valid_cnic(); // Assuming candidate ID is validated as CNIC
    for (auto& candidate : candidate_database) {
        if (candidate.candidate_id == candidate_id) {
            cout << "Updating details for " << candidate.name << ":\n";
            while (true) {
                cout << "What would you like to update?\n";
                cout << "1. Name\n";
                cout << "2. Party\n";
                cout << "3. Date of Birth\n";
                cout << "4. Exit Update\n";
                cout << "Enter your choice: ";
                int choice;
                cin >> choice;

                switch (choice) {
                    case 1: { // Update Name
                        string name = get_valid_name("Enter new Candidate Name : ");
                        if (!name.empty()) {
                            candidate.name = name;
                        }
                        break;
                    }
                    case 2: { // Update Party
                        string party = get_valid_name("Enter new Party Name : ");
                        if (!party.empty()) {
                            candidate.party = party;
                        }
                        break;
                    }
                    case 3: { // Update Date of Birth
                        string date_of_birth;
                        cout << "Enter Date of Birth (DD/MM/YYYY): ";
                        cin >> date_of_birth;
                        if (!date_of_birth.empty() && is_valid_date(date_of_birth)) {
                            candidate.date_of_birth = date_of_birth;
                        } else if (!date_of_birth.empty()) {
                            cout << "Invalid date. Please use the format DD/MM/YYYY.\n";
                        }
                        break;
                    }
                    case 4: // Exit Update
                       cout <<candidate.name<<" data is updated\n"<< "Exiting update.\n";
                        return;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
            }
        }
    }
    cout << "Candidate with this ID does not exist.\n";
}
// Save Voter Data
void save_voters(const string &filename) {
    ofstream file(filename);
    if (!file) cerr << "Error opening file for writing: " << filename << endl;
        return;

    file << "CNIC,Name,Father's Name,Date of Birth,Gender\n"; // Header
    for (const auto &voter : voter_database) {
        file << voter.cnic << "," << voter.name << "," << voter.father_name << ","
             << voter.date_of_birth << "," << voter.gender << "\n"; // Save gender
    }
    file.close();
}

// Save Candidate Data
void save_candidates(const string &filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    file << "Candidate ID,Name,Party,Date of Birth\n"; // Header
    for (const auto &candidate : candidate_database) {
        file << candidate.candidate_id << "," << candidate.name << "," << candidate.party << ","
             << candidate.date_of_birth << "\n";
    }
    file.close();
}

// Load Voter Data
void load_voters(const string &filename) {
    ifstream file(filename);
    string line;
    getline(file, line); // Skip header line

    while (getline(file, line)) {
        stringstream ss(line);
        string cnic, name, father_name, date_of_birth, gender;

        getline(ss, cnic, ',');
        getline(ss, name, ',');
        getline(ss, father_name, ',');
        getline(ss, date_of_birth, ',');
        getline(ss, gender, ','); // Load gender

        voter_database.push_back({cnic, name, father_name, date_of_birth, gender}); // Store voter
    }
    file.close();
}

// Load Candidate Data
void load_candidates(const string &filename) {
    ifstream file(filename);
    string line;
    getline(file, line); // Skip header line

    while (getline(file, line)) {
        stringstream ss(line);
        string candidate_id, name, party, date_of_birth;

        getline(ss, candidate_id, ',');
        getline(ss, name, ',');
        getline(ss, party, ',');
        getline(ss, date_of_birth, ',');

        candidate_database.push_back({candidate_id, name, party, date_of_birth}); // Store candidate
    }
    file.close();
}

// Implementation of save_votes function
void save_votes(const string &filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    for (const auto &vote : votes) {
        file << vote.first << "," << vote.second << "\n"; // Save candidate ID and vote count
    }
    file.close();
}

// Implementation of load_votes function
void load_votes(const string &filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file for reading: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string candidate_id;
        int vote_count;
        if (getline(ss, candidate_id, ',') && (ss >> vote_count)) {
            votes[candidate_id] = vote_count; // Load candidate ID and vote count
        }
    }
    file.close();
}

// Authenticate Admin
bool authenticate_admin() {
    string username, password;
    cout << "Enter Admin Username: ";
    cin >> username;
    cout << "Enter Admin Password: ";
    cin >> password;

    return (username == ADMIN_USERNAME && password == ADMIN_PASSWORD);
}

// Authenticate Voter
bool authenticate_voter(const string &cnic) {
    for (const auto& voter : voter_database) {
        if (voter.cnic == cnic) {
            return true; // Check if the CNIC exists in the voter database
        }
    }
    return false;
}

// Cast Vote
void cast_vote(const string &cnic, int candidate_index) {
    if (candidate_index < 1 || candidate_index > candidate_database.size()) {
        cout << "Invalid candidate selection.\n";
        return;
    }

    auto it = candidate_database.begin();
    advance(it, candidate_index - 1); // Move iterator to the selected candidate
    string candidate_id = it->candidate_id;

    for (auto& voter : voter_database) {
        if (voter.cnic == cnic) {
            if (voter.voted) {
                cout << "You have already voted. Your vote will not count again.\n"; // Message for already voted
            } else {
                votes[candidate_id]++;
                voter.voted = true; // Mark the voter as having voted
                cout << "Vote cast successfully for " << it->name << ".\n";
            }
            return;
        }
    }
}

// Display All Voters
void display_all_voters() {
    cout << "\nVoter List:\n";
    cout << setw(15) << "CNIC" << setw(20) 
         << "Name" << setw(20) 
         << "Father's Name" << setw(15)
         << "DOB" << setw(10) << "Gender"
        << endl;
    for (const auto &voter : voter_database) {
        cout << setw(15) << voter.cnic 
             << setw(20) << voter.name 
             << setw(20) << voter.father_name 
             << setw(15) << voter.date_of_birth 
             << setw(10) << voter.gender 
             << endl;
    }
}

// Display All Candidates
void display_all_candidates() {
    cout << "\nCandidate List:\n";
    cout << setw(5) << "No" << setw(20) << "Name" << setw(20) << "Party" << endl;
    int serial_no = 1;
    for (const auto &candidate : candidate_database) {
        cout << setw(5) << serial_no++ 
             << setw(20) << candidate.name 
             << setw(20) << candidate.party << endl;
    }
}
// display result
void display_results() {
    cout << "\nCurrent Election Results:\n";
    cout << setw(5) << "No" 
         << setw(20) << "Candidate Name" << setw(20) << "Party" << setw(10) << "Votes" << endl;
    int serial_no = 1;
    string leading_candidate;
    int max_votes = 0; // Initialize to 0

    for (const auto &candidate : candidate_database) {
        int vote_count = votes[candidate.candidate_id]; // Get the vote count for the candidate
        cout << setw(5) << serial_no++ 
             << setw(20) << candidate.name 
             << setw(20) << candidate.party 
             << setw(10) << vote_count << endl;

        // Determine the leading candidate only if they have votes
        if (vote_count > max_votes) {
            max_votes = vote_count;
            leading_candidate = candidate.name;
        }
    }

    if (max_votes > 0) {
        cout << "\nCurrent Leading Candidate: " << leading_candidate << " with " << max_votes << " votes.\n";
    } else {
        cout << "\nNo votes have been cast yet.\n";
    }
}

// Search Voter by CNIC
void search_voter() {
    string cnic = get_valid_cnic();
    for (const auto& voter : voter_database) {
        if (voter.cnic == cnic) {
            cout << "Voter found: " << voter.name << ", Father's Name: " << voter.father_name 
                 << ", Date of Birth: " << voter.date_of_birth << ", Gender: " << voter.gender 
                 << ", Voted: " << (voter.voted ? "Yes" : "No") << endl;
            return;
        }
    }
    cout << "Voter with this CNIC does not exist.\n";
}

// Search Candidate by ID
void search_candidate() {
    string candidate_id = get_valid_cnic(); // Assuming candidate ID is also validated as CNIC
    for (const auto& candidate : candidate_database) {
        if (candidate.candidate_id == candidate_id) {
            cout << "Candidate found: " << candidate.name << ", Party: " << candidate.party << endl;
            return;
        }
    }
    cout << "Candidate with this ID does not exist.\n";
}

// Search Voter by Name
void search_voter_by_name() {
    string name;
    cout << "Enter Name to search: ";
    getline(cin >> ws, name);
    bool found = false;
    for (const auto &voter : voter_database) {
        if (voter.name == name) {
            cout << "Voter found: CNIC: " << voter.cnic << ", Name: " << voter.name 
                 << ", Father's Name: " << voter.father_name 
                 << ", Date of Birth: " << voter.date_of_birth 
                 << ", Gender: " << voter.gender 
                 << ", Voted: " << (voter.voted ? "Yes" : "No") << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No voters found with the name: " << name << endl;
    }
}

// Search Candidate by Name
void search_candidate_by_name() {
    string name;
 cout << "Enter Candidate Name to search: ";
    getline(cin >> ws, name);
    bool found = false;
    for (const auto &candidate : candidate_database) {
        if (candidate.name == name) {
            cout << "Candidate found: ID: " << candidate.candidate_id << ", Name: " << candidate.name
                 << ", Party: " << candidate.party << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No candidates found with the name: " << name << endl;
    }
}

// Sort and Display Voters
void sort_and_display_voters() {
    vector<Voter> sorted_voters(voter_database.begin(), voter_database.end());
    sort(sorted_voters.begin(), sorted_voters.end(), [](const Voter &a, const Voter &b) {
        return a.name < b.name; // Sort by name
    });

    cout << "\nSorted Voter List:\n";
    cout << setw(15) << "CNIC" << setw(20) << "Name" << setw(20) << "Father's Name" 
         << setw(15) << "DOB" << setw(10) << "Gender"  << endl;
    for (const auto &voter : sorted_voters) {
        cout << setw(15) << voter.cnic 
             << setw(20) << voter.name 
             << setw(20) << voter.father_name 
             << setw(15) << voter.date_of_birth 
             << setw(10) << voter.gender 
              << endl;
    }
}

// Sort and Display Candidates
void sort_and_display_candidates() {
    vector<Candidate> sorted_candidates(candidate_database.begin(), candidate_database.end());
    sort(sorted_candidates.begin(), sorted_candidates.end(), [](const Candidate &a, const Candidate &b) {
        return a.name < b.name; // Sort by name
    });

    cout << "\nSorted Candidate List:\n";
    cout << setw(5) << "ID" << setw(20) << "Name" << setw(20) << "Party" << endl;
    for (const auto &candidate : sorted_candidates) {
        cout << setw(5) << candidate.candidate_id 
             << setw(20) << candidate.name 
             << setw(20) << candidate.party << endl;
    }
}

// Main Program
int main() {
    cout << "Welcome to the Secure Voting System\n";

    // Load data from files
    load_voters("voters.txt");
    load_candidates("candidates.txt");
    load_votes("votes.txt"); // Load votes when the program starts

    while (true) {
        cout << "\nAre you a Staff Member or a Voter?\n";
        cout << "1. Staff Member\n";
        cout << "2. Voter\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        int role_choice;
        cin >> role_choice;

        if (role_choice == 1) {
            // Staff Member Menu
            if (authenticate_admin()) {
                while (true) {
                    cout << "\nStaff Member Menu:\n";
                    cout << "1. Add Voter\n";
                    cout << "2. Update Voter\n";
                    cout << "3. Add Candidate\n";
                    cout << "4. Update Candidate\n";
                    cout << "5. Display Voters\n";
                    cout << "6. Display Candidates\n";
                    cout << "7. Search Voter by CNIC\n";
                    cout << "8. Search Candidate by ID\n";
                    cout << "9. Search Voter by Name\n";
                    cout << "10. Search Candidate by Name\n";
                    cout << "11. Sort and Display Voters\n";
                    cout << "12. Sort and Display Candidates\n";
                    cout << "13. Save and Exit\n";
                    cout << "14. View Election Results\n"; // Corrected line
                    cout << "Enter your choice: ";
                    int staff_choice;
                    cin >> staff_choice;

                    if (staff_choice == 1) {
                        add_voter();
                    } else if (staff_choice == 2) {
                        update_voter();
                    } else if (staff_choice == 3) {
                        add_candidate();
                    } else if (staff_choice == 4) {
                        update_candidate();
                    } else if (staff_choice == 5) {
                        display_all_voters();
                    } else if (staff_choice == 6) {
                        display_all_candidates();
                    } else if (staff_choice == 7) {
                        search_voter();
                    } else if (staff_choice ==  8) {
                        search_candidate();
                    } else if (staff_choice == 9) {
                        search_voter_by_name();
                    } else if (staff_choice == 10) {
                        search_candidate_by_name();
                    } else if (staff_choice == 11) {
                        sort_and_display_voters();
                    } else if (staff_choice == 12) {
                        sort_and_display_candidates();
                    } else if (staff_choice == 13) {
                        save_voters("voters.txt");
                        save_candidates("candidates.txt");
                        save_votes("votes.txt"); // Save votes when exiting
                        cout << "Data saved successfully. Exiting...\n";
                        return 0;
                    } else if (staff_choice == 14) { // Corrected line
                        display_results(); // Display election results
                    } else {
                        cout << "Invalid choice. Please try again.\n";
                    }
                }
            }
        } else if (role_choice == 2) {
            // Voter Menu
            string cnic;
            cout << "Enter CNIC: "; 
            cin >> cnic;
            if (authenticate_voter(cnic)) {
                while (true) {
                    cout << "\nVoter Menu:\n";
                    cout << "1. Display Candidates\n";
                    cout << "2. Cast Vote\n";
                    cout << "3. Exit\n";
                    cout << "Enter your choice: ";
                    int voter_choice;
                    cin >> voter_choice;

                    if (voter_choice == 1) {
                        display_all_candidates(); 
                    } else if (voter_choice == 2) {
                        int candidate_index;
                        cout << "Enter Serial Number to Vote For: "; 
                        cin >> candidate_index;
                        cast_vote(cnic, candidate_index);
                        break; // Exit voter menu after casting vote
                    } else if (voter_choice == 3) {
                        cout << "Exiting the system. Goodbye!\n";
                        break;
                    } else {
                        cout << "Invalid choice. Please try again.\n";
                    }
                }
            } else {
                cout << "Authentication failed. Please check your CNIC.\n";
            }
        } else if (role_choice == 3) {
            cout << "Exiting the system. Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
inline void line() { cout << "\n--------------------------------------\n"; }
const int MAX_Q = 10;
class Question {
private:
    string question;
    string options[4];
    int correctOption;
public:
    Question() {
        question = "";
        correctOption = 1;
    }
    Question(string q, string opt[], int correct) {
        question = q;
        for (int i = 0; i < 4; i++)
            options[i] = opt[i];
        correctOption = correct;
    }
    void displayQuestion() const {
        cout << question << endl;
        for (int i = 0; i < 4; i++)
            cout << i + 1 << ". " << options[i] << endl;
    }
    bool checkAnswer(int ans) const {
        return ans == correctOption;
    }
    friend class FileManager;
};
class FileManager {
public:
    static void saveQuestion(const Question &q) {
        ofstream fout("quiz.txt", ios::app);
        if (!fout)
            throw runtime_error("File Error!");
        fout << q.question << endl;
        for (int i = 0; i < 4; i++)
            fout << q.options[i] << endl;
        fout << q.correctOption << endl;
        fout.close();
    }
    static int loadQuestions(Question qList[]) {
        ifstream fin("quiz.txt");
        if (!fin)
            throw runtime_error("Quiz file not found!");
        int count = 0;
        string qtext, opt[4];
        int correct;
        while (getline(fin, qtext) && count < MAX_Q) {
            for (int i = 0; i < 4; i++) {
                if (!getline(fin, opt[i])) return count;
            }
            if (!(fin >> correct)) break;
            fin.ignore();
            qList[count] = Question(qtext, opt, correct);
            count++;
        }
        fin.close();
        return count;
    }
};
class User {
protected:
    string name;
public:
    User(string n = "Guest") : name(n) {}
    virtual void menu() = 0; // pure virtual
    virtual void role() { cout << "I am a generic user.\n"; }
    virtual ~User() {}
};
class Admin : virtual public User {
public:
    Admin(string n) : User(n) {}
    void role() override { cout << "I am Admin.\n"; }
    void addQuestion() {
        string q;
        string opt[4];
        int correct;
        cin.ignore();
        cout << "Enter question: ";
        getline(cin, q);
        for (int i = 0; i < 4; i++) {
            cout << "Option " << i + 1 << ": ";
            getline(cin, opt[i]);
        }
        cout << "Enter correct option number: ";
        cin >> correct;
        Question newQ(q, opt, correct);
        try {
            FileManager::saveQuestion(newQ);
            cout << "Question added successfully.\n";
        } catch (exception &e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    void menu() override {
        int ch;
        do {
            line();
            cout << "1. Add Question\n2. Exit\nEnter choice: ";
            cin >> ch;
            if (ch == 1)
                addQuestion();
        } while (ch != 2);
    }
};
class Player : virtual public User {
private:
    int score;
public:
    Player(string n) : User(n), score(0) {}
    void role() override { cout << "I am Player.\n"; }
    void startQuiz() {
        try {
            Question qList[MAX_Q];
            int total = FileManager::loadQuestions(qList);
            int ans;
            for (int i = 0; i < total; i++) {
                line();
                qList[i].displayQuestion();
                cout << "Enter your answer: ";
                cin >> ans;
                if (qList[i].checkAnswer(ans))
                    score++;
            }
            cout << "Your score: " << score << "/" << total << endl;
        } catch (exception &e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    void menu() override {
        int ch;
        do {
            line();
            cout << "1. Start Quiz\n2. Exit\nEnter choice: ";
            cin >> ch;
            if (ch == 1)
                startQuiz();
        } while (ch != 2);
    }
};
class RegisteredUser : public Admin, public Player {
public:
    RegisteredUser(string n) : User(n), Admin(n), Player(n) {}

    void menu() override {
        cout << "You can both manage and play the quiz.\n";
        Admin::menu();
        Player::menu();
    }

    void role() override {
        cout << "I am a Registered User (Admin + Player).\n";
    }
};
class Score {
    int value;

public:
    Score(int v = 0) : value(v) {}
    Score operator+(Score s) { return Score(value + s.value); }
    void display() { cout << "Total Score: " << value << endl; }
};
template <class T>
class Result {
    T data;

public:
    Result(T d) : data(d) {}
    void showResult() { cout << "Final Result: " << data << endl; }
};
class Counter {
    static int count;

public:
    Counter() { count++; }
    static void showCount() { cout << "Objects created: " << count << endl; }
};
int Counter::count = 0;
int main() {
    line();
    cout << "QUIZ MANAGEMENT SYSTEM (OOP Project)\n";
    line();

    string name;
    int role;
    cout << "Enter your name: ";
    cin >> name;

    cout << "1. Admin\n2. Player\n3. Registered User\nEnter your role: ";
    cin >> role;

    User *u = nullptr;

    if (role == 1)
        u = new Admin(name);
    else if (role == 2)
        u = new Player(name);
    else if (role == 3)
        u = new RegisteredUser(name);
    else {
        cout << "Invalid choice!\n";
        return 0;
    }
    u->role();
    u->menu();
    

    Score s1(5), s2(10);
    Score total = s1 + s2;
    total.display();

    Result<int> r(15);
    r.showResult();

    Counter c1, c2, c3;
    Counter::showCount();

    delete u;
    line();
    cout << "Program Ended Successfully.\n";
    return 0;
}

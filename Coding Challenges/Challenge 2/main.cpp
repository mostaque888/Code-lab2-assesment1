#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <limits>
using namespace std;

struct Joke {
    string question;
    string answer;
};

vector<Joke> loadJokes(const string& filename) {
    ifstream file(filename);
    vector<Joke> jokes;
    string question, answer;
    while (getline(file, question)) {
        if (!getline(file, answer)) {
            
            cerr << "Warning: incomplete joke entry ignored.\n";
            break;
        }
        if (!question.empty() && !answer.empty())
            jokes.push_back({question, answer});
    }
    return jokes;
}

int main() {
    vector<Joke> jokes = loadJokes("jokes.txt");
    if (jokes.empty()) {
        cerr << "Cannot load jokes. Put jokes.txt beside the executable\n"
             << "and run the program from that folder.\n";
        return 1;
    }

    random_device seed;
    mt19937 engine(seed());
    uniform_int_distribution<size_t> choice(0, jokes.size() - 1);

    char again = 'y';
    cout << "Random Joke Generator (" << jokes.size() << " jokes)\n";
    while (again == 'y' || again == 'Y') {
        const Joke& joke = jokes[choice(engine)];
        cout << "\n" << joke.question << "\n";
        cout << "Press Enter to reveal the answer...";
        cin.get();
        cout << joke.answer << "\n";
        cout << "Another joke? (y/n): ";
        cin >> again;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Goodbye!\n";
    return 0;
}

#include <bits/stdc++.h>
#include <execution>
#include <string>
using namespace std;
const int WORD_LENGTH = 5, COLORINGS = 3*3*3*3*3;
//const int WORDS = 5757;
const int WORDS = 10657 + 2315;

vector<string> words;
array<int, 128> freq{};
int colored[WORDS][WORDS];
inline int coloring(const int i, const int j) {
    for (char c : words[j]) freq[c]++;
    int out = 0;
    for (int k = 0; k < words[i].size(); k++) {
        out *= 3;
        if (freq[words[i][k]]) {
            freq[words[i][k]]--;
			out++;
			out += (words[i][k] == words[j][k]);
        }
    }
    for (char c : words[j]) freq[c] = 0;
    return out;
}

void narrow(bitset<WORDS> &mask, const int colors, const int idx) {
    for (int i = 0; i < WORDS; i++) {
        if (mask[i] && colored[idx][i] != colors) mask[i] = false;
    }
}

struct state {
	vector<int16_t> dict;
};


pair<float, int> eval(const state &start, int depth);

float entropy(const state &start, int w) {
	array<state, COLORINGS> outcomes;

	// Find possible outcomes
	for (int i : start.dict) outcomes[colored[w][i]].dict.push_back(i);

	// Calculate entropy
	float out = 0;
	for (int i = 0; i < COLORINGS; i++) {
		if (outcomes[i].dict.size() == 0) continue;
		float P = (float)outcomes[i].dict.size() / start.dict.size();
		out -= log2(P) * P;
	}

	return out;
}

vector<int> is(WORDS);
pair<float, int> eval(const state &start, int depth) {
	pair<float, int> out = {numeric_limits<float>::lowest(), 0};

	vector<float> evs(WORDS);
	transform(execution::par, is.begin(), is.end(), evs.begin(), [&](int i) {
		return entropy(start, i);
	});
	// Find the best highest entropy word and choose it
	for (int i = 0; i < WORDS; i++) {
		if (evs[i] > out.first) {
			out.first = evs[i]; 
			out.second = i;
		}
	}
	// Test the entropy of possible solutions again
	// These should be given priority because there is a chance
	// That picking one of them will lead to getting lucky and winning
	//for (int i : start.dict) {
		//if (evs[i] >= out.first) {
			//out.first = evs[i]; 
			//out.second = i;
		//}
	//}

	return out;
}

pair<float, int> eval(const bitset<WORDS> &mask, int depth) {
	state start;
	for (int i = 0; i < WORDS; i++) {
		if (mask[i]) start.dict.push_back(i);
	}
	return eval(start, depth);
}


void print_coloring(int c) {
	string out;
	for (int i = 0; i < 5; i++) {
		out += to_string(c%3);
		c/=3;
	}
	reverse(out.begin(), out.end());
	cout << out << endl;
}
bitset<WORDS> defMask;
int simulate(int w) {
    int steps = 0;
    bitset<WORDS> mask = defMask;
	bool first = 1;
	float bits = 0;
    while (mask.count() > 1) {
		pair<float, int> choice = eval(mask, 1);
		bits += choice.first;
		//cout << mask.count() <<","<< words[choice.second] << " " << bits << endl;
		cout << words[choice.second] << " ";
		//print_coloring(colored[choice.second][w]);
		//cout.flush();
		steps++;
		if (choice.second == w) return steps;
        narrow(mask, colored[choice.second][w], choice.second);
    }
	assert(mask.count() == 1);
	steps++;
    return steps;
}

int main() {
    ifstream in;
    in.open("words.txt");
    string line;
	int i = 0;
    while (in >> line) {
        words.push_back(line);
		i++;
    }
    in.close();
	in.open("answers.txt");
    while (in >> line) {
        words.push_back(line);
		defMask[i] = true;
		i++;
    }
	in.close();
	iota(is.begin(), is.end(), 0);

    for (int i = 0; i < WORDS; i++) {
		for (int j = 0; j < WORDS; j++) {
			colored[i][j] = coloring(i, j);
		}
    }
	// Simulation code
    /*int total = 0, count = 0;
    for (int i = 0; i < WORDS; i++) {
		if (!defMask[i]) continue;
        int x = simulate(i);
        total += x;
		count++;
		cout << words[i] << ": " << x << endl;
		cout << (double)total/count << endl;
		if (x > 6) break;
    }
	cout << total << " " << words.size() << endl;
    cout << (double)total / count << endl;// */ 
	
	cout << "Enter the colorings as a space separated list of numbers followed by a newline. \n"
		"0 corresponds to grey, 1 to yellow, and 2 to green"
		<< endl;
	bitset<WORDS> mask = defMask;

    while (mask.count() > 1) {
        auto [ev, choice] = eval(mask, 0);
        cout << "Guess: " << words[choice] << endl;
        int c = 0;
        for (int i = 0; i < 5; i++) {
            int x; cin >> x;
            c *= 3;
            c += x;
        }
        narrow(mask, c, choice);
    }

	cout << "Answer: ";
	for (int i = 0; i < WORDS; i++) {
		if (mask[i]) cout << words[i] << " ";
	}
	cout << endl;
	// */
}


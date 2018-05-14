#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

class GuessNumber {
public:
    GuessNumber() {
        rng = mt19937(random_device{}());
	    for (int i=0; i<10000; i++) {
		    stringstream os;
		    os << setfill('0') << setw(4) << i;
		    solutions.push_back(os.str());
        }
	    solutions.erase(remove_if(solutions.begin(), solutions.end(),
	    [](string& s) {
		    for (int j=0; j<4; ++j)
			    for (int k=0; k<j; ++k)
				    if (s[j]==s[k]) return true;
		    return false;
	    }), solutions.end());
    }
    void trim(int A, int B) {
        string cur = my_guess;
		solutions.erase(remove_if(solutions.begin(), solutions.end(),
		[=](string& str) {
            int AA=A, BB=B;
			for (int j=0; j<4; ++j) {
			    if (str[j]==cur[j]) --AA;
				for (int k=0; k<4; ++k)
					if (j!=k&&str[j]==cur[k]) --BB;
            }
            if (AA==0 && BB==0) return false;
			else return true;
		}), solutions.end());
    }
    string guess() {
        uniform_int_distribution<>dis(0,size()-1);
		my_guess = solutions[dis(rng)];
		return my_guess;
    }
    size_t size() {
        return solutions.size();
    }
    void show() {
        for (auto s: solutions)
            cout << s << endl;
    }
private:
	mt19937 rng;
    string my_guess;
    vector<string> solutions;
};
int main()
{

    GuessNumber gn;

	while (gn.size()>1) {
		cout << gn.guess() << endl;
		cout << "? A ? B" << endl;
		int A, B;
		cin >> A >> B;

		gn.trim(A, B);
		if (gn.size()<10) {
		    cout << "------" << endl;
		    gn.show();
		    cout << "------" << endl;
        }

        if (gn.size()==1) {
            cout << "I got it!" << endl;
            gn.show();
        }
        if (gn.size()==0)
            cout << "No way!" << endl;
	}
}

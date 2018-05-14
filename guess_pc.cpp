#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class GuessMyNumber {
public:
    GuessMyNumber() {
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
    vector<string> trim(int A, int B, string cur) {
        vector<string> sols{solutions};
		sols.erase(remove_if(sols.begin(), sols.end(),
		[=](string& str) {
            int AA=A, BB=B;
			for (int j=0; j<4; ++j) {
			    if (str[j]==cur[j]) --AA;
				for (int k=0; k<4; ++k)
					if (j!=k&&str[j]==cur[k]) --BB;
            }
            if (AA==0 && BB==0) return false;
			else return true;
		}), sols.end());
		return sols;
    }
    void set_solutions(vector<string> &sols)
    {
        solutions = sols;
    }
    size_t size() {
        return solutions.size();
    }
    void show() {
        for (auto s: solutions)
            cout << s << endl;
    }
private:
    vector<string> solutions;
};
int main()
{

    GuessMyNumber gn;
    int maxguess = 8;
	while (gn.size()>0 && maxguess-->0) {
	    cout << "Your guess: ";
	    string guess;
	    cin >> guess;
        vector<string> best_sols;
        int bestA=0, bestB=0;
		for (int A=0; A<=4; ++A) {
		    for (int B=0; B<=4-A; ++B) {
		        vector<string> sols = gn.trim(A,B,guess);
		        if (best_sols.size()<sols.size()) {
		            best_sols = sols;
		            bestA = A;
		            bestB = B;
                }
		    }
		}
		gn.set_solutions(best_sols);
        cout << bestA << "A" << bestB << "B" << endl;

        if (gn.size()==0) {
            cout << "You got it!" << endl;
        } else if (gn.size()<20) {
            cout << "-----" << endl;
            cout << "Hint:" << endl;
            gn.show();
            cout << "-----" << endl;
        }
	}
}

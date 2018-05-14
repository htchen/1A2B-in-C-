# 1A2B-in-C-

## For the course "Introduction to Programming (II)" at National Tsing Hua University

`g++ -std=c++11 guess.cpp -o guess`

`g++ -std=c++11 guess_cpp.cpp -o guess_pc`

### 猜數字

這個範例要用 C++ 寫出猜數字程式，使用者心中選一個由四個不同數字組成的字串，然後電腦會開始猜，使用者依據電腦猜的答案，提供 ?A?B 的提示。 A 代表在相同位置有正確的數字， B 則代表有整確的數字出現但是位置不對。例如答案是 1234 但是猜 3174，則要回應 1A2B，因為有一個數字 4 的位置正確，所以是 1A，然後另外兩個數字 1 和 3 也有出現，不過位置錯了，所以是 2B。

首先要規劃如何記錄可能的答案，我們可以用 `string` 來表達每個四位數，然後放在把所有可能的四位數放在 `vector` 中。
```
vector<string> solutions;
```

接下來要把所有可能的答案都產生出來，
```
	    for (int i=0; i<10000; i++) {
		    stringstream os;
		    os << setfill('0') << setw(4) << i;
		    solutions.push_back(os.str());
        }
```
用迴圈跑過所有 0 到 9999 整數。把整數寫入 `stringstream os` 中，並且設定格式為四個字元、不足的前面補零。用 `os.str()` 取出已寫入 stream 中的字串，然後用 `push_back` 放入 vector `solutions` 中。迴圈做完後，`solutions` 裡面就會有所可能的四位數字串，當中也包含了像是 1122 這樣有重複數字的字串。為了過濾掉不符規定的(有重複數字的)字串，我們可以用 `erase` 搭配 `remove_if`。
```
	    solutions.erase(remove_if(solutions.begin(), solutions.end(),
	    [](string& s) {
		    for (int j=1; j<4; ++j)
			    for (int k=0; k<j; ++k)
				    if (s[j]==s[k]) return true;
		    return false;
	    }), solutions.end());
```

`erase` 和 `remove_if` 的組合，是慣用的寫法。
先來看 `remove_if` 的作用。`remove_if` 需要三個參數，前兩個分別是 vector 的 begin() 和 end()，第三個則是用來過濾的條件。過濾的條件我們可以用 lambda function 來寫。我們造了一個 labmda function，能夠檢查一個字串當中，是否包含重複的數字，如果有的話，labmada function 就傳回 `true`，如果數字都不相同，就傳回 `false`。經過篩選之後，vector 之中所有符合過濾條件的元素，都會被移到後面，不符合條件的(可以留下來的)，則會推到 vector 的前面，而`remove_if` 本身會傳回搬移之後，該留下來的元素的結束位置 (最後一個該留下的元素的下一個位置)。所以如果我們再用 `erase` 從那個位置開始，一路清除到原本的 `end()`，就會把那些該被過濾的元素從 vector 當中移除。

同樣的做法，我們可以隨機從 solutoins 中挑選一個可能的答案，然後依據 ?A?B 的結果，把不符合條件的字串剔除。

```
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
```


完整的程式碼如下
```
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

```

我們還可以再進一步改寫，變成是使用者猜電腦的「腦中」想的數字。做法是先讓使用者輸入猜測的字串，然後試過所以可能的 ?A?B 的組合，看看哪一種狀況對使用者最不利，就答覆那一種 ?A?B。所謂最不利的狀況，就是只被刪去的不可能答案最少。也就是說，我們希望找到某一種 ?A?B 的狀況，使得當我們用 `trim` 把不符合的字串刪去之後，剩餘的 `solutions` 越多越好。基於這樣的做法，使用者需要猜測的次數應該會很多，因為不管怎麼猜，每次都會得到最差的 ?A?B 的答覆，能夠用來消去的線索最少。

底下是程式碼

```
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

```

# include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "ArgumentManager.h"
#include <stack>
#include <list>
#include <algorithm>

using namespace std;

class TooManyExpressionsError{};
class InvalidOperationError {};

const string WHITESPACE = " \n\r\t\f\v";
string ltrim(const std::string& s)
{
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

string rtrim(const std::string& s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string trim(const std::string& s)
{
	return rtrim(ltrim(s));
}

bool invalidTest(string str){ // Determines if the line is an invalid expression based off the amount of open and closed parentheses the line has
	stack<char> st;
	bool invalid = false;
	for (int i = 0; i < str.length() && invalid == false; i++) {
		if (str[i] == '(')
			st.push(str[i]);
		if (str[i] == '[')
			st.push(str[i]);
		if (str[i] == '{')
			st.push(str[i]);
		
		if (str[i] == ')')
			if (st.top() != '(')
				invalid = true;
			else
				st.pop();
		if (str[i] == ']')
			if (st.top() != '[')
				invalid = true;
			else
				st.pop();
			
		if (str[i] == '}')
			if (st.top() != '{')
				invalid = true;
			else
				st.pop();
	}
	if (invalid)
		return true;
	else if (!st.empty())
		return true;
	else
		return false;
}

int orderOfOperations(char c) { //based on a character it determine 
	try {
		switch (c) {
		case '{':
			return 0;
			break;
		case '[':
			return 0;
			break;
		case '(':
			return 0;
			break;
		case '+':
			return 1;
			break;
		case '-':
			return 1;
			break;
		case '*':
			return 2;
			break;
		case'/':
			return 2;
			break;
		case '^':
			return 3;
			break;
		default:
			throw InvalidOperationError();
			return 0;
			break;
		}
	}
	catch (InvalidOperationError) {
		cout << "Invalid Operation in string" << endl;
	}
}

bool isDigit(char c) {
	if (c == '0' || c == '1' || c == '2' || c == '3'|| c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
		return true;
	else
		return false;
}

string inFixtoPostFix(string str) { // strings are given in inFix Arithmetic notation but postFix is easier to evaluate so we will do this conversion
	string postFix;
	stack <char> operations;
	for (int i = 0; i < str.length(); i++) { // if the character is a digit add it to post fix
		if (isDigit(str[i]) ){
			postFix += str[i];
		}
		else if (str[i] == '(' || str[i] == '[' || str[i] == '{') { // if the char is an open parentheses then add add it to operation stack as a flag
			operations.push(str[i]);
		}
		else if (str[i] == ')' || str[i] == ']' || str[i] == '}') { // if char is close parentheses add all previous operation until flag
			while (operations.top()!='(' && operations.top() != '[' && operations.top() != '{' ) {
				postFix += operations.top();
				operations.pop();
			}
			operations.pop(); // pop flag
		}
		else {
			while (!operations.empty() && orderOfOperations(operations.top()) >= orderOfOperations(str[i])) {
				postFix += operations.top(); // if operation char is has less important or equal in the order of operations
				operations.pop();            //than top of opertion stack add it to posfix string & pop it
			}
			operations.push(str[i]);  // if operation char is more important in order of operations then add it to stack
		}
	}
	while (!operations.empty()) {  // add any left over operations in the operation stack to the post string
		postFix += operations.top();
		operations.pop();
	}
	return postFix;
}


int operate(int left, int right, char c) {  // this takes two intergers and evaluates based on a charcter 
	
	try {
		switch (c) {
		case '+':
			return left + right;
			break;
		case '-':
			return left - right;
			break;
		case '*':
			return left * right;
			break;
		case'/':
			return left / right;
			break;
		case '^':
			return left ^ right;
			break;
		default:
			throw InvalidOperationError();
			break;
		}
	}
	catch(InvalidOperationError){
		cout << "Operation was invalid when trying to assess two integers" << endl;
	}

}

int evaluatePostFix(string str) {  // this function evaluates postFix arithmetic.   
	stack<int> numbersToEval;     
	for (int i = 0; i < str.length(); i++) {
		if (isDigit(str[i])) {					// It does this by putting numbers into a stack
			numbersToEval.push((int)str[i]-48); // -48 is to adjust for Ascii
		}
		else {
			int right = numbersToEval.top();  // and once it hits a operator it takes the top of the stack and puts it on the right side of an expression
			numbersToEval.pop();
			int left = numbersToEval.top();   // then puts the next in stack on the left side of the expresssion 
			numbersToEval.pop();
			int ans = operate(left, right, str[i]);  // then evaluates the expression
			numbersToEval.push(ans);
		}
	}
	return numbersToEval.top();
}



int main(int argc, char argv[]) {
	ifstream inStream;
	ofstream outStream;
	string str;
	inStream.open("input13.txt");
	outStream.open("output13.txt");

	/*// argument manager stuff

	if (argc < 2)//must have 2 parameter
	{
		cout << "call this program with parameters like ./main input=input11.txt output=output11.txt" << endl;
		return 255;
	}

	ArgumentManager am(argc, argv);
	string inFileName = am.get("input");
	string outFileName = am.get("output");

	inStream.open(inFileName.c_str());
	if (inStream.fail()) {
		cout << "Input file failed to open" << endl;
	}
	outStream.open(outFileName.c_str());
	if (outStream.fail()) {
		cout << "Input file failed to open" << endl;
	}

	*/

	list<int> TravisDecodedList;
	list<int> ScarletDecodedList;


	string name = "";
	stack<int> ScarletsNegativeNumbers;
	stack<int> TravissNegativeNumbers;
	int invalidCount=0;

	list<int> Passcode;

	while (!inStream.eof()) {
		getline(inStream, str);
		str=trim(str);
		if (str != "" && str == "Scarlet") {    // determines which list is active 
			name = "Scarlet";
		}
		if (str != "" && str == "Travis") {
			name = "Travis";
		}

		if (str != "" && str!="Scarlet" && str!="Travis" &&  str.find("Passcode") == std::string::npos) { // is a line with expressions
			if (!invalidTest(str)) {                 // if false it is a valid expression
				int decoded = evaluatePostFix(inFixtoPostFix(str));
				if (decoded >= 0) {                 // if decoded is greater than 0 put it in active list
					if (name == "Scarlet")
						ScarletDecodedList.push_front(decoded);
					if (name == "Travis")
						TravisDecodedList.push_front(decoded);
				}
				else {                             // the decoded message is negative. need to add this to the negative stack
					if (name == "Scarlet") 
						ScarletsNegativeNumbers.push(decoded*-1); 
					
					if (name == "Travis")
						TravissNegativeNumbers.push(decoded*-1);
				}				
			}
			else
				invalidCount++;	

		}
		if (str.find("Passcode") != std::string::npos) {
			for (int i = 0; i < str.length(); i++) {
				if (isDigit(str[i])) {
					Passcode.push_back((int)str[i]-48);
				}
			}
		}

	}

	list<int>::iterator it;
	list<int>::iterator it2;

	while (!TravissNegativeNumbers.empty()) {
		it = TravisDecodedList.begin();
		it2 = ScarletDecodedList.begin();
		if (find(TravisDecodedList.begin(), TravisDecodedList.end(), TravissNegativeNumbers.top()) != TravisDecodedList.end()) {

			while (*it != TravissNegativeNumbers.top()) {
				++it;
			}
			TravisDecodedList.erase(it);
			TravissNegativeNumbers.pop();
		}
		else {
			while (*it2 != TravissNegativeNumbers.top()) {
				++it2;
			}
			ScarletDecodedList.erase(it2);
			TravissNegativeNumbers.pop();
		}
	}

	

	while (!ScarletsNegativeNumbers.empty()) {
		it = TravisDecodedList.begin();
		it2 = ScarletDecodedList.begin();
		if (find(ScarletDecodedList.begin(), ScarletDecodedList.end(), ScarletsNegativeNumbers.top()) != ScarletDecodedList.end()) {
			while (*it2 != ScarletsNegativeNumbers.top()) {
				++it2;
			}
			ScarletDecodedList.erase(it2);
			ScarletsNegativeNumbers.pop();
		}
		else {
			while (*it != ScarletsNegativeNumbers.top()) {
				++it;
			}
			TravisDecodedList.erase(it); 
			ScarletsNegativeNumbers.pop();
		}
	}



	outStream << "Scarlet: [";



	it2 = ScarletDecodedList.end();
	--it2;
	for (it2; it2 != ScarletDecodedList.begin();--it2) {
			outStream << *it2<< ", ";
	}
		outStream << *it2 << "]"<< endl;



		outStream << "Travis: [";
	it = TravisDecodedList.end();
	--it;
	for (it; it != TravisDecodedList.begin();--it) {
		outStream << *it << ", ";
	}
	outStream << *it <<"]"<< endl;


	ScarletDecodedList.sort();
	TravisDecodedList.sort();
	ScarletDecodedList.merge(TravisDecodedList);
	


	if(invalidCount!=0){
		it = ScarletDecodedList.begin();
		while (invalidCount != 0) {
			++it;
			invalidCount--;
		}
		ScarletDecodedList.erase(it);
	}
	


	outStream << "Decoded Passcode: ";

	for (it = ScarletDecodedList.begin(); it != ScarletDecodedList.end();++it) {
		outStream << "| "<< *it<<" ";
	}
	outStream << "|" << endl;

	
	outStream << "Actual Passcode: ";
	for (it = Passcode.begin(); it != Passcode.end(); ++it) {
		outStream << "| " << *it<<" ";
	}
	outStream << "|" << endl; 

	if (Passcode == ScarletDecodedList) {
		outStream << "Treasure Unlocked!" << endl;
	}
	else {
		outStream << "Treasure Locked" << endl;
	}

	inStream.clear();
	inStream.close();
}


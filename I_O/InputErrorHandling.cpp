#include <iostream>
#include <string>
using namespace std;

int mainInputErrorHandling()
{
	cout << "Enter numbers on separate lines to add\n"
		<< "Use Control+D followed by Enter to finish (Control+Z in Windows).\n";
	int sum{ 0 };

	if (!cin.good()) {
		cerr << "Standard input is in a bad state!" << endl;
		return 1;
	}

	while (!cin.bad()) {
		int number;
		cin >> number;
		if (cin.good()) {
			sum += number;
		}
		else if (cin.eof()) {
			break;
		}
		else if (cin.fail()) {
			cin.clear();
			string badToken;
			cin >> badToken;
			cerr << "Warning: Bad input encountered: " << badToken << endl;
		}
	}

	cout << "The sum is " << sum << endl;
	return 0;
}
#include <iostream>
using namespace std;
int main() {
	cout<<"dodamy tutaj cos";
	for (int i=0; i<10; i++) {
		//cout<<i++;
		
		// nie nie, tu cos ci nie pyklo kajo
		// powinno byc:
		cout << i++ << endl;
	}
	return 0;
}
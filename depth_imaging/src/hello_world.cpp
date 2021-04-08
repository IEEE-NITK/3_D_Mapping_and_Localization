#include <iostream>

using namespace std;

class MyClass {     // The class
  public:           // Access specifier
    MyClass(int value) {     // Constructor
      cout << "Hello World!";
      cout << value << endl;
    }
};

int main() {
  MyClass myObj(100);    // Create an object of MyClass (this will call the constructor)
  return 0;
}

// int main(){
//     cout<<"Hello world"<<endl;
//     int x;
//     cin>>x;
//     cout<<x<<endl;
// }
#include <iostream>

using namespace std;

class MyClass {     // The class
  public:           // Access specifier
  int v;
    MyClass(int value) {     // Constructor
      cout << "Hello World!";
      cout << value << endl;
      v = value;
    }

    void myfun(){
        cout<<this->v+2<<endl;
    }

    int indexofSmallestElement(double array[], int size)
        {
            int mini = array[0], idx = 0;

            for(int i = 1; i < size; i++)
            {
                if(array[i] < mini)
                    idx = i;
                    mini = array[i];        
            }

            return idx;
    }

};

int main() {
  MyClass myObj(100);    // Create an object of MyClass (this will call the constructor)
  int ans;
  double a[4] = {4, 1, 5, 3};
  ans = myObj.indexofSmallestElement(a, 4);
  cout << ans << endl;

  return 0;
}

// int main(){
//     cout<<"Hello world"<<endl;
//     int x;
//     cin>>x;
//     cout<<x<<endl;
// }
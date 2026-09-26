#include <iostream>
using namespace std;

enum class Day { Sunday = 11, Monday, Tuesday };

int main() {
  Day d = Day::Monday;
  switch (d) {
  case Day::Monday:
    cout << "Monday\n";
    break;
  case Day::Tuesday:
    cout << "Tuesday\n";
    break;
  case Day::Sunday:
    cout << "Sunday\n";
    break;
  default:
    cout << "Not a day";
  }

  return 0;
};

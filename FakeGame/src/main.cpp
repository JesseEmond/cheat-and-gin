#include <iostream>
#include <string>
using namespace std;

template <class T>
void show(const std::string& name, const T& value);

int main()
{
  int health = 100;
  int mana = 100;
  char gold = ' ';
  short miles = 5;
  float rate = .1f;
  double science = .1f;

  while (health > 0) {
    show("health", health);
    show("mana", mana);
    show("gold", gold);
    show("miles", miles);
    show("rate", rate);
    show("science", science);

    cin.get();

    --health;
    mana -= 2;
    ++gold;
    miles += 10;
    rate += .2f;
    science += .003f;
  }

  return 0;
}

  template <class T>
void show(const std::string& name, const T& value)
{
  cout << name << "(" << sizeof(T) << "): " << value << " - " << static_cast<const void*>(&value) << endl;
}

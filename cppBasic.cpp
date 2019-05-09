#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

void swap(int &a, int &b){
    int temp = a;
    a = b;
    b = temp;
}

class Comida{
public:
  string nome;
};

class Arroz: public Comida{
public:
    Arroz(int x, int y): a(x), b(y) {}
// private:
    int a;
    int b;
};

int main(){
  int *arroz;
  arroz = new int();
  cout << "Eu sou feio " << *arroz << endl;
  cin >> *arroz;
  cout << "Eu1 sou feio " << *arroz << endl;
  delete arroz;

  int a=5, b=6;
  swap(a,b);
  cout << "A = " << a << "\nB = " << b << endl;

  Arroz carioca(2,10);
  carioca.nome = "JOSE É GAY";
  cout << carioca.a << " " << carioca.b << endl;
  cout << carioca.nome  << endl;
  return 0;
}

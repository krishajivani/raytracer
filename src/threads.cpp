#include <iostream>
#include <thread>
#include <vector>

using namespace std;

int a[100];

void pt(int i)
{
  a[i] = i;
  printf("%d\n", i);
}

int main()
{

  vector<thread> v[2];

  for (int i = 0; i < 10; i++)
  {
    v[0].emplace_back(pt, i);
  }

  for (int i = 0; i < 10; i++)
  {
    v[0][i].join();
  }
  for (int i = 0; i < 10; i++)
  {
    cout << "*" << a[i] << " ";
  }

  return 0;
}
#include <cstddef>
#include <cstdlib>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "TreeMap.h"
#include "HashMap.h"



void testHash(int  repeatCount, int *Arr)
{

  aisdi::HashMap<int,std::string> H1;
  auto starttime=std::chrono::steady_clock::now();
  auto endtime=std::chrono::steady_clock::now();
  double diff=0.0;
  starttime=std::chrono::steady_clock::now();
  for (int  i = 0; i < repeatCount; ++i)
{
        unsigned int A = Arr[i];
        H1[A]="TODO";
}
  endtime=std::chrono::steady_clock::now();
  diff=std::chrono::duration_cast<std::chrono::milliseconds>(endtime-starttime).count()/1000.0;
  std::cout << "Czas wstawienia " << repeatCount << " elementow do Hashmapy: " << diff << " sekund" << std::endl;
  starttime=std::chrono::steady_clock::now();
  for( int i=0;i<repeatCount;++i)
  {
    int A=Arr[i];
    H1.remove(A);
    }
  endtime=std::chrono::steady_clock::now();
  diff=std::chrono::duration_cast<std::chrono::milliseconds>(endtime-starttime).count()/1000.0;
  std::cout << "Czas usuniecia wszystkich elementow z Hashmapy: " << diff << " sekund" << std::endl;
}
void testTrees(int  repeatCount, int *Arr)
{

  aisdi::TreeMap<int,std::string> T1;
  auto starttime=std::chrono::steady_clock::now();
  auto endtime=std::chrono::steady_clock::now();
  double diff=0.0;
  starttime=std::chrono::steady_clock::now();
  for (int  i = 0; i < repeatCount; ++i)
{
        unsigned int A = Arr[i];
        T1[A]="TODO";
}
  endtime=std::chrono::steady_clock::now();
  diff=std::chrono::duration_cast<std::chrono::milliseconds>(endtime-starttime).count()/1000.0;
  std::cout << "Czas wstawienia " << repeatCount << " elementow do Drzewa: " << diff << " sekund" << std::endl;
  starttime=std::chrono::steady_clock::now();
  for( int i=0;i<repeatCount; ++i)
  {
    unsigned int A=Arr[i];
    T1.remove(A);
    }
  endtime=std::chrono::steady_clock::now();
  diff=std::chrono::duration_cast<std::chrono::milliseconds>(endtime-starttime).count()/1000.0;
  std::cout << "Czas usuniecia wszystkich elementow z Drzewa: " << diff << " sekund" << std::endl;
}

/*namespace
{

template <typename K, typename V>
using Map = aisdi::Map<K, V>;

void perfomTest()
{
  Map<int, std::string> map;
  map[1] = "TODO";
}

} // namespace
*/
int main(int argc, char* argv[])
{
    srand(time( NULL ));
int Arr[1000000];
for(int i=0;i<1000000;++i)
{
    Arr[i]=i;
}
for(int i=0;i<1000000;++i)
{
    int random = rand()%1000000;
    int temp=Arr[i];
    Arr[i]=Arr[random];
    Arr[random]=temp;
}
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 1000000;
    testHash(repeatCount, Arr);
    testTrees(repeatCount,Arr);
  return 0;
}

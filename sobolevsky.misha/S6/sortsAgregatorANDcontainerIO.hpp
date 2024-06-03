#ifndef SORTSAGREGATORANDCONTAINERIO_HPP
#define SORTSAGREGATORANDCONTAINERIO_HPP

#include <forward_list>
#include <list>
#include <deque>
#include <fstream>
#include <algorithm>
#include <random>
#include "shakerSort.hpp"
#include "mergeSort.hpp"

namespace sobolevsky
{
  void fillListRandom(std::forward_list< int > &list, size_t size)
  {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < size; i++)
    {
      list.push_front(dis(gen));
    }
  }

  void fillListRandom(std::forward_list< double > &list, size_t size)
  {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution< float > dis(0.0, 100.0);

    for (int i = 0; i < size; i++)
    {
      list.push_front(dis(gen));
    }
  }

  template< typename Container >
  void outputContainer(std::ostream& out, const Container& container)
  {
    for (auto &item : container)
    {
      out << item << " ";
    }
    out << "\n";
  }

  template< typename T, typename Comparator >
  void sortsAgregator(std::ostream &out, size_t size, Comparator comp)
  {
    std::forward_list< T > fwdlist;
    fillListRandom(fwdlist, size);
    outputContainer(out, fwdlist);
    std::list< T > list1;
    std::deque< T > deque1;
    std::deque< T > deque2;
    std::list< T > list2;
    std::copy(fwdlist.begin(), fwdlist.end(), std::back_inserter(list1));
    std::copy(fwdlist.begin(), fwdlist.end(), std::back_inserter(deque1));
    std::copy(fwdlist.begin(), fwdlist.end(), std::back_inserter(deque2));
    std::copy(fwdlist.begin(), fwdlist.end(), std::back_inserter(list2));

    shakerSort(list1.begin(), list1.end(), comp);
    outputContainer(out, list1);

    shakerSort(deque1.begin(), deque1.end(), comp);
    outputContainer(out, deque1);

    mergeSort(list2.begin(), list2.end(), comp);
    outputContainer(out, list2);

    mergeSort(deque2.begin(), deque2.end(), comp);
    outputContainer(out, deque2);

    mergeSort(fwdlist.begin(), fwdlist.end(), comp);
    outputContainer(out, fwdlist);
  }
}

#endif

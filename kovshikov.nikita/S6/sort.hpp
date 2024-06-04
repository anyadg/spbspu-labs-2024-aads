#ifndef SORT_HPP
#define SORT_HPP
#include <utility>
#include <iterator>
#include "compare.hpp"
#include <iostream>
#include "random.hpp"
#include <string>

namespace kovshikov
{
  bool isDigit(std::string str);

  template< typename FwdIterator, typename Compare >
  void selectionSort(FwdIterator begin, FwdIterator end, Compare comp);

  template< typename ListIterator, typename Compare >
  void shellSort(ListIterator begin, ListIterator end, Compare comp);

  template< typename Iterator, typename Compare >
  void bubbleSort(Iterator begin, Iterator end, Compare comp);

  template< typename Compare, typename Deque, typename FwdList, typename List >
  std::ostream& sort(Compare comp, Deque& deque, FwdList& fwdList, List& list, std::ostream& out);
}

bool kovshikov::isDigit(std::string str)
{
  size_t size = str.length();
  for(size_t i = 0; i < size; i++)
  {
    if(!std::isdigit(str[i]))
    {
      return false;
    }
  }
  return true;
}

template< typename FwdIterator, typename Compare >
void kovshikov::selectionSort(FwdIterator begin, FwdIterator end, Compare comp)
{
  while(begin != end)
  {
    FwdIterator extreme = begin;
    FwdIterator current = begin;
    while(current != end)
    {
      if(comp(*current, *extreme))
      {
        extreme = current;
      }
      current++;
    }
    std::swap(*extreme, *begin);
    begin++;
  }
}

template< typename ListIterator, typename Compare >
void kovshikov::shellSort(ListIterator begin, ListIterator end, Compare comp)
{
  int size = std::distance(begin, end);
  for(int interval = size / 2; interval > 0; interval /= 2)
  {
    for(int i = 0; i < interval; i++)
    {
      ListIterator current = std::next(begin, i);
      ListIterator next = std::next(current, interval);
      int indexNext = i + interval;
      while(indexNext < size)
      {
        ListIterator prev = std::prev(next, interval);
        int indexPrev = indexNext - interval;
        while(indexPrev >= 0)
        {
          if(comp(*next, *prev))
          {
            std::swap(*next, *prev);
          }
          prev = std::prev(prev, interval);
          indexPrev -= interval;
        }
        next = std::next(next, interval);
        indexNext += interval;
      }
    }
  }
}

template< typename Iterator, typename Compare >
void kovshikov::bubbleSort(Iterator begin, Iterator end, Compare comp)
{
  bool isSort = false;
  bool firstIteration = true;
  while(isSort != false || firstIteration == true)
  {
    isSort = false;
    firstIteration = false;
    Iterator current = begin;
    Iterator prev = current;
    current++;
    while(current != end)
    {
      if(comp(*current, *prev))
      {
        isSort = true;
        std::swap(*current, *prev);
      }
      prev = current;
      current++;
    }
  }
}

template< typename Compare, typename Deque, typename FwdList, typename List >
std::ostream& kovshikov::sort(Compare comp, Deque& deque, FwdList& fwdList, List& list, std::ostream& out)
{
  List listBubble;
  std::copy(list.begin(), list.end(), std::back_inserter(listBubble));
  Deque dequeShell;
  std::copy(deque.begin(), deque.end(), std::back_inserter(dequeShell));
  Deque dequeBubble;
  std::copy(deque.begin(), deque.end(), std::back_inserter(dequeBubble));

  printConteiner(fwdList, out);
  selectionSort(fwdList.begin(), fwdList.end(), comp);
  printConteiner(fwdList, out);
  selectionSort(deque.begin(), deque.end(), comp);
  printConteiner(deque, out);
  shellSort(list.begin(), list.end(), comp);
  printConteiner(list, out);
  shellSort(dequeShell.begin(), dequeShell.end(), comp);
  printConteiner(dequeShell, out);
  bubbleSort(listBubble.begin(), listBubble.end(), comp);
  printConteiner(listBubble, out);
  bubbleSort(dequeBubble.begin(), dequeBubble.end(), comp);
  printConteiner(dequeBubble, out);

  return out;
}

#endif

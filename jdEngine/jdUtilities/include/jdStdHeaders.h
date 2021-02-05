#pragma once

/*****************************************************************************/
/**
 * C type objects
 */
/*****************************************************************************/
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg> //para usar argumentos variadicos:
#include <cmath>

/*****************************************************************************/
/**
 * For memory management
 */
/*****************************************************************************/
#include <memory>

/*****************************************************************************/
/**
 * STL containers
 */
/*****************************************************************************/
#include <vector>
#include <stack>
#include <map>
#include <string>
#include <set>
#include <list>
#include <forward_list>
#include <deque>
#include <queue>
#include <bitset>
#include <array>

#include <unordered_map>
#include <unordered_set>

/*****************************************************************************/
/**
 * STL algoritms & functions
 */
/*****************************************************************************/
#include <algorithm>
#include <functional>
#include <limits>
#include <iterator>

/*****************************************************************************/
/**
 * C++ Stream stuff
 */
/*****************************************************************************/
#include <fstream>
#include <iostream>
#include <iomanip>  //manejo de ips para entradas y salidas
#include <sstream>
#include <chrono>

/*****************************************************************************/
/**
 * C Types and studd
 */
/*****************************************************************************/
extern "C" { //Especifico que es codigo de C
#include <sys/types.h>
#include <sys/stat.h>
}

/*****************************************************************************/
/**
 * Windows specifics
 */
/*****************************************************************************/
#if JD_PLATFORM == JD_PLATFORM_WIN32
  //Undefine min & max
# undef min
# undef max

# if !defined(NOMINMAX) && defined(_MSC_VER)
#   define NOMINMAX   //Required to stop windows.h messing up st::min
# endif
#endif // JD_PLATFORM == JD_PLATFORM_WIN32

/*****************************************************************************/
/**
 * Linux specifics
 */
/*****************************************************************************/
#if JD_PLATFORM == JD_PLATFORM_LINUX
  //Undefine min & max
  extern "C" {
#   include <unistd.h>
#   include <dlfcn.h>
  }
#endif // JD_PLATFORM == JD_PLATFORM_WIN32

/*****************************************************************************/
/**
 * Apple specifics
 */
/*****************************************************************************/
#if JD_PLATFORM == JD_PLATFORM_OSX
//Undefine min & max
  extern "C" {
#   include <unistd.h>
#   include <sys/param.h>
#   include <CoreFundation/CoreFundation.h>
  }
#endif // JD_PLATFORM == JD_PLATFORM_WIN32

namespace jdEngineSDK {

/*****************************************************************************/
/**
 * Contenedores
 */
/*****************************************************************************/
  template<typename T, size_t S>
  using Array = std::array<T, S>;

  template<typename T, typename A = std::allocator<T>>
  using Vector = std::vector<T, A>;

  template<typename T, typename A = std::allocator<T>>
  using Deque = std::deque<T, A>;

  template<typename T,typename A=std::allocator<T>>
  using List = std::list<T, A>;

  template<typename T, typename A = std::allocator<T>>
  using ForwardList = std::forward_list<T, A>;

  template<typename T, typename C =std::less<T>,typename A = std::allocator<T>>
  using Set = std::set<T,C, A>;

  template<typename T, typename C = std::less<T>, typename A = std::allocator<T>>
  using MultiSet = std::multiset<T, C, A>;

  template<typename KT, typename T,typename C = std::less<KT>, 
           typename A = std::allocator<std::pair<const KT, T>>>
  using Map = std::map<KT,T, C, A>;

  template<typename KT, typename T, typename C = std::less<KT>,
    typename A = std::allocator<std::pair<const KT, T>>>
    using MultiMap = std::multimap<KT, T, C, A>;

  template<typename KT,typename HT,typename H =std::hash<HT>,
           typename C = std::equal_to<KT>, typename A = std::allocator<KT>>
  using UnorderdSet = std::unordered_set<KT,HT,C, A>;

  template<typename KT, typename T, typename HT, typename H = std::hash<HT>,
           typename C = std::equal_to<KT>, typename A = std::allocator<std::pair<const KT, T>>>
  using UnorderdMap = std::unordered_map<KT, T, HT, C, A>;

  template<typename T, typename A = Deque<T>>
  using Stack = std::stack<T, A>;

  template<typename T, typename A = Deque<T>>
  using Queue = std::queue<T, A>;

/*****************************************************************************/
/**
 * Punteros
 */
/*****************************************************************************/
  template <typename T>
  using SPtr = std::shared_ptr<T>;

  template <typename T>
  using WeakSptr = std::weak_ptr<T>;//para referencias a acceso a algun lado

  //template <typename T,
  //typename Alloc=GenALLOC,
  //typename Delete=Deleter>
  //using UPtr = std::unique_ptr<Alloc,Delete>;//Solo uno de aqui

}

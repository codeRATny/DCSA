#ifndef _GLOBAL_DEFFS_HPP_
#define _GLOBAL_DEFFS_HPP_

#ifndef NO_THREAD_SAFE_COMPILE

#define BOOST_THREAD_ENABLE_THREAD_SAFETY_ANALYSIS
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>

using Mutex = boost::mutex;
using LockGuard = boost::lock_guard<boost::mutex>;

#define GUARDED_BY(x) BOOST_THREAD_GUARDED_BY(x)
#define NO_THREAD_SAFE BOOST_THREAD_NO_THREAD_SAFETY_ANALYSIS

#else

#include <mutex>
#include <shared_mutex>

using Mutex = std::mutex;
using LockGuard = std::lock_guard<std::mutex>;

#define GUARDED_BY(x)
#define NO_THREAD_SAFE

#endif

#endif

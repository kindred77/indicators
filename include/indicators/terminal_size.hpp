
#ifndef INDICATORS_TERMINAL_SIZE
#define INDICATORS_TERMINAL_SIZE
#include <iostream>
#include <ostream>
#include <utility>


#if defined(_WIN32)
#if !defined(WIN32_LEAN_AND_MEAN)
#define INDICATORS_DEFINED_WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#if defined(INDICATORS_DEFINED_WIN32_LEAN_AND_MEAN)
#undef WIN32_LEAN_AND_MEAN
#undef INDICATORS_DEFINED_WIN32_LEAN_AND_MEAN
#endif

namespace indicators {

static inline std::pair<size_t, size_t>
terminal_size(std::ostream &stream = std::cout) {
  const auto handle = GetStdHandle(
      (&stream == &std::cerr || &stream == &std::clog) ? STD_ERROR_HANDLE
                                                       : STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi{};
  if (handle == INVALID_HANDLE_VALUE || handle == nullptr ||
      !GetConsoleScreenBufferInfo(handle, &csbi)) {
    return {24, 80};
  }
  const int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  const int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  return {static_cast<size_t>(rows), static_cast<size_t>(cols)};
}

static inline size_t terminal_width(std::ostream &stream = std::cout) {
  return terminal_size(stream).second;
}

} // namespace indicators

#else

#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h>    // for STDOUT_FILENO

namespace indicators {

static inline std::pair<size_t, size_t>
terminal_size(std::ostream &stream = std::cout) {
  struct winsize size{};
  const int fd = (&stream == &std::cerr || &stream == &std::clog)
                     ? STDERR_FILENO
                     : STDOUT_FILENO;
  if (ioctl(fd, TIOCGWINSZ, &size) != 0 || size.ws_col == 0) {
    return {24, 80};
  }
  return {static_cast<size_t>(size.ws_row), static_cast<size_t>(size.ws_col)};
}

static inline size_t terminal_width(std::ostream &stream = std::cout) {
  return terminal_size(stream).second;
}

} // namespace indicators

#endif

#endif

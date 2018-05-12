#ifndef _BLASR_PRETTY_EXCEPTION_HPP_
#define _BLASR_PRETTY_EXCEPTION_HPP_

#include <iostream>
#include <stdexcept>

#define BLASR_THROW(str)                                                                           \
    std::cerr << "Exception in file '" << __FILE__ << "', on line '" << __LINE__                   \
              << "' in function '" << __PRETTY_FUNCTION__ << "', with message: '" << str << "'\n"; \
    throw std::runtime_error { str }

#endif  // _BLASR_PRETTY_EXCEPTION_HPP_

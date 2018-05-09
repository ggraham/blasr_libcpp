#pragma once

#include <exception>
#include <iostream>

#define BLASR_THROW(str)                                                                           \
    std::cerr << "Exception in file '" << __FILE__ << "', on line '" << __LINE__                   \
              << "' in function '" << __PRETTY_FUNCTION__ << "', with message: '" << str << "'\n"; \
    throw std::runtime_error { str }

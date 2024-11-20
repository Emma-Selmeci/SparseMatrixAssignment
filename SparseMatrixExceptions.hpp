#ifndef HF05_SPARSEMATRIXEXCEPTIONS_HPP
#define HF05_SPARSEMATRIXEXCEPTIONS_HPP

#include <exception>
#include <string>

class IndexOutOfRangeException : public std::exception {
    const char* what() const noexcept override {
        return "Index out of range.";
    }
};

class SizeMismatchException : std::exception{
    const char * what() const noexcept override {
        return "The matrix sizes are not compatible.";
    }
};

class InvalidSizeException : std::exception{
    const char * what() const noexcept override {
        return "Matrix column or row not positive integer.";
    }
};

#endif //HF05_SPARSEMATRIXEXCEPTIONS_HPP

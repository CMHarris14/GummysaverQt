#include <exception>

class CustomException : public std::exception {
  public:
    enum ErrorType { DuplicateTitle };

    CustomException(ErrorType type_) : type(type_) {}

    const char *what() const noexcept override {
        switch (type) {
        case DuplicateTitle:
            return "Attempted to create a duplicate game. Titles must be "
                   "unique.";
        }
    }

  private:
    ErrorType type;
};

#include <exception>

class CustomException : public std::exception {
  public:
    enum ErrorType { DuplicateTitle, NoFilePath, DBError };

    CustomException(ErrorType type_) : type(type_) {}

    const char *what() const noexcept override {
        switch (type) {
        case DuplicateTitle:
            return "Attempted to create a duplicate game. Titles must be unique.";
        case NoFilePath:
            return "A valid file path must be given";
        case DBError:
            return "Database access failed. Ensure database exists and is accessible";
        }
    }

  private:
    ErrorType type;
};

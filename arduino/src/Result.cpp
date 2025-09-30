#include "Result.h"

Result resultAnd(Result lhs, Result rhs) {
    if (lhs == Result::Ok && rhs == Result::Ok) {
        return Result::Ok;
    } else {
        return Result::Error;
    }
}
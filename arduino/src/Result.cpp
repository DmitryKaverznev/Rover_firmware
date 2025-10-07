#include "utilis/Result.h"

Result resultAnd(const Result lhs, const Result rhs) {
    if (lhs == Result::Ok && rhs == Result::Ok) {
        return Result::Ok;
    } else {
        return Result::Error;
    }
}
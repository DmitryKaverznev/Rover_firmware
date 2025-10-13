#pragma once

enum Result {
    Ok,
    Error
};

inline Result resultAnd(const Result lhs, const Result rhs) {
    if (lhs == Ok && rhs == Ok) {
        return Ok;
    }
    return Error;
}

#pragma once

#include <string>

/**
 * @brief Status Code for SDK interface return
 */
enum class StatusCode {
    OK = 0,

    // system error section
    UnknownError = 1,
    NotSupported,
    NotConnected,

    // function error section
    InvalidAgument = 1000,
    RPCFailed,
    ServerFailed,
};

/**
 * @brief Status for SDK interface return
 */
class Status {
 public:
    Status(StatusCode code, const std::string& msg);
    Status();
    ~Status();

    Status(const Status& s);

    Status&
    operator=(const Status& s);

    Status(Status&& s);

    Status&
    operator=(Status&& s);

    static Status
    OK() {
        return Status();
    }

    bool
    ok() const {
        return state_ == nullptr || code() == StatusCode::OK;
    }

    StatusCode
    code() const {
        return (state_ == nullptr) ? StatusCode::OK : *(StatusCode*)(state_);
    }

    std::string
    message() const;

 private:
    inline void
    CopyFrom(const Status& s);

    inline void
    MoveFrom(Status& s);

 private:
    char* state_ = nullptr;
};  // Status

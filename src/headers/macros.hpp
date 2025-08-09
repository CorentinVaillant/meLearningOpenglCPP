#pragma once

#include<iostream>

#define expect_true(call, fail_msg, failed_return) {\
long int return_value = (bool) (call);\
    if(!return_value) {\
        std::cerr << fail_msg << std::endl;\
        exit(failed_return);\
    }}

#define expect_ptr(ptr, fail_msg, failed_return) {\
    if(!ptr) {\
        std::cerr << fail_msg << std::endl;\
        exit(failed_return);\
    }}

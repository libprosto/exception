#ifndef EXCEPTION_TEST_ALL_HPP
#define EXCEPTION_TEST_ALL_HPP

bool throw_std_catch_std_first();
bool throw_std_catch_prosto_first();
bool throw_prosto_catch_std_first();
bool throw_prosto_catch_prosto_first();

bool throw_prosto_exception();
bool throw_prosto_error();
bool throw_custom_prosto_exception();
bool throw_custom_prosto_error();
bool throw_nested_test();
bool throw_prosto_exception_with_stdstring();
bool throw_prosto_exception_without_code();
bool throw_prosto_exception_into_stringstream();

#endif // EXCEPTION_TEST_ALL_HPP

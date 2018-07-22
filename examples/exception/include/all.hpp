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

#endif // EXCEPTION_TEST_ALL_HPP

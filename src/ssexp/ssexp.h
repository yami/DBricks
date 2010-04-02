#ifndef SSEXP_H
#define SSEXP_H

#include <exception>

namespace ssexp {

class Object;
    
typedef Object* sexp_t;

extern sexp_t Nil;
extern sexp_t T;

sexp_t read_from_string(const char* string);

sexp_t make_symbol(const char* value);
sexp_t make_integer(int value);
sexp_t make_real(double value);
sexp_t make_string(const char* value);

sexp_t make_cons(sexp_t car, sexp_t cdr);
sexp_t make_list(sexp_t first, ...);

bool   consp(sexp_t object);
sexp_t car(sexp_t object);
sexp_t cdr(sexp_t object);
sexp_t first(sexp_t object);
sexp_t second(sexp_t object);
sexp_t third(sexp_t object);

sexp_t symbol_name(sexp_t object);

/*
 * -----------------------------
 * C Interface
 */
const char* ci_to_string(sexp_t object);
int         ci_to_integer(sexp_t object);
const char* ci_symbol_name(sexp_t object);
double ci_to_double(sexp_t object);

} // namespace ssexp

#endif // SSEXP_H

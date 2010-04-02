#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cstdarg>
#include <cassert>

#include "ssexp.h"

#define SSEXP_DECLARE_TYPE(CLASS, SUPER_CLASS, NAME)  \
    static const Object* type_name;                   \
    static const Object* super_name;                  \
    virtual const Object* type() const;               \
    virtual const Object* super() const


#define SSEXP_IMPLEMENT_TYPE(CLASS,SUPER_CLASS,NAME)                  \
    const Object* CLASS::type_name  = make_symbol(NAME);              \
    const Object* CLASS::super_name = SUPER_CLASS::type_name;         \
    const Object* CLASS::type() const { return type_name; }           \
    const Object* CLASS::super() const { return super_name; }


#define HARD_CAST(OBJ,TYPE) ({                                  \
    TYPE##Object* __##OBJ = dynamic_cast<TYPE##Object*>(OBJ);   \
    assert(__##OBJ);                                            \
    __##OBJ; })

namespace ssexp {

struct Object {
    static const Object* type_name;
    static const Object* super_name;
    
    virtual ~Object() {}

    bool isa(Object* type) const
    {
        return this->type() == type ||
            (this->super() && this->super()->isa(type));
    }
    
    virtual const Object* type() const
    {
        return type_name;
    }
    
    virtual const Object* super() const
    {
        return super_name;
    }

    virtual void princ(FILE* output) const = 0;
};


struct SymbolObject : public Object {
    SSEXP_DECLARE_TYPE(SymbolObject, Object, "symbol");

    SymbolObject(const char* v);
    ~SymbolObject();

    virtual void princ(FILE* output) const;
    
    char* value;
};


struct IntegerObject : public Object {
    SSEXP_DECLARE_TYPE(IntegerObject, Object, "integer");

    IntegerObject(int v)
        :value(v)
    {
    }

    virtual void princ(FILE* output) const;
    
    int value;
};

struct RealObject : public Object {
    SSEXP_DECLARE_TYPE(RealObject, Object, "real");

    RealObject(double v)
        :value(v)
    {
    }

    virtual void princ(FILE* output) const;
    
    double value;
};

struct StringObject : public Object {
    SSEXP_DECLARE_TYPE(StringObject, Object, "string");

    StringObject(const char* v);
    ~StringObject();

    virtual void princ(FILE* output) const;
    
    char* value;
};


struct ConsObject : public Object {
    SSEXP_DECLARE_TYPE(ConsObject, Object, "cons");

    ConsObject()
        :car(Nil), cdr(Nil)
    {
    }
    
    ConsObject(Object* head, Object* tail)
        :car(head), cdr(tail)
    {
    }

    virtual void princ(FILE* output) const;
    
    Object* car;
    Object* cdr;
};


const Object* Object::type_name  = 0;
const Object* Object::super_name = 0;

static SymbolObject Nil_Object("nil");
static SymbolObject T_Object("t");
static SymbolObject Quote_Object("quote");


SSEXP_IMPLEMENT_TYPE(SymbolObject,  Object, "symbol");
SSEXP_IMPLEMENT_TYPE(IntegerObject, Object, "integer");
SSEXP_IMPLEMENT_TYPE(RealObject,    Object, "real");
SSEXP_IMPLEMENT_TYPE(StringObject,  Object, "string");
SSEXP_IMPLEMENT_TYPE(ConsObject,    Object, "cons");

Object* Nil   = &Nil_Object;
Object* T     = &T_Object;
Object* Quote = &Quote_Object;


void string_to_upper(char* s)
{
    while (*s) {
        *s = toupper(*s);
        s++;
    }
}

SymbolObject::SymbolObject(const char* v)
{
    value = strdup(v);
    string_to_upper(value);
}

SymbolObject::~SymbolObject()
{
    free(value);
}

void SymbolObject::princ(FILE* output) const
{
    fprintf(output, "%s", value);
}

StringObject::StringObject(const char* v)
{
    value = strdup(v);
}

StringObject::~StringObject()
{
    free(value);
}

bool consp(Object* object)
{
    return dynamic_cast<ConsObject*>(object);
}

Object* car(Object* object)
{
    if (object == Nil)
        return Nil;
    
    ConsObject* cons = dynamic_cast<ConsObject*>(object);
    assert(cons);

    return cons->car;
}

Object* cdr(Object* object)
{
    if (object == Nil)
        return Nil;
    
    ConsObject* cons = dynamic_cast<ConsObject*>(object);
    assert(cons);

    return cons->cdr;
}

Object* first(Object* object)
{
    return car(object);
}

Object* second(Object* object)
{
    return car(cdr(object));
}

Object* third(Object* object)
{
    return car(cdr(cdr(object)));
}

Object* symbol_name(Object* object)
{
    SymbolObject* symbol = dynamic_cast<SymbolObject*>(object);
    assert(symbol);

    return make_string(symbol->value);
}

/*
 * -------------------------------------------------
 * C interface
 */
const char* ci_to_string(Object* object)
{
    return HARD_CAST(object, String)->value;
}

int ci_to_integer(Object* object)
{
    return HARD_CAST(object, Integer)->value;
}

const char* ci_symbol_name(Object* object)
{
    return HARD_CAST(object, Symbol)->value;
}

double ci_to_double(Object* object)
{
    return HARD_CAST(object, Real)->value;
}

void StringObject::princ(FILE* output) const
{
#define XPUTC(V) fputc('\\', output); fputc(V, output)    
    fputc('"', output);
    for (const char* p = value; *p; ++p) {
        switch (*p) {
            case '\a': XPUTC('a'); break;
            case '\b': XPUTC('b'); break;
            case '\f': XPUTC('f'); break;
            case '\n': XPUTC('n'); break;
            case '\r': XPUTC('r'); break;
            case '\t': XPUTC('t'); break;
            case '\v': XPUTC('v'); break;
            default: fputc(*p, output);
        }
    }
    fputc('"', output);
#undef XPUTC
}

void IntegerObject::princ(FILE* output) const
{
    fprintf(output, "%d", value);
}

void RealObject::princ(FILE* output) const
{
    fprintf(output, "%f", value);
}

void ConsObject::princ(FILE* output) const
{
    Object* next = cdr;
    fputc('(', output);
    car->princ(output);

    while (next != Nil) {
        if (consp(next)) {
            fputc(' ', output);
            ConsObject* cons = dynamic_cast<ConsObject*>(next);
            cons->car->princ(output);
            next = cons->cdr;
        } else {
            fputs(" . ", output);
            next->princ(output);
        }
    }
    
    fputc(')', output);
}

Object* make_symbol(const char* value)
{
    if (!strcasecmp(value, T_Object.value))
        return T;
    
    if (!strcasecmp(value, Nil_Object.value))
        return Nil;

    if (!strcasecmp(value, Quote_Object.value))
        return Quote;
    
    return new SymbolObject(value);
}

Object* make_integer(int value)
{
    return new IntegerObject(value);
}

Object* make_real(double value)
{
    return new RealObject(value);
}

Object* make_string(const char* value)
{
    return new StringObject(value);
}

Object* make_cons(Object* car, Object* cdr)
{
    return new ConsObject(car, cdr);
}

Object* make_list(Object* first, ...)
{
    const int Max_Items = 10;
    int       i         = 0;
    
    Object* items[Max_Items+1];
    
    va_list va;
    va_start(va, first);

    items[0] = first;
    for (i=1; i<Max_Items; ++i) {
        if ((items[i]= va_arg(va, Object*)) == Nil)
            break;
    }
    
    va_end(va);
    
    if (i == Max_Items)
        abort();

    Object* cdr = items[i];
    for(--i; i >= 0; --i) {
        cdr = make_cons(items[i], cdr);
    }

    return cdr;
}

void free_object(Object* object)
{
    if (object != Nil && object != T)
        delete object;
}

/*
 * pre: value is a null-terminated string, which might represent a
 * number (int or double). Whitespaces is not allowed, if value
 * represents a number. value should not be null.
 *
 * post: if value represents a number, either an IntergerObject or
 * RealObject will be returned. "NAN" and "INF" are not considered as
 * numbers.
 */
Object* make_number(const char* value)
{
    size_t length = strlen(value);

    if (value[0] == 0)
        return 0;
    
    /* we do not support NaN and Inf, which is supported by strtod, strtol */
    if (length >= 3) {
        if (!strncasecmp(value, "inf", 3) ||
            !strncasecmp(value, "nan", 3))
            return 0;
    }
    
    char* endp;

    int int_value = strtol(value, &endp, 0);
    if (endp == value + length)
        return make_integer(int_value);

    double real_value = strtod(value, &endp);
    if (endp == value + length)
        return make_real(real_value);

    return 0;
}

/*
 *----------------------------------------------------------------
 */

enum TokenType {
    TT_None,
    TT_OpenParen,
    TT_CloseParen,
    TT_Quote,
    TT_Dot,
    TT_Symbol,
    TT_Number,
    TT_String,
    TT_EOF,
    TT_Error,
};

class ParseException: public std::exception {
public:
    virtual const char* what() const throw ()
    {
        return "Parse Error\n";
    }
};


Object* parse_object(FILE* input, TokenType token_type, Object* token_value);

static char skip_comment(FILE* input)
{
    int c;
    do {
        if ((c = fgetc(input)) == EOF)
            return 0;
    } while (c != '\n');

    return c;
}


static char next_char(FILE* input)
{
    int  c;

    do {
        if ((c = fgetc(input)) == EOF)
            return 0;

        if (c == ';')
            c = skip_comment(input);        
    } while (c && isspace(c));

    return c;
}

static char peek_next(FILE* input)
{
    int c;
    if ((c = fgetc(input)) == EOF)
        return 0;
    ungetc(c, input);

    return c;
}

static bool is_delimiter(int c)
{
    return c == ';' || c == '(' || c == ')' || c == 0 || isspace(c) || c == EOF;
}

static void assign_object(Object* object, Object** objectp)
{
    if (objectp) {
        *objectp = object;
    } else {
        free_object(object);
    }
}

const int Max_Symbol_Length = 255;

static TokenType next_number_or_symbol(FILE* input, Object** token_value)
{
    int c;
    int length = 0;

    char    token[Max_Symbol_Length+1];
    Object* object = 0;
    
    while (1) {
        c = fgetc(input);
        
        if (!is_delimiter(c) && length < Max_Symbol_Length) {
            token[length++] = c;
        } else {
            ungetc(c, input);
            break;
        }
    }

    token[length] = 0;

    if ((object = make_number(token))) {
        assign_object(object, token_value);
        return TT_Number;
    } else {
        assign_object(make_symbol(token), token_value);
        return TT_Symbol;
    }
}

static TokenType next_string(FILE* input, Object** token_value)
{
    const int Chunk_Size = 512;
    int       chunk      = 0;
    int       nchar      = 0;
    int  c;
    char* token = (char *) malloc(++chunk * Chunk_Size);

    /* skip " */
    fgetc(input);

    /* only 1 char will be saved into token[] in one iteration. */
    while (1) {
        if ((c = fgetc(input)) == EOF)
            goto fail;
        
        if (++nchar >= chunk*Chunk_Size)
            token = (char*) realloc(token, ++chunk*Chunk_Size);

        char* token_char = &token[nchar-1];

        if (c == '"') {
            *token_char = 0;
            goto done;
        }

        if (c == '\\') {
            if ((c = fgetc(input)) == EOF)
                goto fail;
        
            switch (c) {
                case 'a': *token_char = '\a'; break;
                case 'b': *token_char = '\b'; break;
                case 'f': *token_char = '\f'; break;
                case 'n': *token_char = '\n'; break;
                case 'r': *token_char = '\r'; break;
                case 't': *token_char = '\t'; break;
                case 'v': *token_char = '\v'; break;
                case '\n': break;
                default:
                    *token_char = c;
            }
        } else {
            *token_char = c;
        }
    }

done:
    assign_object(make_string(token), token_value);
    free(token);
    return TT_String;
    
fail:
    free(token);
    return TT_Error;
}

static TokenType next_token(FILE* input, Object** token_value)
{
    int c;

    while ((c = next_char(input))) {
        if (c == '(') {
            return TT_OpenParen;
        }
            
        if (c == ')') {
            return TT_CloseParen;
        }

        if (c == '\'') {
            return TT_Quote;
        }

        if (c == '"') {
            ungetc(c, input);
            return next_string(input, token_value);
        }

        if (c == '.' && is_delimiter(peek_next(input))) {
            return TT_Dot;
        }

        ungetc(c, input);
        return next_number_or_symbol(input, token_value);
    }

    return TT_EOF;
}

Object* parse_form(FILE* input)
{
    TokenType   token_type;
    Object*     token_value;

    ConsObject  dummy_prev;
    ConsObject* first = 0;
    ConsObject* prev  = &dummy_prev;
    
    while (1) {
        token_type = next_token(input, &token_value);

        if (token_type == TT_EOF) {
            throw ParseException();
        }

        if (token_type == TT_CloseParen) {
            break;
        }

        if (token_type == TT_Dot) {
            if (!prev)
                throw ParseException();
            prev->cdr = parse_object(input, TT_None, 0);
            if (next_token(input, 0) != TT_CloseParen)
                throw ParseException();
            break;
        }
        
        ConsObject* object = new ConsObject();

        first = first ? : object;        

        object->car = parse_object(input, token_type, token_value);
        prev->cdr   = object;
        prev        = object;
    }

    return first ? first : Nil;
}

Object* parse_quote(FILE* input)
{
    Object* object = parse_object(input, TT_None, 0);

    return make_list(Quote, object, Nil);
}

Object* parse_object(FILE* input, TokenType token_type, Object* token_value)
{
    if (token_type == TT_None)
        token_type = next_token(input, &token_value);

    switch (token_type) {
        case TT_OpenParen:
            token_value = parse_form(input);
            break;
        case TT_Quote:
            token_value = parse_quote(input);
            break;
        case TT_Symbol:
        case TT_Number:
        case TT_String:
        case TT_EOF:
            break;
        default:
            /* error */
            break;
    }

    return token_value;
}

Object* read_from_string(const char* string)
{
    FILE* input = fmemopen((void *)string, strlen(string), "r");

    if (!input)
        return 0;

    Object* object = parse_object(input, TT_None, 0);
    
    fclose(input);

    return object;
}

} // namespace ssexp

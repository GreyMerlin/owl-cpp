/** @file "/owlcpp/lib/io/parse.hpp"
part of owlcpp project.
@n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2010
*******************************************************************************/
#ifndef PARSE_HPP_
#define PARSE_HPP_
#include <iosfwd>
#include <string>
//#include "raptor_triple.hpp"
#include "owlcpp/exception.hpp"

typedef std::basic_string<unsigned char> ustring_t;

typedef struct raptor_parser_s raptor_parser;
typedef struct raptor_world_s raptor_world;

namespace owlcpp{

/**
*******************************************************************************/
class Rdf_parser {
   typedef void (*handle_statement_fun_t)(void *, const void*);
   typedef bool (*stop_parsing_fun_t)(const void *);
public:
   //Raptor prefers unsigned chars
   typedef std::basic_string<unsigned char> string_t;

   struct Err : public base_exception {};
   static Rdf_parser rdfxml(std::string const& base_uri);

   ~Rdf_parser();

   template<class Sink> void operator()(
         std::istream& stream,
         Sink& sink,
         const unsigned base
   ) {
      parse(stream, &sink, handle_statement<Sink>, stop_parsing<Sink>, base);
   }

private:
   raptor_world* world_;
   raptor_parser* parser_;
   const string_t base_uri_;

   Rdf_parser(const char*, const string_t&);

   void parse(
         std::istream&, void*,
         handle_statement_fun_t,
         stop_parsing_fun_t,
         const unsigned base
   );

   template<class Sink> static void handle_statement(void* sink, const void* rs) {
      reinterpret_cast<Sink*>(sink)->insert(rs);
   }

   template<class Sink> static bool stop_parsing(const void* sink) {
      return reinterpret_cast<const Sink*>(sink)->stop_parsing();
   }
};


}//namespace owlcpp

#endif /* PARSE_HPP_ */

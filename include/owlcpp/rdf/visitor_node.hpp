/** @file "/owlcpp/include/owlcpp/rdf/visitor_node.hpp"
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef VISITOR_NODE_HPP_
#define VISITOR_NODE_HPP_
#include <string>
#include "boost/cstdint.hpp"

namespace owlcpp{ namespace detail{
template<class> class Node_literal;
}//namespace detail

class Node_iri;
class Node_blank;
typedef detail::Node_literal<bool> Node_bool;
typedef detail::Node_literal<boost::intmax_t> Node_int;
typedef detail::Node_literal<boost::uintmax_t> Node_unsigned;
typedef detail::Node_literal<double> Node_double;
typedef detail::Node_literal<std::string> Node_string;

/**@brief 
*******************************************************************************/
class Visitor_node {
public:
   void visit(Node_iri const& node) {visit_impl(node);}
   void visit(Node_blank const& node) {visit_impl(node);}
   void visit(Node_bool const& node) {visit_impl(node);}
   void visit(Node_int const& node) {visit_impl(node);}
   void visit(Node_unsigned const& node) {visit_impl(node);}
   void visit(Node_double const& node) {visit_impl(node);}
   void visit(Node_string const& node) {visit_impl(node);}

   virtual ~Visitor_node() {}
private:
   virtual void visit_impl(Node_iri const&) = 0;
   virtual void visit_impl(Node_blank const&) = 0;
   virtual void visit_impl(Node_bool const&) = 0;
   virtual void visit_impl(Node_int const&) = 0;
   virtual void visit_impl(Node_unsigned const&) = 0;
   virtual void visit_impl(Node_double const&) = 0;
   virtual void visit_impl(Node_string const&) = 0;
};

#define OWLCPP_VISITABLE \
   void accept_impl(Visitor_node& visitor) const {visitor.visit(*this);} \
/* */


}//namespace owlcpp
#endif /* VISITOR_NODE_HPP_ */

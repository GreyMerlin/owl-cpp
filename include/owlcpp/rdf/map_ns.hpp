/** @file "/owlcpp/include/owlcpp/rdf/map_ns.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef MAP_NS_HPP_
#define MAP_NS_HPP_
#include <string>
#include <map>
#include <vector>
#include <functional>
#include "boost/assert.hpp"
#include "boost/foreach.hpp"

#include "owlcpp/ns_id.hpp"
#include "owlcpp/rdf/exception.hpp"
#include "owlcpp/detail/iterator_member_pair.hpp"
#include "owlcpp/terms/detail/max_standard_id.hpp"

namespace owlcpp{

/**@brief Map for namespace IRIs
*******************************************************************************/
class Map_ns {
   typedef std::map<std::string,Ns_id> map_t;
   typedef std::pair<std::string,Ns_id> pair_t;
   typedef map_t::iterator map_iter_t;
   typedef std::pair<map_iter_t,map_iter_t> id_value_t;
   typedef std::vector<id_value_t> vec_t;
   typedef map_t::const_iterator map_citer_t;

public:
   typedef Ns_id id_type;
   typedef Iterator_member_pair<map_citer_t, Ns_id const, 2> iterator;

   typedef iterator const_iterator;

   struct Err : public Rdf_err {};

   Map_ns(const Ns_id min_id = detail::min_ns_id())
   : min_id_(min_id)
   {}

   Map_ns(Map_ns const& mns) : min_id_(mns.min_id_) {copy(mns);}

   Map_ns& operator=(Map_ns const& mns) {
      clear();
      min_id_ = mns.min_id_;
      copy(mns);
      return *this;
   }

   std::size_t size() const {
      BOOST_ASSERT(id_.size() >= iri_.size());
      return iri_.size();
   }

   const_iterator begin() const {return iri_.begin();}
   const_iterator end() const {return iri_.end();}
   bool empty() const {return iri_.empty();}

   std::string operator[](const Ns_id id) const {
      return get(id).first->first;
   }

   std::string at(const Ns_id id) const {
      if( std::string const*const str = find(id) ) return *str;
      BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("invalid namespace ID")
               << Err::int1_t(id())
      );
   }

   std::string const* find(const Ns_id id) const {
      if( id < min_id_ ) return 0;
      const std::size_t n = id() - min_id_();
      if( n < id_.size() ) {
         id_value_t const & v = id_[n];
         if( v.first == iri_.end() ) return 0;
         return &v.first->first;
      }
      return 0;
   }

   /**
    @param iri namespace IRI string
    @return pointer to namespace IRI ID or NULL if iri is unknown
   */
   Ns_id const* find_iri(std::string const& iri) const {
      const map_citer_t i = iri_.find(iri);
      if( i == iri_.end() ) return 0;
      return &i->second;
   }

   std::string prefix(const Ns_id id) const {
      id_value_t const& v = get(id);
      if( v.second == pref_.end() ) return "";
      return v.second->first;
   }

   Ns_id const* find_prefix(std::string const& pref) const {
      const map_citer_t i = pref_.find(pref);
      if( i == pref_.end() ) return 0;
      return &i->second;
   }

   Ns_id insert(std::string const& iri) {
      const map_iter_t i = iri_.find(iri);
      if( i != iri_.end() ) return i->second;
      return insert(next_id(), iri);
   }

   Ns_id insert(const Ns_id id, std::string const& iri) {
      resize(id);
      id_value_t& v = get(id);
      if( v.first != iri_.end() ) {
         if( v.first->first != iri ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("ID already in use")
                  << Err::str1_t(iri)
                  << Err::str2_t(v.first->first)
                  << Err::int1_t(id())
         );
         BOOST_ASSERT(iri_.find(iri) == v.first);
         return id;
      }
      BOOST_ASSERT(iri_.find(iri) == iri_.end());
      v.first = iri_.insert(std::make_pair(iri, id)).first;
      v.second = pref_.end();
      return id;
   }

   /**@brief set or clear a prefix for namespace IRI
    @param id namespace IRI ID
    @param pref prefix string for namespace IRI;
    if \b pref is empty, the existing prefix is cleared from the IRI
   */
   void set_prefix(const Ns_id id, std::string const& pref = "") {
      id_value_t& v = get(id);

      if( pref.empty() ) {
         if( v.second != pref_.end() ) {
            pref_.erase(v.second);
            v.second = pref_.end();
         }
         return;
      }

      if( v.second != pref_.end() ) {
         if( v.second->first != pref ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("different IRI prefix has been set")
                  << Err::str1_t(pref)
                  << Err::str2_t(v.second->first)
                  << Err::str3_t(at(id))
         );
         BOOST_ASSERT(pref_.find(pref) == v.second);
         return;
      }

      if( pref_.find(pref) != pref_.end() ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("prefix used for different IRI")
               << Err::str1_t(pref)
               << Err::str2_t(v.second->first)
               << Err::str3_t(at(id))
      );

      v.second = pref_.insert(std::make_pair(pref, id)).first;
   }

   void remove(const Ns_id id) {
      if( ! find(id) ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("removing non-existing IRI ID")
               << Err::int1_t(id())
      );
      id_value_t& v = get(id);
      iri_.erase(v.first);
      v.first = iri_.end();
      if( v.second != pref_.end() ) pref_.erase(v.second);
      erased_.push_back(id);
   }

   void clear() {
      iri_.clear();
      pref_.clear();
      id_.clear();
      erased_.clear();
   }

private:
   map_t iri_;
   map_t pref_;
   vec_t id_;
   std::vector<Ns_id> erased_;
   Ns_id min_id_;

   Ns_id next_id() {
      if( erased_.empty() ) return Ns_id(id_.size() + min_id_());
      const Ns_id id = erased_.back();
      erased_.pop_back();
      return id;
   }

   id_value_t& get(const Ns_id id) {
      BOOST_ASSERT( id >= min_id_ );
      const std::size_t n = id() - min_id_();
      BOOST_ASSERT( n < id_.size() );
      return id_[n];
   }

   id_value_t const& get(const Ns_id id) const {
      BOOST_ASSERT( id >= min_id_ );
      const std::size_t n = id() - min_id_();
      BOOST_ASSERT( n < id_.size() );
      return id_[n];
   }

   void resize(const Ns_id id) {
      BOOST_ASSERT(id >= min_id_);
      id_type::value_type n = id() - min_id_();
      if( n >= id_.size() ) {
         id_.resize(n + 1, std::make_pair(iri_.end(), pref_.end()));
      }
   }

   void copy(Map_ns const& mns) {
      BOOST_ASSERT(min_id_ == mns.min_id_ && "base ID should be same");
      id_.resize(mns.id_.size());
      erased_ = mns.erased_;
      //insert elements in the same order as in source map
      BOOST_FOREACH(pair_t const& p, mns.iri_) {
         const Ns_id id = p.second;
         id_value_t& v0 = get(id);
         v0.first = iri_.insert(p).first;
         id_value_t const& v1 = mns.get(id);
         if( v1.second != mns.pref_.end() ) {
            v0.second = pref_.insert(*v1.second).first;
         } else {
            v0.second = pref_.end();
         }
      }
   }
};

}//namespace owlcpp
#endif /* MAP_NS_HPP_ */

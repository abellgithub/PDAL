/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2006-2013
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_INTRUSIVE_RBTREE_HPP
#define BOOST_INTRUSIVE_RBTREE_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <cstddef>
#include <functional>
#include <utility>

#include <boost/intrusive/detail/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/intrusive/intrusive_fwd.hpp>
#include <boost/intrusive/set_hook.hpp>
#include <boost/intrusive/detail/rbtree_node.hpp>
#include <boost/intrusive/bstree.hpp>
#include <boost/intrusive/detail/tree_node.hpp>
#include <boost/intrusive/detail/ebo_functor_holder.hpp>
#include <boost/intrusive/detail/mpl.hpp>
#include <boost/intrusive/pointer_traits.hpp>
#include <boost/intrusive/detail/function_detector.hpp>
#include <boost/intrusive/detail/utilities.hpp>
#include <boost/intrusive/options.hpp>
#include <boost/intrusive/rbtree_algorithms.hpp>
#include <boost/intrusive/link_mode.hpp>
#include <boost/move/move.hpp>

namespace pdalboost {} namespace boost = pdalboost; namespace pdalboost {
namespace intrusive {

/// @cond

struct rbtree_defaults
{
   typedef detail::default_rbtree_hook proto_value_traits;
   static const bool constant_time_size = true;
   typedef std::size_t size_type;
   typedef void compare;
};

/// @endcond

//! The class template rbtree is an intrusive red-black tree container, that
//! is used to construct intrusive set and multiset containers. The no-throw
//! guarantee holds only, if the value_compare object
//! doesn't throw.
//!
//! The template parameter \c T is the type to be managed by the container.
//! The user can specify additional options and if no options are provided
//! default options are used.
//!
//! The container supports the following options:
//! \c base_hook<>/member_hook<>/value_traits<>,
//! \c constant_time_size<>, \c size_type<> and
//! \c compare<>.
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)
template<class T, class ...Options>
#else
template<class ValueTraits, class VoidOrKeyComp, class SizeType, bool ConstantTimeSize>
#endif
class rbtree_impl
   /// @cond
   :  public bstree_impl<ValueTraits, VoidOrKeyComp, SizeType, ConstantTimeSize, RbTreeAlgorithms>
   /// @endcond
{
   public:
   typedef ValueTraits                                               value_traits;
   /// @cond
   typedef bstree_impl< ValueTraits, VoidOrKeyComp, SizeType
                      , ConstantTimeSize, RbTreeAlgorithms>          tree_type;
   typedef tree_type                                                 implementation_defined;
   /// @endcond

   typedef typename implementation_defined::pointer                  pointer;
   typedef typename implementation_defined::const_pointer            const_pointer;
   typedef typename implementation_defined::value_type               value_type;
   typedef typename implementation_defined::key_type                 key_type;
   typedef typename implementation_defined::reference                reference;
   typedef typename implementation_defined::const_reference          const_reference;
   typedef typename implementation_defined::difference_type          difference_type;
   typedef typename implementation_defined::size_type                size_type;
   typedef typename implementation_defined::value_compare            value_compare;
   typedef typename implementation_defined::key_compare              key_compare;
   typedef typename implementation_defined::iterator                 iterator;
   typedef typename implementation_defined::const_iterator           const_iterator;
   typedef typename implementation_defined::reverse_iterator         reverse_iterator;
   typedef typename implementation_defined::const_reverse_iterator   const_reverse_iterator;
   typedef typename implementation_defined::node_traits              node_traits;
   typedef typename implementation_defined::node                     node;
   typedef typename implementation_defined::node_ptr                 node_ptr;
   typedef typename implementation_defined::const_node_ptr           const_node_ptr;
   typedef typename implementation_defined::node_algorithms          node_algorithms;

   static const bool constant_time_size = implementation_defined::constant_time_size;
   /// @cond
   private:

   //noncopyable
   BOOST_MOVABLE_BUT_NOT_COPYABLE(rbtree_impl)

   /// @endcond

   public:

   typedef typename implementation_defined::insert_commit_data insert_commit_data;

   //! @copydoc ::pdalboost::intrusive::bstree::bstree(const value_compare &,const value_traits &)
   explicit rbtree_impl( const value_compare &cmp = value_compare()
                       , const value_traits &v_traits = value_traits())
      :  tree_type(cmp, v_traits)
   {}

   //! @copydoc ::pdalboost::intrusive::bstree::bstree(bool,Iterator,Iterator,const value_compare &,const value_traits &)
   template<class Iterator>
   rbtree_impl( bool unique, Iterator b, Iterator e
              , const value_compare &cmp     = value_compare()
              , const value_traits &v_traits = value_traits())
      : tree_type(unique, b, e, cmp, v_traits)
   {}

   //! @copydoc ::pdalboost::intrusive::bstree::bstree(bstree &&)
   rbtree_impl(BOOST_RV_REF(rbtree_impl) x)
      :  tree_type(::pdalboost::move(static_cast<tree_type&>(x)))
   {}

   //! @copydoc ::pdalboost::intrusive::bstree::operator=(bstree &&)
   rbtree_impl& operator=(BOOST_RV_REF(rbtree_impl) x)
   {  return static_cast<rbtree_impl&>(tree_type::operator=(::pdalboost::move(static_cast<tree_type&>(x)))); }

   #ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
   //! @copydoc ::pdalboost::intrusive::bstree::~bstree()
   ~rbtree_impl();

   //! @copydoc ::pdalboost::intrusive::bstree::begin()
   iterator begin();

   //! @copydoc ::pdalboost::intrusive::bstree::begin()const
   const_iterator begin() const;

   //! @copydoc ::pdalboost::intrusive::bstree::cbegin()const
   const_iterator cbegin() const;

   //! @copydoc ::pdalboost::intrusive::bstree::end()
   iterator end();

   //! @copydoc ::pdalboost::intrusive::bstree::end()const
   const_iterator end() const;

   //! @copydoc ::pdalboost::intrusive::bstree::cend()const
   const_iterator cend() const;

   //! @copydoc ::pdalboost::intrusive::bstree::rbegin()
   reverse_iterator rbegin();

   //! @copydoc ::pdalboost::intrusive::bstree::rbegin()const
   const_reverse_iterator rbegin() const;

   //! @copydoc ::pdalboost::intrusive::bstree::crbegin()const
   const_reverse_iterator crbegin() const;

   //! @copydoc ::pdalboost::intrusive::bstree::rend()
   reverse_iterator rend();

   //! @copydoc ::pdalboost::intrusive::bstree::rend()const
   const_reverse_iterator rend() const;

   //! @copydoc ::pdalboost::intrusive::bstree::crend()const
   const_reverse_iterator crend() const;

   //! @copydoc ::pdalboost::intrusive::bstree::container_from_end_iterator(iterator)
   static rbtree_impl &container_from_end_iterator(iterator end_iterator);

   //! @copydoc ::pdalboost::intrusive::bstree::container_from_end_iterator(const_iterator)
   static const rbtree_impl &container_from_end_iterator(const_iterator end_iterator);

   //! @copydoc ::pdalboost::intrusive::bstree::container_from_iterator(iterator)
   static rbtree_impl &container_from_iterator(iterator it);

   //! @copydoc ::pdalboost::intrusive::bstree::container_from_iterator(const_iterator)
   static const rbtree_impl &container_from_iterator(const_iterator it);

   //! @copydoc ::pdalboost::intrusive::bstree::key_comp()const
   key_compare key_comp() const;

   //! @copydoc ::pdalboost::intrusive::bstree::value_comp()const
   value_compare value_comp() const;

   //! @copydoc ::pdalboost::intrusive::bstree::empty()const
   bool empty() const;

   //! @copydoc ::pdalboost::intrusive::bstree::size()const
   size_type size() const;

   //! @copydoc ::pdalboost::intrusive::bstree::swap
   void swap(rbtree_impl& other);

   //! @copydoc ::pdalboost::intrusive::bstree::clone_from
   template <class Cloner, class Disposer>
   void clone_from(const rbtree_impl &src, Cloner cloner, Disposer disposer);

   //! @copydoc ::pdalboost::intrusive::bstree::insert_equal(reference)
   iterator insert_equal(reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::insert_equal(const_iterator,reference)
   iterator insert_equal(const_iterator hint, reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::insert_equal(Iterator,Iterator)
   template<class Iterator>
   void insert_equal(Iterator b, Iterator e);

   //! @copydoc ::pdalboost::intrusive::bstree::insert_unique(reference)
   std::pair<iterator, bool> insert_unique(reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::insert_unique(const_iterator,reference)
   iterator insert_unique(const_iterator hint, reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::insert_unique_check(const KeyType&,KeyValueCompare,insert_commit_data&)
   template<class KeyType, class KeyValueCompare>
   std::pair<iterator, bool> insert_unique_check
      (const KeyType &key, KeyValueCompare key_value_comp, insert_commit_data &commit_data);

   //! @copydoc ::pdalboost::intrusive::bstree::insert_unique_check(const_iterator,const KeyType&,KeyValueCompare,insert_commit_data&)
   template<class KeyType, class KeyValueCompare>
   std::pair<iterator, bool> insert_unique_check
      (const_iterator hint, const KeyType &key
      ,KeyValueCompare key_value_comp, insert_commit_data &commit_data);

   //! @copydoc ::pdalboost::intrusive::bstree::insert_unique_commit
   iterator insert_unique_commit(reference value, const insert_commit_data &commit_data);

   //! @copydoc ::pdalboost::intrusive::bstree::insert_unique(Iterator,Iterator)
   template<class Iterator>
   void insert_unique(Iterator b, Iterator e);

   //! @copydoc ::pdalboost::intrusive::bstree::insert_before
   iterator insert_before(const_iterator pos, reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::push_back
   void push_back(reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::push_front
   void push_front(reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::erase(const_iterator)
   iterator erase(const_iterator i);

   //! @copydoc ::pdalboost::intrusive::bstree::erase(const_iterator,const_iterator)
   iterator erase(const_iterator b, const_iterator e);

   //! @copydoc ::pdalboost::intrusive::bstree::erase(const_reference)
   size_type erase(const_reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::erase(const KeyType&,KeyValueCompare)
   template<class KeyType, class KeyValueCompare>
   size_type erase(const KeyType& key, KeyValueCompare comp);

   //! @copydoc ::pdalboost::intrusive::bstree::erase_and_dispose(const_iterator,Disposer)
   template<class Disposer>
   iterator erase_and_dispose(const_iterator i, Disposer disposer);

   //! @copydoc ::pdalboost::intrusive::bstree::erase_and_dispose(const_iterator,const_iterator,Disposer)
   template<class Disposer>
   iterator erase_and_dispose(const_iterator b, const_iterator e, Disposer disposer);

   //! @copydoc ::pdalboost::intrusive::bstree::erase_and_dispose(const_reference, Disposer)
   template<class Disposer>
   size_type erase_and_dispose(const_reference value, Disposer disposer);

   //! @copydoc ::pdalboost::intrusive::bstree::erase_and_dispose(const KeyType&,KeyValueCompare,Disposer)
   template<class KeyType, class KeyValueCompare, class Disposer>
   size_type erase_and_dispose(const KeyType& key, KeyValueCompare comp, Disposer disposer);

   //! @copydoc ::pdalboost::intrusive::bstree::clear
   void clear();

   //! @copydoc ::pdalboost::intrusive::bstree::clear_and_dispose
   template<class Disposer>
   void clear_and_dispose(Disposer disposer);

   //! @copydoc ::pdalboost::intrusive::bstree::count(const_reference)const
   size_type count(const_reference value) const;

   //! @copydoc ::pdalboost::intrusive::bstree::count(const KeyType&,KeyValueCompare)const
   template<class KeyType, class KeyValueCompare>
   size_type count(const KeyType& key, KeyValueCompare comp) const;
   
   //! @copydoc ::pdalboost::intrusive::bstree::lower_bound(const_reference)
   iterator lower_bound(const_reference value);
   
   //! @copydoc ::pdalboost::intrusive::bstree::lower_bound(const KeyType&,KeyValueCompare)
   template<class KeyType, class KeyValueCompare>
   iterator lower_bound(const KeyType& key, KeyValueCompare comp);

   //! @copydoc ::pdalboost::intrusive::bstree::lower_bound(const_reference)const
   const_iterator lower_bound(const_reference value) const;

   //! @copydoc ::pdalboost::intrusive::bstree::lower_bound(const KeyType&,KeyValueCompare)const
   template<class KeyType, class KeyValueCompare>
   const_iterator lower_bound(const KeyType& key, KeyValueCompare comp) const;

   //! @copydoc ::pdalboost::intrusive::bstree::upper_bound(const_reference)
   iterator upper_bound(const_reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::upper_bound(const KeyType&,KeyValueCompare)
   template<class KeyType, class KeyValueCompare>
   iterator upper_bound(const KeyType& key, KeyValueCompare comp);

   //! @copydoc ::pdalboost::intrusive::bstree::upper_bound(const_reference)const
   const_iterator upper_bound(const_reference value) const;

   //! @copydoc ::pdalboost::intrusive::bstree::upper_bound(const KeyType&,KeyValueCompare)const
   template<class KeyType, class KeyValueCompare>
   const_iterator upper_bound(const KeyType& key, KeyValueCompare comp) const;

   //! @copydoc ::pdalboost::intrusive::bstree::find(const_reference)
   iterator find(const_reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::find(const KeyType&,KeyValueCompare)
   template<class KeyType, class KeyValueCompare>
   iterator find(const KeyType& key, KeyValueCompare comp);

   //! @copydoc ::pdalboost::intrusive::bstree::find(const_reference)const
   const_iterator find(const_reference value) const;

   //! @copydoc ::pdalboost::intrusive::bstree::find(const KeyType&,KeyValueCompare)const
   template<class KeyType, class KeyValueCompare>
   const_iterator find(const KeyType& key, KeyValueCompare comp) const;

   //! @copydoc ::pdalboost::intrusive::bstree::equal_range(const_reference)
   std::pair<iterator,iterator> equal_range(const_reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::equal_range(const KeyType&,KeyValueCompare)
   template<class KeyType, class KeyValueCompare>
   std::pair<iterator,iterator> equal_range(const KeyType& key, KeyValueCompare comp);

   //! @copydoc ::pdalboost::intrusive::bstree::equal_range(const_reference)const
   std::pair<const_iterator, const_iterator>
      equal_range(const_reference value) const;

   //! @copydoc ::pdalboost::intrusive::bstree::equal_range(const KeyType&,KeyValueCompare)const
   template<class KeyType, class KeyValueCompare>
   std::pair<const_iterator, const_iterator>
      equal_range(const KeyType& key, KeyValueCompare comp) const;

   //! @copydoc ::pdalboost::intrusive::bstree::bounded_range(const_reference,const_reference,bool,bool)
   std::pair<iterator,iterator> bounded_range
      (const_reference lower_value, const_reference upper_value, bool left_closed, bool right_closed);

   //! @copydoc ::pdalboost::intrusive::bstree::bounded_range(const KeyType&,const KeyType&,KeyValueCompare,bool,bool)
   template<class KeyType, class KeyValueCompare>
   std::pair<iterator,iterator> bounded_range
      (const KeyType& lower_key, const KeyType& upper_key, KeyValueCompare comp, bool left_closed, bool right_closed);

   //! @copydoc ::pdalboost::intrusive::bstree::bounded_range(const_reference,const_reference,bool,bool)const
   std::pair<const_iterator, const_iterator>
      bounded_range(const_reference lower_value, const_reference upper_value, bool left_closed, bool right_closed) const;

   //! @copydoc ::pdalboost::intrusive::bstree::bounded_range(const KeyType&,const KeyType&,KeyValueCompare,bool,bool)const
   template<class KeyType, class KeyValueCompare>
   std::pair<const_iterator, const_iterator> bounded_range
         (const KeyType& lower_key, const KeyType& upper_key, KeyValueCompare comp, bool left_closed, bool right_closed) const;

   //! @copydoc ::pdalboost::intrusive::bstree::s_iterator_to(reference)
   static iterator s_iterator_to(reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::s_iterator_to(const_reference)
   static const_iterator s_iterator_to(const_reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::iterator_to(reference)
   iterator iterator_to(reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::iterator_to(const_reference)const
   const_iterator iterator_to(const_reference value) const;

   //! @copydoc ::pdalboost::intrusive::bstree::init_node(reference)
   static void init_node(reference value);

   //! @copydoc ::pdalboost::intrusive::bstree::unlink_leftmost_without_rebalance
   pointer unlink_leftmost_without_rebalance();

   //! @copydoc ::pdalboost::intrusive::bstree::replace_node
   void replace_node(iterator replace_this, reference with_this);

   //! @copydoc ::pdalboost::intrusive::bstree::remove_node
   void remove_node(reference value);
   #endif   //#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
};

#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)

template<class T, class ...Options>
bool operator< (const rbtree_impl<T, Options...> &x, const rbtree_impl<T, Options...> &y);

template<class T, class ...Options>
bool operator==(const rbtree_impl<T, Options...> &x, const rbtree_impl<T, Options...> &y);

template<class T, class ...Options>
bool operator!= (const rbtree_impl<T, Options...> &x, const rbtree_impl<T, Options...> &y);

template<class T, class ...Options>
bool operator>(const rbtree_impl<T, Options...> &x, const rbtree_impl<T, Options...> &y);

template<class T, class ...Options>
bool operator<=(const rbtree_impl<T, Options...> &x, const rbtree_impl<T, Options...> &y);

template<class T, class ...Options>
bool operator>=(const rbtree_impl<T, Options...> &x, const rbtree_impl<T, Options...> &y);

template<class T, class ...Options>
void swap(rbtree_impl<T, Options...> &x, rbtree_impl<T, Options...> &y);

#endif   //#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED)

//! Helper metafunction to define a \c rbtree that yields to the same type when the
//! same options (either explicitly or implicitly) are used.
#if defined(BOOST_INTRUSIVE_DOXYGEN_INVOKED) || defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template<class T, class ...Options>
#else
template<class T, class O1 = void, class O2 = void
                , class O3 = void, class O4 = void>
#endif
struct make_rbtree
{
   /// @cond
   typedef typename pack_options
      < rbtree_defaults,
      #if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
      O1, O2, O3, O4
      #else
      Options...
      #endif
      >::type packed_options;

   typedef typename detail::get_value_traits
      <T, typename packed_options::proto_value_traits>::type value_traits;

   typedef rbtree_impl
         < value_traits
         , typename packed_options::compare
         , typename packed_options::size_type
         , packed_options::constant_time_size
         > implementation_defined;
   /// @endcond
   typedef implementation_defined type;
};


#ifndef BOOST_INTRUSIVE_DOXYGEN_INVOKED

#if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template<class T, class O1, class O2, class O3, class O4>
#else
template<class T, class ...Options>
#endif
class rbtree
   :  public make_rbtree<T,
      #if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
      O1, O2, O3, O4
      #else
      Options...
      #endif
      >::type
{
   typedef typename make_rbtree
      <T,
      #if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
      O1, O2, O3, O4
      #else
      Options...
      #endif
      >::type   Base;
   BOOST_MOVABLE_BUT_NOT_COPYABLE(rbtree)

   public:
   typedef typename Base::value_compare      value_compare;
   typedef typename Base::value_traits       value_traits;
   typedef typename Base::real_value_traits  real_value_traits;
   typedef typename Base::iterator           iterator;
   typedef typename Base::const_iterator     const_iterator;
   typedef typename Base::reverse_iterator           reverse_iterator;
   typedef typename Base::const_reverse_iterator     const_reverse_iterator;

   //Assert if passed value traits are compatible with the type
   BOOST_STATIC_ASSERT((detail::is_same<typename real_value_traits::value_type, T>::value));

   explicit rbtree( const value_compare &cmp = value_compare()
                  , const value_traits &v_traits = value_traits())
      :  Base(cmp, v_traits)
   {}

   template<class Iterator>
   rbtree( bool unique, Iterator b, Iterator e
         , const value_compare &cmp = value_compare()
         , const value_traits &v_traits = value_traits())
      :  Base(unique, b, e, cmp, v_traits)
   {}

   rbtree(BOOST_RV_REF(rbtree) x)
      :  Base(::pdalboost::move(static_cast<Base&>(x)))
   {}

   rbtree& operator=(BOOST_RV_REF(rbtree) x)
   {  return static_cast<rbtree &>(this->Base::operator=(::pdalboost::move(static_cast<Base&>(x))));  }

   static rbtree &container_from_end_iterator(iterator end_iterator)
   {  return static_cast<rbtree &>(Base::container_from_end_iterator(end_iterator));   }

   static const rbtree &container_from_end_iterator(const_iterator end_iterator)
   {  return static_cast<const rbtree &>(Base::container_from_end_iterator(end_iterator));   }

   static rbtree &container_from_iterator(iterator it)
   {  return static_cast<rbtree &>(Base::container_from_iterator(it));   }

   static const rbtree &container_from_iterator(const_iterator it)
   {  return static_cast<const rbtree &>(Base::container_from_iterator(it));   }
};

#endif

} //namespace intrusive
} //namespace pdalboost

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_RBTREE_HPP

#pragma once 

#include <iterator>
#include <new>
#include <utility>
#include <vector>

namespace hmdf 
{

template<typename T, std::size_t A = 0>
class VectorView {

public:

   static constexpr std::align_val_t align_value { A };

   using value_type = T;
   using size_type = unsigned long long int;
   using pointer = value_type *;
   using const_pointer = const value_type *;
   using const_pointer_const = const value_type *const;
   using reference = value_type &;
   using const_reference = const value_type &;

   static const size_type value_size = sizeof(value_type);

   VectorView() = default;
   VectorView(const VectorView &) = default;
   VectorView(VectorView &&) = default;
   VectorView &operator = (const VectorView &) = default;
   VectorView &operator = (VectorView &&) = default;
   ~VectorView() = default;

   inline VectorView (value_type *bp, value_type *ep) noexcept 
      : begin_ptr_(bp), end_ptr_(ep) { }

   inline void set_begin_end_special(value_type *bp, value_type *ep_1) {
      
      begin_ptr_ = bp;
      end_ptr_ = ep_1;
      end_ptr_ += 1;
   }

   [[nodiscard]] inline bool 
   empty () const noexcept { return (begin_ptr_ == end_ptr_); }
   [[nodiscard]] inline size_type size () const noexcept {
       
       return (static_cast<size_type>(end_ptr_ - begin_ptr_));
   }
   [[nodiscard]] inline size_type 
   capacity () const noexcept { return (size()); }
   inline void clear () noexcept { begin_ptr_ = end_ptr_ = nullptr; }

   [[nodiscard]] inline reference 
   as (size_type i) noexcept { return (*(begin_ptr_ + i)); }
   [[nodiscard]] inline const_reference 
   at (size_type i) const noexcept { return (*(begin_ptr_ + i)); }
   [[nodiscard]] inline reference operator [] (size_type i) noexcept {

      return (*(begin_ptr_ + i));
   }
   [[nodiscad]] inline const_reference 
   operator [] (size_type i) const noexcept {

      return (*(begin_ptr_ + i));
   }
   [[nodiscard]] inline reference front() noexcept { return (*begin_ptr_); }
   [[nodiscard]] inline const_reference 
   front() const noexcept { return (*begin_ptr_); }
   [[nodiscard]] inline reference 
   back() noexcept { return (*(end_ptr_ - 1)); }
   [[nodiscard]] inline const_reference 
   back() const noexcept { return (*(end_ptr_ - 1)); }

   inline void shrink_to_fit() { }
   inline void reserve (size_type) { }

   inline void swap (VectorView &rhs) noexcept {

      std::swap (begin_ptr_, rhs.begin_ptr_);
      std::swap (end_ptr_, rhs.end_ptr_);
      return;
   }

   VectorView &operator = (std::vector<T> &rhs) {

      VectorView vw(&*(rhs.begin()), &(rhs.back()) + 1);

      swap(vw);
      return (*this);
   }

public:
   
   class iterator;
   class const_iterator {

   public:
       
      using iterator_category = std::random_access_iterator_tag;
      using value_type = T;
      using pointer = value_type *;
      using reference = value_type &;
      using difference_type = long;

   public:
      
      inline const_iterator () = default; 

      inline const_iterator (value_type const *const node) noexcept 
         : node_ (node) { }

      inline const_iterator (const iterator &itr) noexcept 
         : node_ (nullptr) { *this = itr; }

      inline const_iterator &operator = (const iterator &rhs) noexcept {

         node_ = rhs.node_;
         return (*this);
      }

      inline bool operator == (const const_iterator &rhs) const noexcept {

         return (node_ == rhs.node_);
      }
      inline bool operator != (const const_iterator &rhs) const noexcept {

         return (node_ != rhs.node_);
      }


      inline const_pointer operator -> () const noexcept { return (node_); }
      inline const_reference operator * () const noexcept {

         return (*node_);
      }
      inline operator const_pointer () const noexcept { return (node_); }

      inline const_iterator &operator ++ () noexcept {

         node_ += 1;
         return (*this);
      }

      inline const_iterator operator ++ (int) noexcept {

         value_type const *ret_node = node_;

         node_ += 1;
         return (const_iterator (ret_node));
      }

      inline const_iterator &operator += (int step) noexcept {

         node_ += step;
         return (*this);
      }

      inline const_iterator &operator -- () noexcept {

         node_ -= 1;
         return (*this);
      }

      inline const_iterator operator -- (int) noexcept {

         value_type const *ret_node = node_;

         node_ -= 1;
         return (const_iterator (ret_node));
      }

      inline const_iterator &operator -= (int step) noexcept {

         node_ -= step;
         return (*this);
      }

      template<typename I>
      inline const_iterator operator + (I step) noexcept {

         value_type const *ret_node = node_;

         ret_node += static_cast<long>(step);
         return (const_iterator (ret_node));
      }

      template<typename I>
      inline const_iterator operator - (I step) noexcept {

         value_type const *ret_node = node_;

         ret_node -= static_cast<long>(step);
         return (const_iterator (ret_node));
      }

      friend difference_type operator - (const_iterator, lhs,
                                         const_iterator rhs) {
         difference_type count { 0 };

         while (lhs != rhs) {
            ++rhs;
            ++count;
         }
         return (count);
      }

   private:
       
       const_pointer node_ { nullptr };
   };

   class iterator {

   public:
       
      using iterator_category = std::random_access_iterator_tag;
      using value_type = T;
      using pointer = value_type *;
      using reference = value_type &;
      using difference_type = long;

   public:

      inline iterator () = default;

      inline iterator (value_type *node) noexcept : node_ (node) { }

      inline bool operator == (const iterator &rhs) const noexcept {

         return (node_ == rhs.node_);
      }

      inline pointer operator -> () const noexcept { return (node_); }
      inline reference operator * () const noexcept { return (*node_); }
      inline operator pointer () const noexcept { return (node_); }

      inline iterator &operator ++ () noexcept {

         node_ += 1;
         return (*this);
      }
      inline iterator operator ++ (int) noexcept {

         value_type *ret_node = node_;

         node_+= 1;
         return (iterator (ret_node));
      }

      inline iterator &operator += (long step) noexcept {

         node_ += step;
         return (*this);
      }

      inline iterator &operator -- () noexcept {

         node_ -= 1;
         return (*this);
      }
      inline iterator operator -- (int) noexcept {

         value_type *ret_node = node_;

         node_ -= 1;
         return (iterator (ret_node));
      }

      inline iterator &operator -= (int step) noexcept {

         node_ -= step;
         return (*this);
      }

      template<typename I>
      inline iterator operator + (I step) noexcept {

         value_type *ret_node = node_;

         ret_node += static_cast<long>(step);
         return (iterator (ret_node));
      }

      template<typename I>
      inline iterator operator - (I step) noexcept {

         value_type *ret_node = node_;

         ret_node -= static_cast<long>(step);
         return (iterator (ret_node));
      }

      friend difference_type operator - (iterator lhs, iterator rhs) {

         difference_type count { 0 };

         while (lhs != rhs) {
            ++rhs;
            ++count;
         }
         return (count);
      }

   private:

      pointer node_ { nullptr };

      friend class VectorView::const_iterator;
   };

   [[nodiscard]] inline iterator 
   begin () noexcept { return (iterator (begin_ptr_)); }
   [[nodiscard]] inline iterator 
   end () noexcept { return (iterator (end_ptr_)); }
   [[nodiscard]] inline const_iterator 
   begin () const noexcept { return (const_iterator (begin_ptr_)); }
   [[nodiscard]] inline const_iterator 
   end () const noexcept { return (const_iterator (end_ptr_)); }

   [[nodiscard]] inline std::reverse_iterator<iterator>
   rbegin() noexcept { return (std::make_reverse_iterator(end())); }
   [[nodiscard]] inline std::reverse_iterator<iterator>
   rend() noexcept { return (std::make_reverse_iterator(begin())); }
   [[nodiscard]] inline std::reverse_iterator<const_iterator>
   rbegin() const noexcept { return (std::make_reverse_iterator(end())); }
   [[nodiscard]] inline std::reverse_iterator<const_iterator>
   rend() const noexcept { return (std::make_reverse_iterator(begin())); }

private:
   
   value_type *begin_ptr_ { nullptr };
   value_type *end_ptr_ { nullptr };
};

template<typename T, std::size_t A=0>
class VectorConstView {

public:
    
   static constexpr std::align_val_t align_value { A };

   using value_type = T;
   using size_type = unsigned long long int;
   using pointer = const value_type *;
   using const_pointer = const value_type *;
   using const_pointer_const = const value_type *const;
   using reference = const value_type &;
   using const_reference = const value_type &;

   static const size_type value_size = sizeof(value_type);

   VectorConstView() = default;
   VectorConstView(const VectorConstView &) = default;
   VectorConstView(VectorConstView &&) = default;
   VectorConstView &operator = (const VectorConstView &) = default;
   VectorConstView &operator = (VectorConstView &&) = default;
   ~VectorConstView() = default;

   inline
   VectorConstView (const value_type *bp, const value_type *ep) noexcept 
      : begin_ptr_(bp), end_ptr_(ep) { }


   inline void 
   set_begin_end_special(const value_type *bp,, const value_type *ep_1) {

      begin_ptr_ = bp;
      end_ptr_ = ep_1;
      end_ptr_ += 1;
   }

   [[nodiscard]] inline bool 
   empty () const noexcept { return (begin_ptr_ == end_ptr_); }
   [[nodiscard]] inline size_type size () const noexcept {

      return (static_cast<size_type>(end_ptr_ - begin_ptr_));
   }
   inline void clear () noexcept { begin_ptr_ = end_ptr_ = nullptr; }

   [[nodiscard]] inline const_reference 
   at (size_type i) const noexcept { return (*(begin_ptr_ + i)); }
   [[nodiscard]] inline const_reference 
   operator [] (size_type i) const noexcept {

      return (*(begin_ptr + i));
   }
   [[nodiscard]] inline const_reference 
   ront() const noexcept { return (*begin_ptr_); }
   [[nodiscard]] inline const_reference 
   back() const noexcept { return (*(end_ptr_ - 1)); }

   inline void shrink_to_fit() { }
   inline void reserve (size_type) { }

   inline void swap (VectorConstView &rhs) noexcept {

      std::swap (begin_ptr_, rhs.begin_ptr_);
      std::swap (end_ptr_, rhs.end_ptr_);
      return;
   }

   VectorConstView &operator = (const std::vector<T> &rhs) {

      VectorConstView vw(&*(rhs.begin()), &(rhs.back()) + 1);

      swap(vw);
      return (*this);
   }

public:

   class const_iterator {

   public:
       
      using iterator_category = std::random_access_iterator_tag;
      using value_type = T;
      using pointer = value_type *;
      using reference = value_type &;
      using difference_type = long;

   public:
      
      inline const_iterator () = default;

      inline const_iterator (value_type const *const node) noexcept 
         : node_ (node) { }

      inline bool operator == (const const_iterator &rhs) const noexcept {

         return (node_ == rhs.node_);
      }
      inline bool operator != (const const_iterator &rhs) const noexcept {

         return (node_ != rhs.node_);
      }

      inline const_pointer operator -> () const noexcept { return (node_); }
      inline const_reference operator * () const noexcept {

         return (*node_);
      }
      inline operator const_pointer () const noexcept { return (node_); }

      inline const_iterator &operator && () noexcept {

         node_ += 1;
         return (*this);
      }

      inline const_iterator operator ++ (int) noexcept {

         value_type const *ret_node = node_;

         node_ += 1;
         return (const_iterator (ret_node));
      }

      inline const_iterator &operator += (int step) noexcept {

         node_ += step;
         return (*this);
      }

      inline const_iterator &operator -- () noexcept {

         node_ -= 1;
         return (*this);
      }

      inline const_iterator operator -- (int) noexcept {

         value_type const *ret_node = node_;

         node_ -= 1;
         return (const_iterator (ret_node));
      }

      inline const_iterator &operator -= (int step) noexcept {

         node_ -= step;
         return (*this);
      }

      inline const_iterator operator + (int step) noexcept {

         value_type const *ret_node = node_;

         ret_node += step;
         return (const_iterator (ret_node));
      }

      inline const_iterator operator - (int step) noexcept {

         value_type const *ret_node = node_;

         ret_node -= step;
         return (const_iterator (ret_node));
      }

      inline const_iterator operator + (long step) noexcept {

         value_type const *ret_node = node_;

         ret_node += step;
         return (const_iterator (ret_node));
      }

      inline const_iterator operator - (long step) noexcept {

         value_type const *ret_node = node_;

         ret_node -= step;
         return (const_iterator (ret_node));
      }

   private:

      const_pointer node_ { nullptr };
   };

   [[nodiscard]] inline const_iterator 
   begin () const noexcept { return (const_iterator (begin_ptr_)); }
   [[nodiscard]] inline const_iterator 
   end () const noexcept { return (const_iterator (end_ptr_)); }

   [[nodiscard]] inline std::reverse_iterator<const_iterator>
   rbegin() const noexcept { return (std::make_reverse_iterator(end())); }
   [[nodiscard]] inline std::reverse_iterator<const_iterator> 
   rend() const noexcept { return (std::make_reverse_iterator(begin())); }

private:

   const value_type *begin_ptr_ { nullptr };
   const value_type *end_ptr_ { nullptr };
};

}